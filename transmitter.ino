//SimpleSend
//RFM12B wireless demo - transmitter - no ack
#include <DHT22.h>
#include <stdio.h> // Only used for sprintf
#include <RF12.h>  //from jeelabs.org
#include <Ports.h> 

// Connect a 4.7K resistor between VCC and the data pin (strong pullup)
#define DHT22_PIN 2

// Setup a DHT22 instance
DHT22 myDHT22(DHT22_PIN);
// RF12B constants:

const byte network  = 100;   // network group (can be in the range 1-255).
const byte myNodeID = 1;     // unique node ID of receiver (1 through 30) 1-send 2-receive

//Frequency of RF12B can be RF12_433MHZ, RF12_868MHZ or RF12_915MHZ.
const byte freq = RF12_433MHZ; // Match freq to module

const byte RF12_NORMAL_SENDWAIT = 0;

typedef struct { int temp1, hum1, acc1, acc2; } PayloadTX;      // create structure - a neat way of packaging data for RF comms
PayloadTX emontx;  

void setup(void)
{
   rf12_initialize(myNodeID, freq, network);   // Initialize RFM12   
   // start serial port
  Serial.begin(9600);
  Serial.println("DHT22 Library Demo");
  pinMode(8, OUTPUT);      // sets the digital pin as output for DHT22

  
}

void loop(void)
{ 
  digitalWrite(8, HIGH);
  
  DHT22_ERROR_t errorCode;
  
  // The sensor can only be read from every 1-2s, and requires a minimum
  // 2s warm-up after power-on.
  delay(10000);
  
  Serial.print("Requesting data...");
  errorCode = myDHT22.readData();
  switch(errorCode)
  {
    case DHT_ERROR_NONE:
      Serial.print("Got Data ");
      Serial.print(myDHT22.getTemperatureC());
      Serial.print("C ");
      Serial.print(myDHT22.getHumidity());
      Serial.println("%");
      // Alternately, with integer formatting which is clumsier but more compact to store and
	  // can be compared reliably for equality:
	  //	  
      char buf[128];
      sprintf(buf, "Integer-only reading: Temperature %hi.%01hi C, Humidity %i.%01i %% RH",
                   myDHT22.getTemperatureCInt()/10, abs(myDHT22.getTemperatureCInt()%10),
                   myDHT22.getHumidityInt()/10, myDHT22.getHumidityInt()%10);
      Serial.println(buf);
      break;
    case DHT_ERROR_CHECKSUM:
      Serial.print("check sum error ");
      Serial.print(myDHT22.getTemperatureC());
      Serial.print("C ");
      Serial.print(myDHT22.getHumidity());
      Serial.println("%");
      break;
    case DHT_BUS_HUNG:
      Serial.println("BUS Hung ");
      break;
    case DHT_ERROR_NOT_PRESENT:
      Serial.println("Not Present ");
      break;
    case DHT_ERROR_ACK_TOO_LONG:
      Serial.println("ACK time out ");
      break;
    case DHT_ERROR_SYNC_TIMEOUT:
      Serial.println("Sync Timeout ");
      break;
    case DHT_ERROR_DATA_TIMEOUT:
      Serial.println("Data Timeout ");
      break;
    case DHT_ERROR_TOOQUICK:
      Serial.println("Polled to quick ");
      break;
  }



emontx.temp1 = (myDHT22.getTemperatureC()*100);
emontx.hum1 = (myDHT22.getHumidity()*100);
emontx.acc1 = 3;
emontx.acc2 = 4;

int i = 0; while (!rf12_canSend() && i<10) {rf12_recvDone(); i++;}
    rf12_sendStart(0, &emontx, sizeof emontx);
    
Serial.println("Data sent ....!!!");


}


/* ADD NECCESARY STUFF IN RTO TRANSMITT TEMP AND HUMIDITY

const int payloadCount = 6; // the number of integers in the payload message
int payload[payloadCount];



void loop()
{
  for( int i= 0; i < payloadCount; i++)
  {
    payload[i] = analogRead(i);   
  }
  while (!rf12_canSend())  // is the driver ready to send?
    rf12_recvDone();       // no, so service the driver

  rf12_sendStart(rf12_hdr, payload, payloadCount*sizeof(int)); 
  rf12_sendWait(RF12_NORMAL_SENDWAIT); // wait for send completion

  delay(1000);  // send every second
}
*/
