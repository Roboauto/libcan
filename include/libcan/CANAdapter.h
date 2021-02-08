/**
 * @file
 * This file declares a generic CANAdapter,
 * to facilitate frame transmission and reception.
 */

#ifndef CAN_ADAPTER_H
#define CAN_ADAPTER_H

#include <unistd.h>
#include <errno.h>

#include <libcan/CANFrame.h>
#include <libcan/CANFrameParser.h>

#include <functional>


namespace libcan {
  /**
   * Specifies the type of a CAN adapter
   */
  typedef enum
  {
      ADAPTER_NONE,
      ADAPTER_SOCKETCAN,
  } can_adapter_t;

  /**
   * How a frame reception handler should look like
   */
  using reception_handler_t = std::function<void(can_frame_t*)>;

  /**
   * error handler for
   */
  using error_handler_handler_t = std::function<void(can_frame_t*, error_t)>;


  /**
   * Facilitates frame transmission and reception via a CAN adapter
   */
  class CANAdapter
  {
    protected:
      can_adapter_t adapter_type;

    public:
      /**
       * Pointer to a function which shall be called
       * when frames are being received from the CAN bus
       */
      reception_handler_t reception_handler;

      /**
       * Error handler called when the error occurred
       */
      error_handler_handler_t error_handler;

      /**
       * Pointer to a CAN frame parser object
       * which's parse_frame() method will be called when frames are received
       */
      CANFrameParser* parser = NULL;

      /** Constructor */
          CANAdapter();
          /** Destructor */
          virtual ~CANAdapter();

          /**
           * Sends the referenced frame to the bus
           */
          virtual void transmit(can_frame_t*);
  };

}

#endif
