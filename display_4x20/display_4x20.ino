//Finlay Robb 2025.02.05
//Muhammad Saad Khan 2025.02.09

//Section 1 - Main Display and Test

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int displaySize[] = {20, 4, 2}; //width, height, amount
LiquidCrystal_I2C displays[] = {
  LiquidCrystal_I2C(0x27, displaySize[0], displaySize[1]),
  LiquidCrystal_I2C(0x28, displaySize[0], displaySize[1]),
};

unsigned int timeOffset = 0;  //unsigned for second values up to full 10 hours
unsigned int startTime = 0;

unsigned int totalTime;
float velocity;
int batteryPercent = 100;
bool motorOn;

unsigned int lapButtonPressedTime = 0;
int lapNum = 0;
int lapAmount;
int lapStartTime;
int prevLapTime;
int bestLapTime = 32767; //max value

int buttonPin = 2;
bool started = false;

//For testing only
int testValues[5] = { 0, 5, 45, 24, 100 };
float testValues2[5] = { 0, 0.1, 0.2362, 4.0, 31.9 };
int v = 0;

uint8_t cust[8] = { 0x1C, 0x10, 0x18, 0x17, 0x15, 0x07, 0x06, 0x05 };

//Variables of the Warning System
//An RBG LED and Active Buzzer will act as a warning for the Driver when the Battery is at 50%, 25%, 10% and OFF

//Pins of RBG LED
const int Red_Pin = 9;
const int Green_Pin = 10;
const int Blue_Pin = 11;

//Active Buzzer
const int buzzerPin = 6;
int buzzesLeft = 0;
bool buzzing = false;
unsigned long int lastBuzz = 0;
int batteryWarningLevel = 0;

void setup() {
  for (int i = 0; i < displaySize[2]; i++) {
    displays[i].init();
    displays[i].backlight();
    displays[i].clear();
  }
  displays[0].print("Welcome. Press");
  displays[0].setCursor(0, 1);
  displays[0].print("button to start");

  //Serial.begin(9600);
  //Serial.println("Hello");

  pinMode(buttonPin, INPUT);

  //Warning System
  pinMode(Red_Pin, OUTPUT);  //Setting Out Warning LED's output pins
  pinMode(Green_Pin, OUTPUT);
  pinMode(Blue_Pin, OUTPUT);
  setColour(0, 255, 0);

  pinMode(buzzerPin, OUTPUT);  //Setting Output of Buzzer
}

void loop() {
  if (started && millis()/1000 - timeOffset >= 1) {  //repeat after button first pressed
    timeOffset = millis()/1000;

    totalTime = millis()/1000 - startTime;
    updateTotalTime(totalTime);
    
    velocity = testValues2[v % 5];
    updateVelocity(velocity);

    batteryPercent = 50;//testValues[v % 5];
    updateBatteryPercent(batteryPercent);

    motorOn = v % 2 == 0;
    updateMotorOn(motorOn);

    updateLapTime(totalTime - lapStartTime);

    //Only needed here for testing otherwise remove
    lapAmount = testValues[(v + 2) % 4];
    updateLapNum(lapNum, lapAmount); 

    v++;  //next testing value
    
    //Operation of Warning System
    batteryWarningSystem();        //Activates method
    checkBuzzer();
  }

  //If button pressed
  if (digitalRead(buttonPin) == HIGH && (millis()/1000 - lapButtonPressedTime > 1)) {
    lapButtonPressed();
  }
}


void lapButtonPressed() {
  lapNum++;
  updateLapNum(lapNum, lapAmount);

  if (!started) {  // If first time pressed
    startTime = millis() / 1000;    // totalTime not set yet
    started = true;
    for (int i = 0; i < displaySize[2]; i++) {
      displays[i].clear();
  	}
    displayLabels();

  } else {
    prevLapTime = totalTime - lapStartTime;
    updatePrevLap(prevLapTime);
    lapStartTime = totalTime;
    updateLapTime(0); //Set the lap time to zero

    if (prevLapTime < bestLapTime) { //If new best lap time
      bestLapTime = prevLapTime;
      updateBestLap(bestLapTime);
    }
  }

  lapButtonPressedTime = millis()/1000;
}

//Structure to store seconds, minutes and hours
struct Time { int s; int m; int h; };
//Funcion to convert total seconds into seconds, minutes and hours
struct Time convertTime(int seconds) {
  struct Time time = { seconds % 60, (seconds % 3600) / 60, seconds / 3600 };
  return time;
}

