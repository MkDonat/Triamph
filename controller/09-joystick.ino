byte joystick_X = 36; // 36 & 39(VN) PINs are INPUT ONLY PINs
byte joystick_Y = 39;

void setup_joystick(){
  pinMode(joystick_X, INPUT);
  pinMode(joystick_Y, INPUT);
}