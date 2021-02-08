/**
 * @file
 * This file implements functions to receive
 * and transmit CAN frames via SocketCAN.
 */

#include <libcan/SocketCAN.h>
#include <stdio.h>
// strncpy
#include <string.h>
// close
#include <unistd.h>
// socket
#include <sys/socket.h>
// SIOCGIFINDEX
#include <sys/ioctl.h>


namespace libcan {

  SocketCAN::SocketCAN()
     :CANAdapter(),
      if_request(),
      addr(),
      receiver_thread(),
      sockfd(-1),
      terminate_receiver_thread(false)
  {
      adapter_type = ADAPTER_SOCKETCAN;
  }


  SocketCAN::~SocketCAN()
  {
      if (this->is_open())
      {
          this->close();
      }
  }


  bool SocketCAN::open(const char* interface)
  {
      // Request a socket
      sockfd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
      if (sockfd == -1)
      {
          printf("Error: Unable to create a CAN socket\n");
          return false;
      }

      // Get the index of the network interface
      strncpy(if_request.ifr_name, interface, IFNAMSIZ);
      if (ioctl(sockfd, SIOCGIFINDEX, &if_request) == -1)
      {
          printf("Unable to select CAN interface %s: I/O control error\n", interface);
          // Invalidate unusable socket
          close();
          return false;
      }

      // Bind the socket to the network interface
      addr.can_family = AF_CAN;
      addr.can_ifindex = if_request.ifr_ifindex;
      int rc = bind(
          sockfd,
          reinterpret_cast<struct sockaddr*>(&addr),
          sizeof(addr)
      );
      if (rc == -1)
      {
          printf("Failed to bind socket to network interface\n");
          // Invalidate unusable socket
          close();
          return false;
      }
      // Start a separate, event-driven thread for frame reception
      start_receiver_thread();

      return true;
  }


  bool SocketCAN::close()
  {
      terminate_receiver_thread = true;
      if (!is_open()) {
          return false;
      }

      // Close the file descriptor for our socket
      receiver_thread.join();
      ::close(sockfd);
      sockfd = -1;

      return true;
  }


  bool SocketCAN::is_open()
  {
      return (sockfd != -1);
  }


  void SocketCAN::transmit(can_frame_t* frame)
  {
      CANAdapter::transmit(frame);
      if (!is_open())
      {
          printf("Unable to transmit: Socket not open\n");
          return;
      }

      // TODO
      printf("Transmission via SocketCAN is not yet implemented.\n");
  }


  static void* socketcan_receiver_thread(void* argv)
  {
      /*
       * The first and only argument to this function
       * is the pointer to the object, which started the thread.
       */
      auto* sock = static_cast<SocketCAN*>(argv);

      // Holds the set of descriptors, that 'select' shall monitor
      fd_set descriptors;

      // Highest file descriptor in set
      int maxfd = sock->sockfd;

      // How long 'select' shall wait before returning with timeout
      timeval timeout {};

      // Buffer to store incoming frame
      can_frame_t rx_frame {};

      // Run until termination signal received
      while (!sock->terminate_receiver_thread)
      {
          // Clear descriptor set
          FD_ZERO(&descriptors);
          // Add socket descriptor
          FD_SET(sock->sockfd, &descriptors);

          // Set timeout
          timeout.tv_sec  = 1;
          timeout.tv_usec = 0;

          // Wait until timeout or activity on any descriptor
          if (select(maxfd+1, &descriptors, NULL, NULL, &timeout) != -1)
          {
              ssize_t len = read(sock->sockfd, &rx_frame, CAN_MTU);
              if (len < 0) {
                  if(sock->error_handler) {
                      sock->error_handler(&rx_frame, errno);
                  }
                  continue;
              }
              if (sock->reception_handler) {
                  sock->reception_handler(&rx_frame);
              }
              continue;
          }
          if(sock->error_handler) {
            sock->error_handler(&rx_frame, errno);
          }
      }
      return NULL;
  }


  void SocketCAN::start_receiver_thread()
  {
      terminate_receiver_thread = false;
      receiver_thread = std::thread(&socketcan_receiver_thread, this);
  }

}