//Return a nicely formatted string of the time
String timeToString(struct Time time) {
  String result = "";
  result = result + time.h + ":";
  if (time.m <= 9) {
    result = result + "0";
  }
  result = result + time.m + ":";

  if (time.s <= 9) {
    result = result + "0";
  }
  result = result + time.s;
  return result;
  
}

const char * labels[8] = {"Time:        ", "Vel:        ", "Battery:        ", "Motor:           ", "Lap Num:       ", "CurrLap:     ", "PrevLap:     ", "BestLap:     "};
void displayLabels(){
  for (int i = 0; i < 8; i++) {
    displays[i/displaySize[1]].setCursor(0, i%displaySize[1]);
    displays[i/displaySize[1]].print(labels[i]);
  }
}

void updateTotalTime(int seconds) {
  //Time:    0:00:00
  displays[0].setCursor(13, 0);
  //Convert seconds to hours, minutes, & seconds, then format to string, then print to display
  displays[0].print(timeToString(convertTime(seconds)));
}

void updateVelocity(float velocity) {
  //Vel:    00.00ms/s
  displays[0].setCursor(12, 1);
  if (velocity <= 9) {
    displays[0].print(" ");
  }

  displays[0].print(velocity, 2); //to 2 decimal places
  displays[0].print("m/s");
}

void updateBatteryPercent(int percent) {
  //Battery:    100%
  displays[0].setCursor(16, 2);
  if (percent < 100) {
    displays[0].print(" ");
  }
  if (percent < 10) {
    displays[0].print(" ");
  }
  displays[0].print(percent);
  displays[0].print("%");
}

void updateMotorOn(bool on) {
  //Motor:       off
  displays[0].setCursor(17, 3);
  if (on) {
    displays[0].print(" on");
  } else {
    displays[0].print("off");
  }
}

void updateLapNum(int lap, int lapAmount) {
  //Lap Num:   00/00
  displays[1].setCursor(15, 0);
  if (lap <= 9) {
    displays[1].print(" ");
  }
  if (lapAmount <= 9) {
    displays[1].print(" ");
  }
  displays[1].print(lap);
  displays[1].print("/");
  displays[1].print(lapAmount);
}

void updateLapTime(int seconds) {
  //CurrLap: 0:00:00
  displays[1].setCursor(13, 1);
  displays[1].print(timeToString(convertTime(seconds)));
}

void updatePrevLap(int seconds) {
  //PrevLap: 0:00:00
  displays[1].setCursor(13, 2);
  displays[1].print(timeToString(convertTime(seconds)));
}

void updateBestLap(int seconds) {
  //BestLap: 0:00:00
  displays[1].setCursor(13, 3);
  displays[1].print(timeToString(convertTime(seconds)));
}

// Section 2 - Warning System

//Check if the buzzer needs buzzed
void checkBuzzer() {
  if (buzzesLeft > 0 && millis() - lastBuzz > 500) {
    lastBuzz = millis();
    if (!buzzing) {
      tone(buzzerPin, 425);
    } else {
      noTone(buzzerPin);
      buzzesLeft--;
    }
    buzzing = !buzzing;
  }
}

//Setting the colour function
void setColour(int R, int G, int B) {
  analogWrite(Red_Pin, R);
  analogWrite(Green_Pin, G);
  analogWrite(Blue_Pin, B);
}


void batteryWarningSystem() {
  //batteryWarningLevel value added so only new warnings activate buzzer
  if (batteryPercent <= 10 && batteryWarningLevel <= 3) {
    // Critical battery level (10% or below) - Red LED + Five Beeps
    batteryWarningLevel = 4;
    setColour(255, 0, 0);
    buzzesLeft += 5;
	
  } else if (batteryPercent <= 25 && batteryWarningLevel <= 2) {
    // Low battery level (25%) - Yellow LED + Two Beeps
    batteryWarningLevel = 3;
    setColour(255, 255, 0);
    buzzesLeft += 2;
	
  } else if (batteryPercent <= 50  && batteryWarningLevel <= 1) {
    // Medium battery level (50%) - Blue LED + One Beep
    batteryWarningLevel = 2;
    setColour(0, 0, 255);
    buzzesLeft += 1;
	
  } else if (batteryPercent == 0 && batteryWarningLevel <= 0) {
    // Battery is OFF
    batteryWarningLevel = 1;
    setColour(0, 0, 0);  // LED OFF
  }
}
