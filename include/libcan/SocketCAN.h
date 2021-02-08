/**
 * @file
 * This file declares an interface to SocketCAN,
 * to facilitates frame transmission and reception.
 */

#ifndef SOCKETCAN_H
#define SOCKETCAN_H

#include <libcan/CANAdapter.h>
#include <libcan/CANFrame.h>
#include <stdbool.h>
// IFNAMSIZ, ifreq
#include <net/if.h>
// Multi-threading
#include <pthread.h>
#include <atomic>

namespace libcan {

  /**
   * Interface request structure used for socket ioctl's
   */
  typedef struct ifreq interface_request_t;

  /**
   * Socket address type for CAN sockets
   */
  typedef struct sockaddr_can can_socket_address_t;


  /**
   * Facilitates frame transmission and reception via a CAN adapter
   */
  class SocketCAN: public CANAdapter
  {
    private:
      interface_request_t if_request;

      can_socket_address_t addr;

      pthread_t receiver_thread_id;

    public:
      /**
       * CAN socket file descriptor
       */
      int sockfd = -1;

      /**
       * Request for the child thread to terminate
       */
      std::atomic_bool terminate_receiver_thread;

      /** Constructor */
      SocketCAN();
      /** Destructor */
      ~SocketCAN();

      /**
       * Open and bind socket
       * @return true - open succeed, false - open failed
       */
      bool open(const char*);

      /**
       * Close and unbind socket
       * @return true - close succeed, false - close failed
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

      /**
       * Starts a new thread, that will wait for socket events
       */
      void start_receiver_thread();
  };

}

#endif
