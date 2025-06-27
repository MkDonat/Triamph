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
  writting_button_message("COLLECT");
}
void onLongPress_B(){
  writting_button_message("BACK2HOME");
}
void duringLongPress_B(){
  writting_button_message("");
}
/* -A- */
void onClick_A(){
  writting_button_message("CLAMPS");
}
void onLongPress_A(){
  writting_button_message("LOAD_UNLOAD");
}
void duringLongPress_A(){
  writting_button_message("");
}
/* -L3- */
void onClick_L3(){
  writting_button_message("L3_Click");
}
void onLongPress_L3(){
  writting_button_message("L3_Long_Press");
}
void duringLongPress_L3(){
  writting_button_message("");
}