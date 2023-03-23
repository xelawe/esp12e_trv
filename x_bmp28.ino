#include <Wire.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp; // I2C

float gv_bmp_temp;
boolean gv_bmp_ok = false;
