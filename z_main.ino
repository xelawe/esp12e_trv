void setup() {

  cy_serial::start(gc_file);

  init_motor();

  motor_stop_all();
  delay(500);

  //init_ina219();
  init_analog();

  wifi_init(gc_hostname);
  delay(500);

  init_ota_local();

  init_mqtt_local( );

  init_bmp280();

  init_state();

  init_sched();

  gv_state = 2; // calibration

}

void loop() {

  check_ota();

  check_mqtt_local();

  check_state( );

  check_sched();

  if (gv_state == 1) {
    if ( gv_dest_pos_mqtt != -1 ) {
      gv_dest_pos = gv_dest_pos_mqtt;
      gv_state = 6;
      gv_dest_pos_mqtt = -1;
    }
  }

}
