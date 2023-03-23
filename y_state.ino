void check_state( ) {

  if (gv_State_millis < millis()) {
    return;
  }

  if (gv_state != 1) {
    // check_ina219( );
    // check_analog();
  }

  int lv_state_old = gv_state;

  switch (gv_state) {
    case 0: //do stop
      motor_stop_all();
      gv_state = 1;
      break;
    case 1: // stopped

      break;
    case 2: // do calibration
      gv_act_pos = -1;
      motor_start_ventil( vent_open );
      gv_state = 3;
    case 3: // calibration opening
      if (current_V > gv_max_V) {
        motor_stop_ventil();
        gv_state = 4;
      }
      break;
    case 4: // do calibration close
      motor_start_ventil( vent_close );
      gv_max_time = 0;
      gv_state = 5;
      break;

    case 5: // calibration closing
      if (current_V < gv_min_V) {
        motor_stop_ventil();
        gv_state = 1;
        gv_act_pos = 0;
        Serial.print("Max time: ");
        Serial.println(gv_max_time);
      }
      break;
    case 6: // do positioning
      if (gv_act_pos == gv_dest_pos) {
        gv_state = 1;
        break;
      }

      gv_old_pos = gv_act_pos;

      if (gv_act_pos > gv_dest_pos) {
        // millis für neue Position errechnen
        int lv_diff_pos = gv_act_pos - gv_dest_pos;
        int lv_diff_millis = (( gv_max_time * 1000 ) / 100 ) * lv_diff_pos;
        gv_old_millis = millis();
        gv_dest_millis = gv_old_millis + lv_diff_millis;

        motor_start_ventil( vent_close );
        gv_state = 7;
      } else {
        // millis für neue Position errechnen
        int lv_diff_pos = gv_dest_pos - gv_act_pos;
        int lv_diff_millis = (( gv_max_time * 1000 ) / 100 ) * lv_diff_pos;
        gv_old_millis = millis();
        gv_dest_millis = gv_old_millis + lv_diff_millis;

        motor_start_ventil( vent_open );
        gv_state = 8;
      }
      //gv_pos_time = gv_max_time - map(gv_dest_pos, 0, 100, gv_max_time, 0);
      break;
    case 7: // go to position closing
      if (current_V < gv_min_V) {
        motor_stop_ventil();
        gv_act_pos = 0;
        gv_state = 1;
        break;
      }

      // aktuelle Position berechnen
      gv_act_pos = gv_old_pos - (( 100 * ( millis() - gv_old_millis ) ) / ( gv_max_time * 1000 ) );

      if (gv_dest_pos != 0) {
        // nur prüfen, wenn nicht voll geschlossen werden soll
        if (gv_act_pos <= gv_dest_pos) {
          motor_stop_ventil();
          gv_state = 1;
          break;
        }
      }
      break;
    case 8: // go to position opening
      if (current_V > gv_max_V) {
        motor_stop_ventil();
        gv_act_pos = 100;
        gv_state = 1;
        break;
      }

      // aktuelle Position berechnen
      gv_act_pos = gv_old_pos + (( 100 * ( millis() - gv_old_millis ) ) / ( gv_max_time * 1000 ) );

      if (gv_dest_pos != 100) {
        // nur prüfen, wenn nicht voll geöffnet werden soll
        if (gv_act_pos >= gv_dest_pos) {
          motor_stop_ventil();
          gv_state = 1;
          break;
        }
      }
      break;
  }

  if (lv_state_old != gv_state) {
    gv_pub_sens = true;
    Serial.print("State: ");
    Serial.println(gv_state);
  }

  set_state_tact();

  return;

}

void check_state_timer_1s() {

  if (gv_state != 1) {
    // check_ina219( );
    check_analog();
    Serial.println(current_V);
  }

  //  print_ina219();
  //  Serial.print("aktuelle Position ");
  //  Serial.println(gv_act_pos);
  switch (gv_state) {
    case 5: // calibration closing
      gv_max_time = gv_max_time + 1;
      break;
    case 7:
      // gv_act_pos = gv_act_pos - ( 100 / gv_max_time );

      break;
    case 8:
      // gv_act_pos = gv_act_pos + ( 100 / gv_max_time );

      break;
  }
}

void init_state() {
  gv_state = 0;
  set_state_tact();
}

void set_state_tact() {
  gv_State_millis = millis() + 50; // Messung alle 50 ms
}
