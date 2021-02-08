
#include <libcan/CANAdapter.h>
#include <cstdio>


namespace libcan {

  CANAdapter::CANAdapter()
      :adapter_type(ADAPTER_NONE),
       reception_handler(nullptr),
       error_handler(nullptr)
  {
  }

  void CANAdapter::transmit(can_frame_t*)
  {
      if (adapter_type == ADAPTER_NONE)
      {
          printf("Unable to send: Unspecified CAN adapter\n");
          return;
      }
  }

}