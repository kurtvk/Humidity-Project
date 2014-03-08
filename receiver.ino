/*

Kurt.v.k@gmail.com
mounted on ladyada perm breadboard .... ....... Digole display - arduino minion - receiving data from arduino funky 2

*/

// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#define _Digole_Serial_UART_  //To tell compiler compile the special communication only, 
//other available is: _Digole_Serial_I2C_ and _Digole_Serial_SPI_
#include <DigoleSerial.h>
//--------UART setup, if you don't use UART, use // to comment following line
DigoleSerialDisp mydisp(&Serial1, 9600); //UART:Arduino Leonardo: Pin 1(TX)on arduino to RX on module
//--------I2C setup, if you don't use I2C, use // to comment following 2 lines
//#include <Wire.h>
//DigoleSerialDisp mydisp(&Wire,'\x27');  //I2C:Arduino UNO: SDA (data line) is on analog input pin 4, and SCL (clock line) is on analog input pin 5 on UNO and Duemilanove
//--------SPI setup, if you don't use SPI, use // to comment following line
//DigoleSerialDisp mydisp(8,9,10);  //SPI:Pin 8: data, 9:clock, 10: SS, you can assign 255 to SS, and hard ground SS pin on module
#define LCDCol 16
#define LCDRow 2

#include "DHT.h"
#include <RF12.h>  //from jeelabs.org
#include <Ports.h> 

#define DHTPIN 2     // what pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11 
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht(DHTPIN, DHTTYPE);

const byte network  = 100;   // network group (can be in the range 1-255).
const byte myNodeID = 2;     // unique node ID of receiver (1 through 30) 1-send 2-receive

//Frequency of RF12B can be RF12_433MHZ, RF12_868MHZ or RF12_915MHZ.
const byte freq = RF12_433MHZ; // Match freq to module

const byte RF12_NORMAL_SENDWAIT = 0;

typedef struct { int temp1, hum1, acc1, acc2; } PayloadTX;      // create structure - a neat way of packaging data for RF comms
PayloadTX emontx;  

const int emonTx_NodeID=1;            //emonTx node ID


void setup() 
{
  
  Serial1.begin(9600); //start Serial1
  Serial1.println("DHTxx test!");
 
  dht.begin();
  
  mydisp.begin();
    /*----------for text LCD adapter and graphic LCD adapter ------------*/
    mydisp.clearScreen(); //CLear screen
    //mydisp.displayConfig(1);    //set config display ON, 0=off
    //mydisp.setI2CAddress(0x29);  //this function only working when you connect using I2C, from 1 to 127
    //delay(1000);
    //mydisp.setLCDColRow(16,2);  //set LCD Col and Row, only time set up is OK
    mydisp.disableCursor(); //disable cursor, enable cursore use: enableCursor();
    
   rf12_initialize(myNodeID,freq,network);   //Initialize RFM12 with settings defined above  

  
}

void loop() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  delay(5000);
  mydisp.clearScreen(); //CLear screen
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT");
  } else {
    Serial.print("Humidity: "); 
    Serial.print(h);
    Serial.println(" %\t");
    mydisp.print("Hum.: "); 
    mydisp.print(h);
    mydisp.println(" %");
    
    Serial.print("Temperature: "); 
    Serial.println(t);
    
    mydisp.setPrintPos(0, 1);
    mydisp.print("Temp: "); 
    //mydisp.print(t);
    //mydisp.print(" C");
    mydisp.setPrintPos(6, 1);
    mydisp.print( 1.8 * t + 32);
    mydisp.print(" F");
    mydisp.setPrintPos(1, 4);
    
    mydisp.setPrintPos(0, 2);
    mydisp.print("Remote");
    
    mydisp.setPrintPos(0, 3);
    mydisp.print(emontx.hum1 /100);
    mydisp.print(" %");
    
    
    mydisp.setPrintPos(0, 4);
    mydisp.print(1.8 * emontx.temp1/100 + 32);
    mydisp.print(" F");

    
    
    {
  
 //starts here
 
 //reset temp and hum to 9999 to detect bogus info
 //emontx.temp1 = 9999;
 //emontx.hum1 = 9999;
 
 if (rf12_recvDone()){    
  if (rf12_crc == 0 && (rf12_hdr & RF12_HDR_CTL) == 0) {
    
    int node_id = (rf12_hdr & 0x1F);		  //extract nodeID from payload
        
    if (node_id == emonTx_NodeID)  {             //check data is coming from node with the corrct ID
        emontx=*(PayloadTX*) rf12_data;            // Extract the data from the payload 
       Serial.print("temp1: "); Serial.println(emontx.temp1); 
       Serial.print("hum1: "); Serial.println(emontx.hum1); 
       Serial.print("acc1: "); Serial.println(emontx.acc1); 
       Serial.print("acc2: "); Serial.println(emontx.acc2); 
       Serial.println("  "); 
  }
 }
}
}
//ends here
  

   }
}
