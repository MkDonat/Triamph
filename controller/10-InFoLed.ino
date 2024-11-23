const byte InfoLed = 2;
bool controller_is_connected_to_peer = &connected_to_peer;

void setup_InfoLed(){
  pinMode(InfoLed, OUTPUT);
  digitalWrite(InfoLed,0);
}
void loop_InfoLed(){
  if(controller_is_connected_to_peer){
    digitalWrite(InfoLed,1);
  }
}