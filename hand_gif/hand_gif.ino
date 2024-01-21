#include <SPI.h>
#include <TFT_eSPI.h>

#include "Orbitron_Medium_14.h"  // UTF-8 Encoding!
#include "RTClib.h"
#include "graph.h"
#include "hand.h"
#include "scull.h"
#include "sonar.h"

#define R_BTN 14

RTC_DS3231 rtc;

TFT_eSPI tft = TFT_eSPI();
uint32_t tmr1 = 0;
uint32_t debounce = 0;
uint8_t iter = 0;
bool int_flag = false;

// Debounce
void ButtonTick() {
  if (millis() - debounce >= 10 && digitalRead(R_BTN)) {
    debounce = millis();
    int_flag = true;
  }
}

void setup() {
  // screen
  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);
  // text part
  tft.setFreeFont(&Orbitron_Medium_14);
  // button
  pinMode(R_BTN, INPUT_PULLUP);
  attachInterrupt(R_BTN, ButtonTick, CHANGE);
  // clock
  if (!rtc.begin()) {
    Serial.println("Could not find RTC! Check circuit.");
  }
  rtc.adjust(DateTime(__DATE__, __TIME__));
}

template <size_t rows, size_t cols>
void ShowGif(const uint16_t (&arr)[rows][cols], uint16_t width, uint16_t height,
             uint8_t delay) {
  for (uint8_t i = 0; i < rows;) {
    if (millis() - tmr1 >= delay) {
      tmr1 = millis();
      tft.pushImage(0, 0, width, height, arr[i]);  // (50,0...) center alignment
      ++i;
    }
  }
}

template <size_t rows, size_t cols>
void ShowFrwBckwrdGif(const uint16_t (&arr)[rows][cols], uint16_t width, uint16_t height,
             uint8_t delay) {
  for (uint8_t i = 0; i < rows;) {
    if (millis() - tmr1 >= delay) {
      tmr1 = millis();
      tft.pushImage(0, 0, width, height, arr[i]);  // (50,0...) center alignment
      ++i;
    }
  }
  for (uint8_t i = rows-2; i > 0;) {
    if (millis() - tmr1 >= delay) {
      tmr1 = millis();
      tft.pushImage(0, 0, width, height, arr[i]);  // (50,0...) center alignment
      --i;
    }
  }
}

void ShowTime() {
  // tft.drawString("Orbitron",0,0);
  DateTime now = rtc.now();
  tft.drawNumber((now.hour(), DEC), 0, 0);
  tft.drawNumber((now.minute(), DEC), 40, 0);
  tft.drawNumber((now.second(), DEC), 80, 0);
}

void loop() {
  // button part: change screen
  if (int_flag) {
    int_flag = false;
    iter++;
    if (iter == 5) iter = 0;
    tft.fillScreen(TFT_BLACK);
  }
  // image part
  switch (iter) {
    case 0:
      ShowTime();
      break;

    case 1: {
      // Hand gif
      ShowGif(hand, 220, 170, 100);
    } break;

    case 2: {
      // Scull gif
      ShowFrwBckwrdGif(scull, 320, 150, 200);
    } break;

    case 3: {
      // Sonar gif
      ShowGif(sonar, 265, 170, 200);
    } break;

    case 4: {
      // Graph gif
      ShowFrwBckwrdGif(graph, 275, 170, 85);
    } break;

    default:
      break;
  }
}


