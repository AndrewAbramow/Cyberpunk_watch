#include <SPI.h>
#include <TFT_eSPI.h>

#include "hand.h"
#include "scull.h"
#include "sonar.h"
#include "graph.h"
#include "Orbitron_Medium_14.h" // UTF-8 Encoding!

#define  R_BTN    14

TFT_eSPI tft = TFT_eSPI();
uint32_t tmr1 = 0;
uint32_t debounce = 0;
uint8_t  iter = 0; 
bool     int_flag = false;

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
  pinMode(R_BTN,INPUT_PULLUP);
  attachInterrupt(R_BTN, ButtonTick, CHANGE);
}

void HandGif() {
  for (uint8_t i = 0; i < hand_frames; ) {
    if (millis() - tmr1 >= 50) { 
      tmr1 = millis();      
      tft.pushImage(0,0,220,170,hand[i]); // (50,0...) center alignment
      ++i;
    }
  }
}

void ScullGif() {
  for (uint8_t i = 0; i < scull_frames; ) { 
    if (millis() - tmr1 >= 200) {  
      tmr1 = millis();      
      tft.pushImage(0,0,320,150,scull[i]); 
      ++i;
    }    
  }
}

void SonarGif() {
  for (uint8_t i=0; i<sonar_frames; ) {
    if (millis() - tmr1 >= 200) { 
      tmr1 = millis();      
      tft.pushImage(0,0,265,170,sonar[i]); 
      ++i;
    }
  }
}


void GraphGif() { 
  for (uint8_t i=0; i<graph_frames;  ) {
    if (millis() - tmr1 >= 85) {  
      tmr1 = millis();      
      tft.pushImage(0,0,275,170,graph[i]); 
      ++i;
    }
  }
  for (uint8_t i=graph_frames-2; i>0;  ) {
    if (millis() - tmr1 >= 85) {  
      tmr1 = millis();      
      tft.pushImage(0,0,275,170,graph[i]); 
      --i;
    }
  }
}

void ShowTime() {
  if (millis() - tmr1 >= 1000) {  
    tmr1 = millis();      
    tft.drawString("Orbitron",235,0);
  }
}

void loop() {
  // button part: change screen
  if (int_flag) {
    int_flag = false;
    iter++;
    if(iter == 5) iter = 0;
    tft.fillScreen(TFT_BLACK);
  }
  // image part
  switch(iter) {
    case 0: ShowTime();   
    break;

    case 1: HandGif();
    break;

    case 2: ScullGif(); 
    break;
  
    case 3: SonarGif();  
    break;

    case 4: GraphGif();   
    break;
  
    default:
    break;
  }  
}


