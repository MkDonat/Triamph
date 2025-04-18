// B_button_Event is a char[16]
//test value
//if (strcmp(SendingData.B_button_Event, "click") == 0)
void onClick_B_button(){
  writting_msg_for_B_button_Event("click");
}
void onLongPressStart_B_button(){
  writting_msg_for_B_button_Event("long_press_start");
}
void writting_msg_for_B_button_Event(char msg[16]){
  strncpy( //writting into char respecting size
    SendingData.B_button_Event,
    msg,
    sizeof(SendingData.B_button_Event)
  );
}
