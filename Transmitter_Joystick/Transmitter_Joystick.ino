// 2 Servo and one BLDC Base F22 Receiver

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


#define CE_PIN   9
#define CSN_PIN 10

const byte slaveAddress[5] = {'R','x','A','B','D'};


RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

struct Data_to_be_sent {
  byte ch1;
  byte ch2;
  byte ch3;
  byte ch4;
  byte ch5;
  byte ch6;
  byte ch7;
};

Data_to_be_sent sent_data;


void setup() {

    Serial.begin(9600);

    Serial.println("SimpleTx Starting");

    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.setRetries(3,5); // delay, count
    radio.openWritingPipe(slaveAddress);

    sent_data.ch1 = 127;
    sent_data.ch2 = 127;
    sent_data.ch3 = 127;
    sent_data.ch4 = 127;
    sent_data.ch5 = 0;
    sent_data.ch6 = 0;
    sent_data.ch7 = 0;                                
}

//====================

void loop() {
    sent_data.ch1 = map( analogRead(A0), 1024, 0, 0, 255);
    sent_data.ch2 = map( analogRead(A1), 0, 1024, 0, 255);
    sent_data.ch3 = map( analogRead(A2), 0, 1024, 0, 255);
    sent_data.ch4 = map( analogRead(A3), 0, 1024, 0, 255);
    sent_data.ch5 = digitalRead(2);
    sent_data.ch6 = digitalRead(3);
    sent_data.ch7 = map( analogRead(A4), 0, 1024, 0, 255);
    //Serial.println(digitalRead(2));
    radio.write(&sent_data, sizeof(Data_to_be_sent));
}