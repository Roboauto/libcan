
#ifndef CAN_FRAME_PARSER
#define CAN_FRAME_PARSER

#include <CANFrame.h>


class CANFrameParser
{
  public:
    virtual void parse_frame(can_frame_t*);
};

#endif
