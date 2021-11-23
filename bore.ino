// ATMEL ATMEGA8 & 168 / ARDUINO
//
//                  +-\/-+
//            PC6  1|    |28  PC5 (AI 5)
//      (D 0) PD0  2|    |27  PC4 (AI 4)
//      (D 1) PD1  3|    |26  PC3 (AI 3)
//      (D 2) PD2  4|    |25  PC2 (AI 2)
// PWM+ (D 3) PD3  5|    |24  PC1 (AI 1)
//      (D 4) PD4  6|    |23  PC0 (AI 0)
//            VCC  7|    |22  GND
//            GND  8|    |21  AREF
//            PB6  9|    |20  AVCC
//            PB7 10|    |19  PB5 (D 13)
// PWM+ (D 5) PD5 11|    |18  PB4 (D 12)
// PWM+ (D 6) PD6 12|    |17  PB3 (D 11) PWM
//      (D 7) PD7 13|    |16  PB2 (D 10) PWM
//      (D 8) PB0 14|    |15  PB1 (D 9) PWM
//                  +----+

#include <Wire.h>

#define PIN_VBAT (A0)
#define PIN_IBAT (A1)

#define PIN_PUMP (8)
#define PIN_VALVE_1 (9)
#define PIN_VALVE_2 (4)
#define PIN_VALVE_3 (5)

#define PRES_I2C_ADDR ((uint8_t)72)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(PIN_PUMP, OUTPUT);
  pinMode(PIN_VALVE_1, OUTPUT);
  pinMode(PIN_VALVE_2, OUTPUT);
  pinMode(PIN_VALVE_3, OUTPUT);

  Wire.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(500);
  uint16_t vbat_mv = ((uint16_t)analogRead(PIN_VBAT) * 111) >> 2;
  Serial.println(vbat_mv);
  digitalWrite(PIN_PUMP, LOW);
  delay(500);
  digitalWrite(PIN_PUMP, HIGH);

  Wire.beginTransmission(PRES_I2C_ADDR);
  Wire.endTransmission();
  uint8_t config = 0x8F; // config for gain of 8,
  Wire.beginTransmission(PRES_I2C_ADDR);
  Wire.write(config);
  Wire.endTransmission();
  Wire.requestFrom(PRES_I2C_ADDR, (uint8_t)2);
  int16_t pressure = Wire.read() << 8;
  pressure |= Wire.read();
  pressure *= 19;
  pressure /= 4;
  Serial.println(pressure);
}
