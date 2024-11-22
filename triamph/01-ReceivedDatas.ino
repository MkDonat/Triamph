typedef struct PacketData { //150 Bytes MAX
  uint32_t gaz_lecture = 0;
  const uint8_t gaz_err = 5;
} PacketData;

PacketData receivedData;