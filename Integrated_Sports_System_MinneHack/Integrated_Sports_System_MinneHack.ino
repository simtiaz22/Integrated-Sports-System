#include <math.h>

#include <Wire.h>      //include for acceleration
#include "MMA7660.h"
MMA7660 accelemeter;

const int Red = 3;     // acceleration variables
int injury = 0;

const int B = 4275;               // B value of the thermistor
const int R0 = 100000;            // R0 = 100k
const int pinTempSensor1 = A0;     // Grove - Temperature Sensor connect to A2
const int pinTempSensor2 = A2;
const int Blue = 4;
const int Green = 2;
const int Buzzer = 8;

//float sensorRoomTemp = 55.0;
//float normalTemp = 36.0;
//float warning = 37.5;
//float danger = 38.4;
//float offset = sensorRoomTemp - normalTemp;

void setup() {
  Serial.begin(9600);

  accelemeter.init();     //acceleration setup
  pinMode(Red, OUTPUT);

  pinMode(Blue, OUTPUT);    //temp setup
  pinMode(Green,OUTPUT);
  pinMode(Buzzer,OUTPUT);

  // put your setup code here, to run once:

}

void loop() {
    float sum1 = 0;
    float sum2 = 0;
    int i;
    for(i=0;i<2;i=i+1){
      int a = analogRead(pinTempSensor1);
      int b = analogRead(pinTempSensor2);
      float R = 1023.0/a-1.0;
      float P = 1023.0/b-1.0;
      P = R0*P;
      R = R0*R;
      float temperature1 = 1.0/(log(R/R0)/B+1/298.15)-273.15; // convert to temperature via datasheet
      float temperature2 = 1.0/(log(P/R0)/B+1/298.15)-273.15;
      sum1 = temperature1 + sum1;
      sum2 = temperature2 + sum2;

      delay(100);
    }


    float ax,ay,az;
    //accelemeter.getXYZ(&x,&y,&z);

    accelemeter.getAcceleration(&ax,&ay,&az);
    //Serial.println("accleration: ");
    float a_mag=(ax*ax)+(ay*ay)+(az*az);
    a_mag= sqrt(a_mag);
    if(a_mag > 2.0){
      injury = injury + 1;
      int i;
      for(i = 0; i < injury; i++)
      {
        digitalWrite(Red, HIGH);
        delay ((100*(pow(1.05,injury)))/injury);
        digitalWrite(Red, LOW);
        delay((100*(pow(1.05,injury)))/injury);
      }

    }

    //Serial.print(a_mag);
    //Serial.print("      ");
    //Serial.println(injury);



    float average1 = (sum1/2.0);  //average temp reading
    float average2 = (sum2/2.0);
    //Serial.print("Average Temperature1 = ");
    Serial.print(average1);
    Serial.print(";");
    //Serial.print("Average Temperature2 = ");
    Serial.print(average2);
    Serial.print(";");
    //Serial.print("Acceleration = ");
    Serial.print(a_mag);
    Serial.print(";");
    //Serial.print("Injuries = ");
    Serial.println(injury);
    Serial.println();


    if((average1 >= 60) || (average2 >= 60))//danger+offset)
    {
      digitalWrite(Blue, HIGH);   // turn the LED on (HIGH is the voltage level)
      tone(Buzzer, 1000);  //Send 1KHz sound signal...
    }

    else if((average1 >= 55) || (average2 >= 55)){ //warning + offset){
      noTone(Buzzer);
      digitalWrite(Blue, LOW);
      digitalWrite(Green, HIGH);   // turn the LED on (HIGH is the voltage level)
      tone(Buzzer,1000);
      delay(50);
      noTone(Buzzer);// wait for a second
      digitalWrite(Green, LOW);    // turn the LED off by making the voltage LOW
    }
    else
    {
      digitalWrite(Blue, LOW);
      digitalWrite(Green, LOW);
      noTone(Buzzer);
    }


  // put your main code here, to run repeatedly:

}
