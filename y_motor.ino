void motor_start_ventil(uint8_t iv_dir) {
  digitalWrite(DIR_R, iv_dir);
  analogWrite(PWM_R, 0);
  digitalWrite(DIR_L, iv_dir);
  analogWrite(PWM_L, gv_speed);
  gv_motor_run = true;
  if (iv_dir == vent_open) {
    gv_motor_dir = true;
    Serial.println("Motor Open");
  } else {
    gv_motor_dir = false;
    Serial.println("Motor Close");
  }
}

void motor_stop_all() {
  motor_stop_ventil();
  gv_motor_run = false;
}

void motor_stop_ventil() {

  analogWrite(PWM_R, 0);
  digitalWrite(DIR_R, LOW);
  analogWrite(PWM_L, 0);
  digitalWrite(DIR_L, LOW);
  gv_motor_run = false;
}

void init_motor() {

  //analogWriteFreq(1150);

  // initialize pins
  pinMode(PWM_R, OUTPUT);
  pinMode(PWM_L, OUTPUT);
  pinMode(DIR_R, OUTPUT);
  pinMode(DIR_L, OUTPUT);

}
