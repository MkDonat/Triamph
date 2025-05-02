typedef struct { //250 Bytes MAX
  //triggers
  uint16_t left_trigger_value = 0;
  uint16_t right_trigger_value = 0;
  //Buttons
  char button_msg[16] = "";
  // joysticks
  int8_t L_J_tor_x = 0;
  int8_t L_J_tor_y = 0;
  int16_t L_J_analog_x = 0;
  int16_t L_J_analog_y = 0;
} SendingPacketData;

SendingPacketData SendingData;