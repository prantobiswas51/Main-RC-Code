// 2 Servo and one BLDC Base F22 Receiver

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

#define CE_PIN   9
#define CSN_PIN 10

const byte thisSlaveAddress[5] = {'R','x','A','B','D'};

RF24 radio(CE_PIN, CSN_PIN);

struct Received_data {
  byte ch1;
  byte ch2;
  byte ch3;
  byte ch4;
};

Received_data received_data;

Servo channel_1;
Servo channel_2;
Servo channel_3;
Servo channel_4;

int ch1_value = 0;
int ch2_value = 0;
int ch3_value = 0;
int ch4_value = 0;

void reset_the_Data() 
{
  received_data.ch1 = 128;     
  received_data.ch2 = 128;
  received_data.ch3 = 129;
  received_data.ch4 = 0;
}


void setup()
{
  //Attach the servo signal on pins from D2 to D8
  channel_1.attach(2);
  channel_2.attach(3);
  channel_3.attach(4);
  channel_4.attach(5);
  
  //We reset the received values
  reset_the_Data();

  //Once again, begin and radio configuration
    Serial.begin(9600);

    Serial.println("SimpleRx Starting");
    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.openReadingPipe(1, thisSlaveAddress);
    radio.startListening();
  
  //We start the radio comunication
    radio.startListening();
}

/**************************************************/

unsigned long lastRecvTime = 0;

//We create the function that will read the data each certain time
void receive_the_data()
{
  while ( radio.available() ) {
  radio.read(&received_data, sizeof(Received_data));
  lastRecvTime = millis(); //Here we receive the data
}
}

/**************************************************/

void loop()
{
  //Receive the radio data
  receive_the_data();

  unsigned long now = millis();
  if ( now - lastRecvTime > 1000 ) {
    // signal lost?
    reset_the_Data();
  } 

  channel_1.writeMicroseconds(ch1_value);  
  channel_2.writeMicroseconds(ch2_value);  
  channel_3.writeMicroseconds(ch3_value);  
  channel_4.writeMicroseconds(ch4_value);
  Serial.print("CH1: - ");
  Serial.print(received_data.ch1);
  Serial.print("  CH2: - ");
  Serial.print(received_data.ch2);
  Serial.print("  CH3: - ");
  Serial.print(received_data.ch3);
  Serial.print("  CH4: - ");
  Serial.println(received_data.ch4);
  
  ch1_value = map(received_data.ch1,0,255,500,2500);
  ch2_value = map(received_data.ch2,0,255,500,2500);
  ch3_value = map(received_data.ch3,0,255,500,2500);
  ch4_value = map(received_data.ch4,0,255,0,2500);


  //Create the PWM signals
  channel_1.writeMicroseconds(ch1_value);  
  channel_2.writeMicroseconds(ch2_value);  
  channel_3.writeMicroseconds(ch3_value);  
  channel_4.writeMicroseconds(ch4_value);
  Serial.print("CH1: - ");
  Serial.print(received_data.ch1);
  Serial.print("  CH2: - ");
  Serial.print(received_data.ch2);
  Serial.print("  CH3: - ");
  Serial.print(received_data.ch3);
  Serial.print("  CH4: - ");
  Serial.println(received_data.ch4);

  
}//Loop end