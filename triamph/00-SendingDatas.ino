typedef struct { //250 Bytes MAX
  bool is_on_water = false;
  uint16_t speed_mps = 0;
  char active_state_name[12];
} SendingPacketData;

SendingPacketData SendingData;