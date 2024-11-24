typedef struct PacketData { //150 Bytes MAX
  //COMMANDE DES GAZ
  uint32_t gaz_lecture = 0;
  const uint8_t gaz_err = 5;
  //COMMANDE JOYSTICK
  int8_t joystick_x_value = 0;
  int8_t joystick_y_value = 0;
  bool joystick_b_value = false;
} PacketData;

PacketData receivedData;