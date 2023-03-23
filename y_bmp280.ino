void check_bmp280() {
  if (!gv_bmp_ok) {
    return;
  }

  gv_bmp_temp = bmp.readTemperature();
  Serial.print(F("Temperature = "));
  Serial.print(gv_bmp_temp);
  Serial.println(" *C");
}


void init_bmp280() {


  Wire.begin(/*SDA*/D6,/*SCL*/D5); //(NodeMCU SPI)
  unsigned status;

  //status = bmp.begin();
  status = bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
  if (!status) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                     "try a different address!"));
    return;
  }

  gv_bmp_ok = true;

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  check_bmp280();
}
