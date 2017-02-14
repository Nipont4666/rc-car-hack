
/*
 RC Car via nRF24L01 module Arduino 
 Written by: Pason Tanpaiboon
 Febuary 2016
 Version.1.6
 
 This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
 To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/ 
 or send a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 
*/


#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

const int StrR = 3;
const int StrL = 4;
const int TrgF = 6;
const int TrgB= 7;

int StrRval = 0; 
int StrLval = 0; 
int TrgFval = 0; 
int TrgBval = 0; 

RF24 radio(9,10);

uint8_t data[4] ; 
const uint8_t buffer_size = sizeof(data);
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };//2 pipes communication

void setup()
{
  pinMode(StrR,INPUT);
  pinMode(StrL,INPUT);
  pinMode(TrgF,INPUT);
  pinMode(TrgB,INPUT);

  Serial.begin(115200);

  radio.begin();
  radio.setRetries(15,15);
  radio.openReadingPipe(1,pipes[1]);
  radio.startListening();
  radio.printDetails();
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  radio.stopListening();
}

void loop(void)
{
  ////////////////////Streering Right/////////////////////////////////////////////////////
  StrRval = digitalRead(StrR);//pin 3
  delay(15); 

  if (StrRval == HIGH){

    data[0] = 1;
  }
  else {

    data[0] = 0;
  } 
  ////////////////////Streering Left/////////////////////////////////////////////////////
  StrLval = digitalRead(StrL);//pin 4
  delay(15); 

  if (StrLval == HIGH){

    data[1] = 2;
  }
  else {

    data[1] = 0;
  }
  ////////////////////Trigger Forward/////////////////////////////////////////////////////
  TrgFval = digitalRead(TrgF);//pin 6
  delay(15); 

  if (TrgFval == HIGH){

    data[2] = 3;
  }
  else {

    data[2] = 0;
  }
  ////////////////////Trigger Backward/////////////////////////////////////////////////////
  TrgBval = digitalRead(TrgB);//pin 7
  delay(15); 

  if (TrgBval == HIGH){

    data[3] = 4;
  }
  else {

    data[3] = 0;
  }

  /////////////////////////////////////////////////////////////////////////

  radio.stopListening();

  bool ok = radio.write(  data ,sizeof(data) );
  delay(30);
  radio.startListening();
  delay(20);
  if (ok){
    Serial.print("data[0]=");
    Serial.print(data[0]);
    Serial.print(" data[1]=");
    Serial.print(data[1]);
    Serial.print(" data[2]=");
    Serial.print(data[2]);
    Serial.print(" data[3]=");
    Serial.println(data[3]);
  }
  else
    printf("failed\n\r");

}//void loop end

