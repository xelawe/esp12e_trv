void check_analog() {
 // int lv_V = analogRead(analogInPin);
 // current_V = lv_V;
  current_V = analogRead(analogInPin);
}


void init_analog() {
  current_V = analogRead(analogInPin);
}
