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

// #include "TinBus.h"

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

  // tinbusBegin();
}

void loop() {
  static unsigned long minutes = 0;
  // static uint8_t lowBatteryTimer = 240; // disable pump for 4 hours
  static int16_t head_cm = 0;

  // delay(1000);
  // int16_t rxData = tinbusRead();
  // if(rxData >= 0){
  //   Serial.println(rxData, HEX);
  // }

  delay(1000);
  uint16_t vbat_mv = ((uint16_t)analogRead(PIN_VBAT) * 111) >> 2;
  Serial.print("vbat ");
  Serial.println(vbat_mv);

  if (vbat_mv < 13200) {
    // lowBatteryTimer = 220; // disable pump for 4 hours
    minutes = 0; // reset timer
    digitalWrite(PIN_PUMP, LOW);
  }

  delay(1000);
  unsigned long newMinutes = (millis() >> 16); // dt = 65.536 secs
  Serial.print("time ");
  Serial.println(newMinutes);

  // pump control
  if (newMinutes != minutes) {
    minutes = newMinutes;
    // if(lowBatteryTimer){
    //   lowBatteryTimer--;
    // }
    if ((minutes > 0xC0) && (minutes < 0x1C0)) {
      if ((head_cm < 550) && ((minutes & 0x30) != 0x30)) {
        digitalWrite(PIN_PUMP, HIGH);
      }
    } else {
      digitalWrite(PIN_PUMP, LOW);
    }

    // if(((minutes & 0xC0) == 0x80) && (lowBatteryTimer == 0)){
    //   digitalWrite(PIN_PUMP, HIGH);
    // } else {
    //   digitalWrite(PIN_PUMP, LOW);
    // }
  }

  // valve sequencing, 2 minutes per hour per valve
  if (((minutes & 0x3E) == 0x32) && (vbat_mv > 12800)) {
    digitalWrite(PIN_VALVE_1, HIGH);
  } else {
    digitalWrite(PIN_VALVE_1, LOW);
  }
  if (((minutes & 0x3E) == 0x34) && (vbat_mv > 12800)) {
    digitalWrite(PIN_VALVE_2, HIGH);
  } else {
    digitalWrite(PIN_VALVE_2, LOW);
  }
  if (((minutes & 0x3E) == 0x36) && (vbat_mv > 12800)) {
    digitalWrite(PIN_VALVE_3, HIGH);
  } else {
    digitalWrite(PIN_VALVE_3, LOW);
  }

  delay(1000);
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
  head_cm = pressure / 100;
  Serial.print("head ");
  Serial.println(head_cm);
}

// delay(500);
// digitalWrite(PIN_PUMP, HIGH);
// digitalWrite(PIN_VALVE_1, HIGH);
// digitalWrite(PIN_VALVE_2, HIGH);
// digitalWrite(PIN_VALVE_3, HIGH);
