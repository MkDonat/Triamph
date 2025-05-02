
void writting_button_message(char msg[16]){
  strncpy( //writting into char respecting size
    SendingData.button_msg,
    msg,
    sizeof(SendingData.button_msg)
  );
}
//test value
//if (strcmp(SendingData.B_button_Event, "click") == 0)
/* -B- */
void onClick_B(){
  writting_button_message("B_click");
}
void onLongPress_B(){
  writting_button_message("B_long_press");
}
void duringLongPress_B(){
  writting_button_message("");
}
/* -A- */
void onClick_A(){
  writting_button_message("A_click");
}
void onLongPress_A(){
  writting_button_message("A_long_press");
}
void duringLongPress_A(){
  writting_button_message("");
}
/* -X- */
void onClick_X(){
  writting_button_message("X_click");
}
void onLongPress_X(){
  writting_button_message("X_long_press");
}
void duringLongPress_X(){
  writting_button_message("");
}
/* -Y- */
void onClick_Y(){
  writting_button_message("Y_click");
}
void onLongPress_Y(){
  writting_button_message("Y_long_press");
}
void duringLongPress_Y(){
  writting_button_message("");
}
/* -Left- */
void onClick_LEFT(){
  writting_button_message("LEFT_click");
}
void onLongPress_LEFT(){
  writting_button_message("LEFT_long_press");
}
void duringLongPress_LEFT(){
  writting_button_message("");
}
/* -Right- */
void onClick_RIGHT(){
  writting_button_message("RIGHT_click");
}
void onLongPress_RIGHT(){
  writting_button_message("RIGHT_long_press");
}
void duringLongPress_RIGHT(){
  writting_button_message("");
}
/* -UP- */
void onClick_UP(){
  writting_button_message("UP_click");
}
void onLongPress_UP(){
  writting_button_message("UP_long_press");
}
void duringLongPress_UP(){
  writting_button_message("");
}
/* -Down- */
void onClick_DOWN(){
  writting_button_message("DOWN_click");
}
void onLongPress_DOWN(){
  writting_button_message("DOWN_long_press");
}
void duringLongPress_DOWN(){
  writting_button_message("");
}
/* -Share (HOME)- */
void onClick_SHARE(){
  writting_button_message("SHARE_click");
}
void onLongPress_SHARE(){
  writting_button_message("SHARE_long_press");
}
void duringLongPress_SHARE(){
  writting_button_message("");
}


