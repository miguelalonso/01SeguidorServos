/* 
 * TimeSerialDateStrings.pde
 * example code illustrating Time library date strings
 *
 * This sketch adds date string functionality to TimeSerial sketch
 * Also shows how to handle different messages
 *
 * A message starting with a time header sets the time
 * A Processing example sketch to automatically send the messages is inclided in the download
 * On Linux, you can use "date +T%s\n > /dev/ttyACM0" (UTC time zone)
 *
 * A message starting with a format header sets the date format

 * send: Fs\n for short date format
 * send: Fl\n for long date format 
 */ 
 

#include <math.h>
#include <SunPos.h>
#include <Servo.h>
#include <Wire.h>
#include "RTClib.h"


Servo myservo_acimut;
Servo myservo_cenit;

float acimut_actual=0.0;
float cenit_actual=0.0;
float acimut_sol; //referido como 0 a mediodía, negativo al este(mañana)
float cenit_sol; //referido como 0 a mediodía, negativo al este(mañana)
float angulo_retorno=90.0; //angulo cenital para decir cuando vuelve a reposo
int noche=0;


 // definimos el lugar, sun y time de acuerdo con sunpos.h
 cLocation lugar;
 cSunCoordinates sun;
 cTime fecha_hora_PSA;

RTC_DS1307 rtc;

void setup()  {
  Serial.begin(9600);
  while (!Serial) ; // Needed for Leonardo only
 
   Serial.println("Starting!");
   
   myservo_acimut.attach(5);
   myservo_cenit.attach(6);
   
   lugar.dLongitude=-3.733;
   lugar.dLatitude=40.41;
   sun.dZenithAngle=0;
   sun.dAzimuth=0;
   #ifdef AVR
  Wire.begin();
#else
  Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
#endif
  rtc.begin();
//rtc.adjust(DateTime(2016, 6, 7, 9, 8, 0));
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
     //rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  
}

void loop(){    
  DateTime now = rtc.now();
  
    digitalClockDisplay();  
  
  
   // fecha de sunpos PSA
  fecha_hora_PSA.iYear=now.year();
  fecha_hora_PSA.iMonth=now.month();
  fecha_hora_PSA.iDay=now.day();
  fecha_hora_PSA.dHours=now.hour();
  fecha_hora_PSA.dMinutes=now.minute();
  fecha_hora_PSA.dSeconds=now.second();
  sunpos(fecha_hora_PSA, lugar, &sun);
  acimut_sol=sun.dAzimuth-180;
  cenit_sol=sun.dZenithAngle;
  
  //
  
   if(cenit_sol>angulo_retorno){ // de noche a reposo
    cenit_sol=0; //oner cero o ángulos de reposo
    acimut_sol=0;
    noche=1;
  }  else {noche=0;}
    
  Seguir_sol();
  
  delay(1000);
}

void digitalClockDisplay() {
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
    
   /* Serial.print(" since midnight 1/1/1970 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");
    
    // calculate a date which is 7 days and 30 seconds into the future
    DateTime future (now.unixtime() + 7 * 86400L + 30);
    
    Serial.print(" now + 7d + 30s: ");
    Serial.print(future.year(), DEC);
    Serial.print('/');
    Serial.print(future.month(), DEC);
    Serial.print('/');
    Serial.print(future.day(), DEC);
    Serial.print(' ');
    Serial.print(future.hour(), DEC);
    Serial.print(':');
    Serial.print(future.minute(), DEC);
    Serial.print(':');
    Serial.print(future.second(), DEC);
    Serial.println();
    
    Serial.println();
  */
  Serial.print(" Acimut_sol:  ");
  Serial.print(acimut_sol);
  Serial.print(" Cenit_sol:  ");
  Serial.print(cenit_sol);
  Serial.println(); 
}

void printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}




 void Seguir_sol(){

  // myservo_acimut.write(acimut_sol);
  // myservo_acimut.write(cenit_sol);
   myservo_acimut.write(acimut_sol+90);
   delay(500);
   myservo_cenit.write(150-cenit_sol);

  myservo_acimut.write(acimut_sol+90);
   delay(500);
  myservo_cenit.write(150-cenit_sol);
   
 }
   
