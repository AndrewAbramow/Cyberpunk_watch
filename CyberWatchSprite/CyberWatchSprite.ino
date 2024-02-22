#include <TFT_eSPI.h>

#include "Orbitron_Medium_64.h"  // UTF-8 Encoding!
#include <microDS3231.h>
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

bool Begin(void){
    Wire.begin(18, 17);                 // Инит шины
    Wire.beginTransmission(0x68);       // Зовем DS3231 по адресу
    return (!Wire.endTransmission());   // если никто не откликнулся - возвращаем false
}

void setup() {
  // screen
  tft.init();
  tft.setRotation(1);
  //tft.setSwapBytes(true);
  //tft.fillScreen(TFT_BLACK);
  // text part
  //tft.setFreeFont(&Orbitron_Medium_64);
  // sprite
  s.createSprite(320, 170);
  s.setSwapBytes(true);
  //delay(50);
  //ledcSetup(0, 10000, 8);
  //ledcAttachPin(38, 0);
  //ledcWrite(0, brightnesses[5]);
  // button
  pinMode(R_BTN, INPUT_PULLUP);
  attachInterrupt(R_BTN, ButtonTick, CHANGE);
  // clock
  Serial.begin(9600);
    if (!Begin()) {
    Serial.println("DS3231 not found");
    for(;;);
  }
  rtc.setTime(COMPILE_TIME);
  //DateTime now = rtc.getTime();
  // отправляем в rtc
  //rtc.setTime(now);
}

template <size_t rows, size_t cols>
void ShowGif(const uint16_t (&arr)[rows][cols], uint16_t width, uint16_t height,
             uint8_t delay) {
  for (uint8_t i = 0; i < rows;) {
    if (millis() - tmr1 >= delay) {
      tmr1 = millis();
      s.pushImage(0, 0, width, height, arr[i]);  // (50,0...) center alignment
      s.pushSprite(0,0);
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
      s.pushImage(0, 0, width, height, arr[i]);  // (50,0...) center alignment
      s.pushSprite(0,0);
      ++i;
    }
  }
  for (uint8_t i = rows-2; i > 0;) {
    if (millis() - tmr1 >= delay) {
      tmr1 = millis();
      s.pushImage(0, 0, width, height, arr[i]);  // (50,0...) center alignment
      s.pushSprite(0,0);
      --i;
    }
  }
}

void ShowTime() {
  if (millis() - tmr1 >= 1000) {
    tmr1 = millis();
    s.fillSprite(TFT_BLACK);    
    s.setTextColor(TFT_WHITE, 0x014C);
    s.setFreeFont(&Orbitron_Medium_64);
    s.drawString(rtc.getTimeString(), 0, 40);
    s.pushSprite(0,0);
  }
}

void loop() {
  // button part: change screen
  if (int_flag) {
    int_flag = false;
    iter++;
    if (iter == 5) iter = 0;
    s.fillSprite(TFT_BLACK);
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