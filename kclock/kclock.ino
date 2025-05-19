#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 Clock;

int hour1[] = {43,42,48,49,51,44,45};
int hour2[] = {38,50,46,47,41,39,40};
int minute1[] = {29,32,35,28,33,36,30};
int minute2[] = {25,23,27,34,37,24,22};
int d1[] = {0,0,0,0,0,0,0};
int d2[] = {0,0,0,0,0,0,0};
int dots[] = {26,31};
int digitPinCount = 7;
int button1 = 0;
int button2 = 0;
int button3 = 0;
int dotsStatus = 0;
int processTimeCounter = -1;
int processDotsCounter = 0;
int hourMode = 1; //           1 = 12 HOUR MODE         2 = 24 HOUR MODE
DateTime MyDateAndTime;

void setup() {
  Serial.begin(9600);
  Clock.begin();
  for (int pin = 22; pin < 52; pin++) {
    pinMode(pin, OUTPUT);
  }
  pinMode(11, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  digitalWrite(11,HIGH);
  digitalWrite(12,HIGH);
  digitalWrite(13,HIGH);

  MyDateAndTime = Clock.now();
    
  parseDigit("hour", getFirstNumber(MyDateAndTime.Hour), getSecondNumber(MyDateAndTime.Hour));
  parseDigit("minute", getFirstNumber(MyDateAndTime.Minute), getSecondNumber(MyDateAndTime.Minute));
}

void loop() {
  button1 = digitalRead(11);
  if (button1 == LOW) {
    //Serial.println("button1");
    addHour();
    delay(500);
  }

  button2 = digitalRead(12);
  if (button2 == LOW) {
    //Serial.println("button2");
    addMinute();
    delay(500);
  }

  button3 = digitalRead(13);
  if (button3 == LOW) {
    switch_12_24_Hour_Mode();
    delay(500);
  }
  delay(10);
  blinkDots();
  if (processTimeCounter == 5000 ||
      processTimeCounter == -1) {
    processTimeCounter = 0;
    updateClock();
  } else {
    processTimeCounter++;
    return;
  }
}

void updateClock() {
  MyDateAndTime = Clock.now();
  parseDigit("hour", getFirstNumber(getHourFromMode()), getSecondNumber(getHourFromMode()));
  parseDigit("minute", getFirstNumber(MyDateAndTime.Minute), getSecondNumber(MyDateAndTime.Minute));
}

void addHour() {
  if (MyDateAndTime.Hour != 23) {
    MyDateAndTime.Hour++;
  } else {
    MyDateAndTime.Hour = 0;
  }
  MyDateAndTime.Second = 0;
  Clock.write(MyDateAndTime);
  updateClock();
}

void addMinute() {
  if (MyDateAndTime.Minute != 59) {
    MyDateAndTime.Minute++;
  } else {
    MyDateAndTime.Minute = 0;
  }
  MyDateAndTime.Second = 0;
  Clock.adjust(MyDateAndTime);
  updateClock();
}

void switch_12_24_Hour_Mode() {
  if (hourMode == 1) {
    hourMode = 2;
  } else {
    hourMode = 1;
  }
  updateClock();
}

int getHourFromMode() {
  //12 HOUR MODE - 12 1 2 3 4 5 6 7 8 9 10 11 12  1  2  3  4  5  6  7  8  9 10 11
  //24 HOUR MODE -  0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
  if (hourMode == 1) {//12 HOUR MODE
    switch(MyDateAndTime.Hour) {//ONLY THE HOURS THAT ARE DIFFERENT FROM THE 24 HOUR MODE
      case 0  : return 12; break;
      case 13 : return 1;  break;
      case 14 : return 2;  break;
      case 15 : return 3;  break;
      case 16 : return 4;  break;
      case 17 : return 5;  break;
      case 18 : return 6;  break;
      case 19 : return 7;  break;
      case 20 : return 8;  break;
      case 21 : return 9;  break;
      case 22 : return 10; break;
      case 23 : return 11; break;
      default : return MyDateAndTime.Hour;
    }
  } else {//24 HOUR MODE
    return MyDateAndTime.Hour;
  }
}

void blinkDots() {
  if (processDotsCounter == 100) {
    processDotsCounter = 0;
    if (dotsStatus == 1) {
      dotsStatus = 0;
      digitalWrite(dots[0],HIGH);
      digitalWrite(dots[1],HIGH);
    } else {
      dotsStatus = 1;
      digitalWrite(dots[0],LOW);
      digitalWrite(dots[1],LOW);
    }
  } else {
    processDotsCounter++;
    return;
  }
}

int getFirstNumber(int inputNumber) {
  if (inputNumber < 10) { return 0; }
  String output = String(inputNumber);
  return output.substring(0,1).toInt();
}

int getSecondNumber(int inputNumber) {
  if (inputNumber < 10) { return inputNumber; }
  String output = String(inputNumber);
  return output.substring(1,2).toInt();
}

void parseDigit(String targetArray, int firstNumber, int secondNumber) {
  if (targetArray == "hour") {
    for (int pin = 0; pin <= 7; pin++) {
      d1[pin] = hour1[pin];
      d2[pin] = hour2[pin];
    }
  } else {
    for (int pin = 0; pin <= 7; pin++) {
      d1[pin] = minute1[pin];
      d2[pin] = minute2[pin];
    }
  }
  processDigit(targetArray, firstNumber, secondNumber);
}

void processDigit(String target, int firstNumber, int secondNumber) {
  if (target == "hour" && firstNumber == 0) {
    buildDigit(-1, d1[0], d1[1], d1[2], d1[3], d1[4], d1[5], d1[6]);
  } else {
    buildDigit(firstNumber, d1[0], d1[1], d1[2], d1[3], d1[4], d1[5], d1[6]);
  }
  buildDigit(secondNumber, d2[0], d2[1], d2[2], d2[3], d2[4], d2[5], d2[6]);
}

void buildDigit(int number, int pin1, int pin2, int pin3, int pin4, int pin5, int pin6, int pin7) {
  switch(number) {
    case -1 :
      digitalWrite(pin1,LOW);digitalWrite(pin2,LOW);digitalWrite(pin3,LOW);digitalWrite(pin4,LOW);digitalWrite(pin5,LOW);digitalWrite(pin6,LOW);digitalWrite(pin7,LOW);
      break;
    case 0 :
      digitalWrite(pin1,HIGH);digitalWrite(pin2,HIGH);digitalWrite(pin3,HIGH);digitalWrite(pin4,HIGH);digitalWrite(pin5,HIGH);digitalWrite(pin6,HIGH);digitalWrite(pin7,LOW);
      break;
    case 1 :
      digitalWrite(pin1,LOW);digitalWrite(pin2,HIGH);digitalWrite(pin3,HIGH);digitalWrite(pin4,LOW);digitalWrite(pin5,LOW);digitalWrite(pin6,LOW);digitalWrite(pin7,LOW);
      break;
    case 2 :
      digitalWrite(pin1,HIGH);digitalWrite(pin2,HIGH);digitalWrite(pin3,LOW);digitalWrite(pin4,HIGH);digitalWrite(pin5,HIGH);digitalWrite(pin6,LOW);digitalWrite(pin7,HIGH);
      break;
    case 3 :
      digitalWrite(pin1,HIGH);digitalWrite(pin2,HIGH);digitalWrite(pin3,HIGH);digitalWrite(pin4,HIGH);digitalWrite(pin5,LOW);digitalWrite(pin6,LOW);digitalWrite(pin7,HIGH);
      break;
    case 4 :
      digitalWrite(pin1,LOW);digitalWrite(pin2,HIGH);digitalWrite(pin3,HIGH);digitalWrite(pin4,LOW);digitalWrite(pin5,LOW);digitalWrite(pin6,HIGH);digitalWrite(pin7,HIGH);
      break;
    case 5 :
      digitalWrite(pin1,HIGH);digitalWrite(pin2,LOW);digitalWrite(pin3,HIGH);digitalWrite(pin4,HIGH);digitalWrite(pin5,LOW);digitalWrite(pin6,HIGH);digitalWrite(pin7,HIGH);
      break;
    case 6 :
      digitalWrite(pin1,HIGH);digitalWrite(pin2,LOW);digitalWrite(pin3,HIGH);digitalWrite(pin4,HIGH);digitalWrite(pin5,HIGH);digitalWrite(pin6,HIGH);digitalWrite(pin7,HIGH);
      break;
    case 7 :
      digitalWrite(pin1,HIGH);digitalWrite(pin2,HIGH);digitalWrite(pin3,HIGH);digitalWrite(pin4,LOW);digitalWrite(pin5,LOW);digitalWrite(pin6,LOW);digitalWrite(pin7,LOW);
      break;
    case 8 :
      digitalWrite(pin1,HIGH);digitalWrite(pin2,HIGH);digitalWrite(pin3,HIGH);digitalWrite(pin4,HIGH);digitalWrite(pin5,HIGH);digitalWrite(pin6,HIGH);digitalWrite(pin7,HIGH);
      break;
    case 9 :
      digitalWrite(pin1,HIGH);digitalWrite(pin2,HIGH);digitalWrite(pin3,HIGH);digitalWrite(pin4,LOW);digitalWrite(pin5,LOW);digitalWrite(pin6,HIGH);digitalWrite(pin7,HIGH);
      break;
  }
}
