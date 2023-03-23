void pub_sens() {

  char buffer[256];

  // Allocate the JSON document
  //
  // Inside the brackets, 200 is the RAM allocated to this document.
  // Don't forget to change this value to match your requirement.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<200> jsondoc;

  // StaticJsonObject allocates memory on the stack, it can be
  // replaced by DynamicJsonDocument which allocates in the heap.
  //
  // DynamicJsonDocument  doc(200);

  // Add values in the document

  // Add the "ANALOG"
  JsonObject analog = jsondoc.createNestedObject("ANALOG");

  analog["Voltage"] = (int)(current_V);

  // Add the "BMP280"
  JsonObject bmp280 = jsondoc.createNestedObject("BMP280");
  if (gv_bmp_ok) {
    bmp280["Temp"] = gv_bmp_temp;
    bmp280["Status"] = "OK";
  } else {
    bmp280["Status"] = "ERROR";
  }

  // Add the "TRV"
  JsonObject trv = jsondoc.createNestedObject("TRV");

  //  if (!gv_error) {
  trv["Position"] = (int)(gv_act_pos);
  trv["State"] = gv_state;
  //  } else {
  //    trv["Status"] = "ERROR";
  //  }

  int n  = serializeJson(jsondoc, buffer);

  client.publish(mqtt_GetTopic_P(gv_ptopic, mqtt_pre_tele, gv_clientname, (PGM_P)F("SENSOR")), buffer, true);
}

void check_mqtt_local() {
  check_mqtt_reset();

  if (gv_pub_sens) {
    pub_sens();
    gv_pub_sens = false;
  }

}

void init_mqtt_local( ) {
  gv_dest_pos_mqtt = -1;
  add_subtopic(mqtt_GetTopic_P(gv_stopic[0], mqtt_pre_cmnd, gv_clientname, get_stopic_ix(0)), callback_pos);

  init_mqtt(gv_clientname);
}

void callback_pos(String topic, byte* message, unsigned int length) {
  String messageTemp;
  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }
  //Serial.println(messageTemp);

  gv_dest_pos_mqtt = (int)payload_to_float( message, length);
  //gv_waterl_ok = true;
}

float payload_to_float( byte* payload, unsigned int length) {

  float lv_float = 0;
  float lv_fact = 0;
  float lv_sign = 1;

  for (int i = 0; i < length; i++) {
    DebugPrint((char)payload[i]);
  }
  DebugPrintln("");

  for (int i = 0; i < length; i++) {

    switch (payload[i]) {
      case 45: //"-"
        lv_sign = -1;
        break;

      case 46: // "."
        lv_fact = 1;
        break;

      default:
        lv_float = (lv_float * 10) + payload[i] - 48;
        lv_fact = lv_fact * 10;
        break;
    }

  }

  if (lv_fact > 0) {
    lv_float = lv_float / lv_fact;
  }

  lv_float = lv_float * lv_sign;

  DebugPrintln(lv_float);

  return lv_float;
}

char *get_stopic_ix( int ix ) {
  strcpy_P(gv_sbuffer, (char*)pgm_read_dword(&(gt_stopic[ix])));
  //strcpy_P(gv_buffer, );
  return gv_sbuffer;
}
