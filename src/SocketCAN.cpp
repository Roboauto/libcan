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
      sockfd(-1),
      receiver_thread_id(0)
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


  void SocketCAN::open(const char* interface)
  {
      // Request a socket
      sockfd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
      if (sockfd == -1)
      {
          printf("Error: Unable to create a CAN socket\n");
          return;
      }

      // Get the index of the network interface
      strncpy(if_request.ifr_name, interface, IFNAMSIZ);
      if (ioctl(sockfd, SIOCGIFINDEX, &if_request) == -1)
      {
          printf("Unable to select CAN interface %s: I/O control error\n", interface);
          // Invalidate unusable socket
          close();
          return;
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
          return;
      }
      // Start a separate, event-driven thread for frame reception
      start_receiver_thread();
  }


  void SocketCAN::close()
  {
      terminate_receiver_thread = true;

      if (!is_open())
          return;

      // Close the file descriptor for our socket
      ::close(sockfd);
      sockfd = -1;
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
      SocketCAN* sock = (SocketCAN*) argv;

      // Holds the set of descriptors, that 'select' shall monitor
      fd_set descriptors;

      // Highest file descriptor in set
      int maxfd = sock->sockfd;

      // How long 'select' shall wait before returning with timeout
      struct timeval timeout;

      // Buffer to store incoming frame
      can_frame_t rx_frame;

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
          if (select(maxfd+1, &descriptors, NULL, NULL, &timeout) == 1)
          {
              ssize_t len = read(sock->sockfd, &rx_frame, CAN_MTU);
              if (len < 0)
                  continue;

              if (sock->reception_handler != NULL)
              {
                  sock->reception_handler(&rx_frame);
              }

              if (sock->parser != NULL)
              {
                  sock->parser->parse_frame(&rx_frame);
              }
          }
      }
      return NULL;
  }


  void SocketCAN::start_receiver_thread()
  {
      /*
       * Frame reception is accomplished in a separate, event-driven thread.
       *
       * See also: https://www.thegeekstuff.com/2012/04/create-threads-in-linux/
       */
      terminate_receiver_thread = false;
      int rc = pthread_create(&receiver_thread_id, NULL, &socketcan_receiver_thread, this);
      if (rc != 0)
      {
          printf("Unable to start receiver thread.\n");
          return;
      }
  }

}
