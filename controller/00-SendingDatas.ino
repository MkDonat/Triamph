typedef struct { //150 Bytes MAX
  //COMMANDE DES GAZ
  uint32_t gaz_value = 0;
  //COMMANDE JOYSTICK
  int8_t joystick_tor_x = 0;
  int8_t joystick_tor_y = 0;
} SendingPacketData;

SendingPacketData SendingData;