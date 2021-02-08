/**
 * @file
 * This file declares an interface to SocketCAN,
 * to facilitates frame transmission and reception.
 */

#pragma once

#include <libcan/CANAdapter.h>
#include <libcan/CANFrame.h>
#include <net/if.h>
#include <thread>
#include <atomic>

namespace libcan {

  /**
   * Interface request structure used for socket ioctl's
   */
  using interface_request_t = ifreq;

  /**
   * Socket address type for CAN sockets
   */
  using can_socket_address_t = sockaddr_can;


  /**
   * Facilitates frame transmission and reception via a CAN adapter
   */
  class SocketCAN: public CANAdapter {
    private:
      interface_request_t if_request;

      can_socket_address_t addr;

      std::thread receiver_thread;

      /**
       * Starts a new thread, that will wait for socket events
       */
      void start_receiver_thread();

    public:
      /**
       * CAN socket file descriptor
       */
      int sockfd = -1;

      /**
       * Request for the child thread to terminate
       */
      std::atomic_bool terminate_receiver_thread;

      SocketCAN();
      ~SocketCAN();

      /**
       * Open and bind socket
       * @retval true - open succeed
       * @retval false - open failed
       */
      bool open(const char*);

      /**
       * Close and unbind socket
       *
       * @retval true - close succeed
       * @retval false - close failed
       */
      bool close();

      /**
       * Returns whether the socket is open or closed
       *
       * @retval true     Socket is open
       * @retval false    Socket is closed
       */
      bool is_open();

      /**
       * Sends the referenced frame to the bus
       */
      void transmit(can_frame_t*);
  };

}
