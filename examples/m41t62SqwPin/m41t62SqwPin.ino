// SQW/OUT pin mode using a M41T62 RTC connected via I2C.
//
// This sketch reads the state of the pin, then iterates through the possible values at
// 5 second intervals.
//
// NOTE:
// You must connect a pull up resistor (~10kohm) from the SQW pin up to VCC.  Without
// this pull up the wave output will not work!

#include <Wire.h>
#include "M41T62.h"

RTC_M41T62 rtc;

int mode_index = 0;

M41T62SqwPinMode modes[] = {SqwNONE, Sqw1Hz, Sqw2Hz, Sqw4Hz, Sqw8Hz, Sqw16Hz, 
  Sqw32Hz, Sqw64Hz, Sqw128Hz, Sqw256Hz, Sqw512Hz, Sqw1kHz, Sqw2kHz, Sqw4kHz, 
  Sqw8kHz, Sqw32kHz};

void print_mode() {
  M41T62SqwPinMode mode = rtc.readSqwPinMode();
  
  Serial.print("Sqw Pin Mode: ");
  switch(mode) {
    case SqwNONE:  Serial.println("NONE");      break;
    case Sqw1Hz:   Serial.println("1Hz");       break;
    case Sqw2Hz:   Serial.println("2Hz");       break;
    case Sqw4Hz:   Serial.println("4Hz");       break;
    case Sqw8Hz:   Serial.println("8Hz");       break;
    case Sqw16Hz:  Serial.println("16Hz");      break;
    case Sqw32Hz:  Serial.println("32Hz");      break;
    case Sqw64Hz:  Serial.println("64Hz");      break;
    case Sqw128Hz: Serial.println("128Hz");     break;
    case Sqw256Hz: Serial.println("256Hz");     break;
    case Sqw512Hz: Serial.println("512Hz");     break;
    case Sqw1kHz:  Serial.println("1.024kHz");  break;
    case Sqw2kHz:  Serial.println("2.048kHz");  break;
    case Sqw4kHz:  Serial.println("4.096kHz");  break;
    case Sqw8kHz:  Serial.println("8.192kHz");  break;
    case Sqw32kHz: Serial.println("32.768kHz"); break;
    default:       Serial.println("UNKNOWN");   break;
  }
}

void setup () {
  Serial.begin(57600);
  Wire.begin();
  rtc.begin();
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set rtc time to sketch compile time
  print_mode();
}

void loop () {
  // Display the time/date
  DateTime now = rtc.now();
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  rtc.writeSqwPinMode(modes[mode_index++]); // Set Square Wave Mode
  print_mode(); // Print Square Wave Mode

  if (mode_index > 15) {
    mode_index = 0;
  }

  delay(5000);
}
