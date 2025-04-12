typedef struct { //250 Bytes MAX
  //triggers
  uint32_t left_trigger_value = 0;
  uint32_t right_trigger_value = 0;
  // joystick
  int8_t joystick_tor_x = 0;
  int8_t joystick_tor_y = 0;
} ReceivedPacketData;

ReceivedPacketData receivedData;