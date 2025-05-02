/*Collections de fonctions utilisables par tous les servos moteurs
de ce projet
*/
bool is_servo_poses_equal(int16_t A, int16_t B, int16_t e) {
  int16_t debutA = A - e;
  int16_t finA = A + e;
  int16_t debutB = B - e;
  int16_t finB = B + e;
  if(finA < debutB || finB < debutA) return false;
  else return true;
  /*si [A-e;A+e] et [B-e;B+e] se croisent return -> true sinon false
  use cases: utilisé pour comparer deux positions A et B de servos avec une marge d’erreur e
  */
}
/*bool is_Loaded(){
  if(is_servo_poses_equal(LU_LastConsignePose, LU_Loaded_pose, servo_error) == true) return true;
  else return false;
}*/
void servo_rollback(Servo &myservo, uint16_t from, uint16_t to, uint16_t step, uint16_t speed){
  if(from >= to){
    speed = constrain(speed, 1, 100);
    speed = map(speed,0,100,1,1000);
    for (uint16_t pos = from; pos >= to; pos -= step) {
      // in steps of 1 degree
      myservo.write(pos);             
      vTaskDelay(pdMS_TO_TICKS(15000/speed));               
    }
  }
}
void servo_roll(Servo &myservo, uint16_t from, uint16_t to, uint16_t step, uint16_t speed){
  if(from <= to){
    speed = constrain(speed, 1, 100);
    speed = map(speed,0,100,1,1000);
    for (uint16_t pos = from; pos <= to; pos += step){
      // in steps of 1 degree
      myservo.write(pos);         
      vTaskDelay(pdMS_TO_TICKS(15000/speed));               
    }
  }
}

