
#ifndef CAN_FRAME_PARSER
#define CAN_FRAME_PARSER

#include <libcan/CANFrame.h>

namespace libcan {

  class CANFrameParser
  {
    public:
      virtual ~CANFrameParser();
      virtual void parse_frame(can_frame_t*);
  };

}

#endif
