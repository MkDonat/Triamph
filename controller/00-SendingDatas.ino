typedef struct { //250 Bytes MAX
  // gaz command
  uint32_t gaz_value = 0;
  // joystick
  int8_t joystick_tor_x = 0;
  int8_t joystick_tor_y = 0;
} SendingPacketData;

SendingPacketData SendingData;