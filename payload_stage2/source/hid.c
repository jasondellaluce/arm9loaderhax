#include "hid.h"

u32 waitHid() {
    u32 pad_state_old = HID_STATE;
    while (true) {
        u32 pad_state = HID_STATE;
        if (pad_state ^ pad_state_old)
            return ~pad_state;
    }
}

u32 getHid()
{
	return ~HID_STATE;
}
