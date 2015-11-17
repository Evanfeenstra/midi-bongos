#include "FastLED.h"
#define PIN 6
#define NUM 64

CRGB leds[NUM];

const int wait = 25;
const int threshold = 202;
const int channel = 1;

void setup() {
  FastLED.addLeds<NEOPIXEL, PIN>(leds, NUM);
  FastLED.show();
  pinMode(13, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(0, INPUT_PULLUP);
  //usbMIDI.setHandleNoteOn(OnNoteOn);   //FOR OUTGOING DATA
}

int ledState = false;
int ledState22 = false;
int piezo = 0;
int piezo22 = 0;
int j=0;

int footpedal = 1 ;
int oldFootpedal = 1;

void loop() {
  //usbMIDI.read(); // USB MIDI receive
  piezo = analogRead(15);  
  piezo22 = analogRead(17);   

  if (piezo>threshold) {//abs(piezo-oldPiezo)>threshold) {
      ledState = true;
      digitalWrite(13, HIGH);
      usbMIDI.sendNoteOn(60, piezo, channel);
      goLed2();
  }
  else { 
    if(ledState==true) {
      digitalWrite(13, LOW); 
      //usbMIDI.sendNoteOff(60, 0, channel);
    }
    ledState=false; 
  }
 
  //kick
  if (piezo22>(threshold+30)) {//abs(piezo22-oldPiezo22)>threshold) {
      ledState22 = true;
      digitalWrite(11, HIGH);
      usbMIDI.sendNoteOn(61, piezo22, channel);
      goLed();
  }
  else {
    if(ledState22==true) {
      digitalWrite(11, LOW); 
      //usbMIDI.sendNoteOff(61, 0, channel); 
    } 
    ledState22=false; 
  }
  
  //FOOTPEDAL
  oldFootpedal = footpedal;
  footpedal = digitalRead(0);
  if (footpedal != oldFootpedal) {
    usbMIDI.sendControlChange(88, footpedal, channel);
  }

  if(j<255){j++;}
  else{j=0;}
  delay(wait);  // delay to avoid overloading the serial port buffer
}

void goLed() {
 for(int k=254; k>0; k=k-2) {
  int kk=k/2;
    for(int i=31; i>=0; i--) {
      leds[i]=CHSV(128-i*2-j+kk,187,k );
    }
    FastLED.show();
  }
}

void goLed2() {
 for(int k=254; k>0; k=k-2) {
  int kk=k/2;
    for(int i=32; i<64; i++) {
      leds[i]=CHSV(i*2+j-kk,187,k );
    }
    FastLED.show();
  }
}

