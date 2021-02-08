/**
 * @file
 * This file defines can_frame_t as a derivative
 * of the CAN frame struct in <linux/can.h>.
 */

#pragma once

#include <linux/can.h>

namespace libcan {

  /**
   * Holds the content of one CAN frame
   * Struct delcared in <linux/can.h>
   */
  using can_frame_t = can_frame;

}
