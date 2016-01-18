/* Alarm functions using a M41T62 RTC connected via I2C and Wire lib
   Note: IRQ pin requires pull up resistor
   
   alarmRepeat() modes: (call to this function without a value returns
     the current mode)
        1: once per second
        2: once per minute
        3: once per hour
        4: once per day
        5: once per month
        6: once per year

   alarmEnable(): 1 is enabled, 0 is disabled

   checkFlags() returns 1 if alarm flag was set, 0 if not. Note, this ignores
     the watchdog and oscillator fail flags which could be the cause of an
     interrupt.
*/
#include <Wire.h>
#include "M41T62.h"

RTC_M41T62 rtc;

int irqPin = 3; // IRQ Interrupt pin to Arduino
int interrupted = 0; // Variable to notify if the alarm went off
DateTime now;

void setup () {
  Serial.begin(57600);
  Wire.begin();
  rtc.begin();
  rtc.checkFlags(); // clear any rtc interrupt flags

  // Setup rtc interrupt input pin
  pinMode(irqPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(irqPin), alarmIrq, FALLING);

  // following line sets the RTC to the date & time this sketch was compiled
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));

  rtc.alarmRepeat(6); // set alarm repeat mode (once per year)
  Serial.print("Repeat Mode Set: ");
  Serial.println(rtc.alarmRepeat()); // print the alarm mode we just set

  now = rtc.now(); // get current rtc time
  // set alarm for 90 seconds in the future
  DateTime alarmTime (now.unixtime()+90);
  rtc.alarmSet(alarmTime); 
  Serial.print("Alarm Set to: ");
  printTime(alarmTime);
  rtc.alarmEnable(1); // enable the alarm
}

void loop (){
  if(interrupted){
    now = rtc.now(); // get current time
    printTime(now);  // and print it
    if(rtc.checkFlags()){ // the alarm went off
      DateTime alarmTime (now.unixtime()+90); // set alarm for 90 seconds in the future
      rtc.alarmSet(alarmTime); // and save it to the rtc
      Serial.print("Alarm Tripped! Setting Alarm to: ");
      printTime(alarmTime);
      rtc.alarmEnable(1); // re-enable the alarm
    }else{ // the watchdog or oscillator fail tripped
      Serial.println("Interrupt Occured! Flag Register Cleared");
    }
    interrupted = 0; // reset the interrupt variable to 0
  }
  delay(1000);
}

void alarmIrq(){
  interrupted = 1;
}

void printTime(DateTime myTime){
  Serial.print(myTime.year(), DEC);
  Serial.print('/');
  Serial.print(myTime.month(), DEC);
  Serial.print('/');
  Serial.print(myTime.day(), DEC);
  Serial.print(' ');
  Serial.print(myTime.hour(), DEC);
  Serial.print(':');
  Serial.print(myTime.minute(), DEC);
  Serial.print(':');
  Serial.print(myTime.second(), DEC);
  Serial.println();
}
