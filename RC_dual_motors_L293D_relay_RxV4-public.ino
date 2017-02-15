
/*
 RC Car via nRF24L01 module Arduino   
 Written by: Pason Tanpaiboon
 Febuary 2016
 Version.1.4
  
 This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
 To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/ 
 or send a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(9,10);

const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };//2 pipes communication
uint8_t received_data[4];
uint8_t num_received_data =sizeof(received_data);


////////////////////////////////////////////////////////////////////////////////////

//Define Pins
//Motor A
const int MotorA1 = 3;
const int MotorA2 = 4;

//Motor B
const int MotorB1 = 6;
const int MotorB2 = 7;

//LED Front,Rear light
const int ledFro = 5;//Front light
const int ledRea = 8;//Rear light

int servoMR=0;           
int servoML=0;
int driveMF=0;
int driveMB=0;

////////////////////////////////////////////////////////////////////////////////////
void setup(void)
{
  Serial.begin(115200);
  //configure pin modes
 
  pinMode (MotorA1, OUTPUT);
  pinMode (MotorA2, OUTPUT);  
 
  pinMode (MotorB1, OUTPUT);
  pinMode (MotorB2, OUTPUT);     

  pinMode (ledFro, OUTPUT);
  pinMode (ledRea, OUTPUT);

  radio.begin();
  radio.setRetries(15,15);
  radio.openReadingPipe(1,pipes[1]);
  radio.startListening();
  radio.printDetails();
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1,pipes[0]);
  radio.startListening();
}//void setup

void loop(void)
{

///////////////////////////////Radio reading///////////////////////////////////////////////////////     
  if ( radio.available() )
  {
    bool done = false;
    while (!done)
    {
      done = radio.read(&received_data, num_received_data  );
      delay(30);
    }// while (!done)
   
    Serial.print(received_data[0]);
    Serial.print ("---"); 
    Serial.print(received_data[1]);
    Serial.print ("---"); 

    Serial.print(received_data[2]);
    Serial.print ("---");
    Serial.println(received_data[3]);

    radio.stopListening();

    radio.startListening();

/*****************Steering motor Right*********************************************************************/
    servoMR = received_data[0];
    if (servoMR == 1 && servoML == 0) {//turn right

      digitalWrite (MotorA1,HIGH);
      digitalWrite (MotorA2,LOW);  
      delay(15);
    }

/*****************Steering motor Left*********************************************************************/
    servoML = received_data[1];
    if (servoML == 2 && servoMR == 0) {//turn left

      digitalWrite (MotorA1,LOW);
      digitalWrite (MotorA2,HIGH);  
      delay(15);
    }

    if (servoMR == 0 && servoML == 0) {//nutral

      digitalWrite (MotorA1,LOW);
      digitalWrite (MotorA2,LOW);
    }
 
/*****************Driving motor Forward*********************************************************************/
    driveMF = received_data[2];
    if (driveMF == 3 && driveMB == 0 ) {//forward

      digitalWrite (MotorB1,HIGH);
      digitalWrite (MotorB2,LOW);  
      delay(15);
      digitalWrite (ledFro,HIGH);//Turn Front light on
    }

/*****************Driving motor Backward*********************************************************************/
    driveMB = received_data[3];

    if (driveMB == 4  && driveMF == 0) {//backward

      digitalWrite (MotorB1,LOW);
      digitalWrite (MotorB2,HIGH);  
      delay(15);
      digitalWrite (ledRea,HIGH);//Turn Rear light on
    }
    if (driveMF == 0 && driveMB == 0 ) {//nutral
      digitalWrite (MotorB1,LOW);
      digitalWrite (MotorB2,LOW);  
      digitalWrite (ledFro,LOW);//Turn Front light off
      digitalWrite (ledRea,LOW);//Turn Rear light off
    }

  }//if ( radio.available() )

}// void loop


