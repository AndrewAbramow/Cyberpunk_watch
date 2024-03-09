#include <TFT_eSPI.h>
#include <microDS3231.h>

#include "HUD_cropped.h"
#include "Orbitron_Medium_52.h"  // UTF-8 Encoding!
#include "attack.h"
#include "ext_arm.h"
#include "globe.h"
#include "graph.h"
#include "hand.h"
#include "scull.h"
#include "sonar.h"

#define R_BTN 14

MicroDS3231 rtc;

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite s = TFT_eSprite(&tft);

uint32_t tmr1 = 0;
uint32_t debounce = 0;
uint8_t iter = 0;
bool int_flag = false;
int brightnesses[7] = {35, 70, 105, 140, 175, 210, 250};

// Debounce
void ButtonTick() {
  if (millis() - debounce >= 10 && digitalRead(R_BTN)) {
    debounce = millis();
    int_flag = true;
  }
}

bool Begin(void) {
  Wire.begin(18, 17);            // Инит шины
  Wire.beginTransmission(0x68);  // Зовем DS3231 по адресу
  return (
      !Wire.endTransmission());  // если никто не откликнулся - возвращаем false
}

void setup() {
  // screen
  tft.init();
  tft.setRotation(1);
  // sprite
  s.createSprite(320, 170);
  s.setSwapBytes(true);
  // button
  pinMode(R_BTN, INPUT_PULLUP);
  attachInterrupt(R_BTN, ButtonTick, CHANGE);
  // clock
  Serial.begin(9600);
  if (!Begin()) {
    Serial.println("DS3231 not found");
    for (;;);
  }
  rtc.setTime(COMPILE_TIME);
  // battery
  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);
}

template <size_t rows, size_t cols>
void ShowGif(const uint16_t (&arr)[rows][cols], uint16_t width, uint16_t height,
             uint16_t x, uint16_t y, uint8_t delay) {
  for (uint8_t i = 0; i < rows;) {
    if (millis() - tmr1 >= delay) {
      tmr1 = millis();
      s.pushImage(x, y, width, height, arr[i]);  // (50,0...) center alignment
      s.pushSprite(0, 0);
      ++i;
    }
  }
}

template <size_t rows, size_t cols>
void ShowFrwBckwrdGif(const uint16_t (&arr)[rows][cols], uint16_t width,
                      uint16_t height, uint16_t x, uint16_t y, uint8_t delay) {
  for (uint8_t i = 0; i < rows;) {
    if (millis() - tmr1 >= delay) {
      tmr1 = millis();
      s.pushImage(x, y, width, height, arr[i]);  // (50,0...) center alignment
      s.pushSprite(0, 0);
      ++i;
    }
  }
  for (uint8_t i = rows - 2; i > 0;) {
    if (millis() - tmr1 >= delay) {
      tmr1 = millis();
      s.pushImage(x, y, width, height, arr[i]);  // (50,0...) center alignment
      s.pushSprite(0, 0);
      --i;
    }
  }
}

void ShowTime() {
  if (millis() - tmr1 >= 1000) {
    tmr1 = millis();
    s.fillSprite(TFT_BLACK);
    s.pushImage(0, 31, 320, 108, HUD_02);
    s.setTextColor(0xFFE0);
    s.setFreeFont(&Orbitron_Medium_52);
    s.drawString(rtc.getTimeString(), 12, 52);
    s.pushSprite(0, 0);
  }
}

void loop() {
  // button for screen change
  if (int_flag) {
    int_flag = false;
    iter++;
    if (iter == 8) iter = 0;
    s.fillSprite(TFT_BLACK);
  }
  // gif's:
  switch (iter) {
    case 0:
      ShowTime();
      break;

    case 1: {
      ShowGif(hand, 220, 170, 50, 0, 100);
    } break;

    case 2: {
      ShowFrwBckwrdGif(scull, 320, 150, 0, 10, 200);
    } break;

    case 3: {
      ShowGif(sonar, 265, 170, 27, 0, 200);
    } break;

    case 4: {
      ShowFrwBckwrdGif(graph, 275, 170, 22, 0, 85);
    } break;

    case 5: {
      ShowGif(globe, 288, 170, 16, 0, 200);
    } break;

    case 6: {
      ShowGif(attack, 287, 170, 16, 0, 200);
    } break;

    case 7: {
      ShowGif(ext_arm, 227, 170, 46, 0, 200);
    } break;

    default:
      break;
  }
}