/*

Kurt.v.k@gmail.com
mounted on ladyada perm breadboard .... ....... Digole display - arduino minion - receiving data from arduino funky 2

*/

// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

/*----------NOTE----------
new version of lib will save you compiled code size for the sample
The size for 2 version(@Arduino 1.0.1):
      UART  I2C    SPI
OLD   8998  8988   9132
NEW   6966  7566   6354
------------------------*/
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

void setup() {
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
    Serial.print(" %\t");
    mydisp.print("Hum.: "); 
    mydisp.print(h);
    mydisp.print(" %");
    
    Serial.print("Temperature: "); 
    Serial.print(t);
    
    mydisp.setPrintPos(0, 2);
    mydisp.print("Temp: "); 
    mydisp.print(t);
    mydisp.print(" C");
    mydisp.setPrintPos(6, 3);
    mydisp.print( 1.8 * t + 32);
    mydisp.print(" F");
    mydisp.setPrintPos(1, 4);
    //mydisp.print('U+00B0');
    //delay (1000);
    //mydisp.drawFrame(1,60,10 ,100);
  //delay(5000);

   }
}
