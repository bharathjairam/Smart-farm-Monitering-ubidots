#include <ESP8266WiFi.h>
#include <Wire.h>
#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11

#define dht_dpin 0
#include "Ubidots.h"

DHT dht(dht_dpin, DHTTYPE);

int sensor_pin = A0; // Soil Sensor input at Analog PIN A0
int output_value ;
int moisture;

int Status = 12;
int sensor = D7;

//WiFiClient client;

const char* UBIDOTS_TOKEN = "BBFF-B6TUZGgPB5oegzPAwjTvouXbR3dMnJ"; 
const char* ssid     = "MAJOR PROJECT"; // Your ssid
const char* password = "12345678"; // Your Password
Ubidots ubidots(UBIDOTS_TOKEN, UBI_HTTP);

void setup() {
  Serial.begin(115200);
  delay(100);

  dht.begin();
  Serial.println("Humidity and temperature\n\n");
  delay(700);

  pinMode(D1,OUTPUT);
  pinMode(sensor, INPUT); // declare sensor as input
  pinMode(Status, OUTPUT);  // declare LED as output
  
  
  
ubidots.wifiConnect(ssid, password);
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, password);

      while (WiFi.status() != WL_CONNECTED) {
         delay(500);
         Serial.print(".");
         }
         
Serial.println("");
Serial.println("WiFi is connected");

}


void loop() { 

    moisture = irrigation_system();
    Serial.println();
    Serial.print("Mositure : ");
    Serial.print(output_value);
    Serial.println("%");


    long state = digitalRead(sensor);
    delay(1000);
    if(state == HIGH){
      digitalWrite (Status, HIGH);
      Serial.println("Motion detected!");
    }
    else {
      digitalWrite (Status, LOW);
      Serial.println("Motion absent!");
      }

  
    float h = dht.readHumidity();
    float t = dht.readTemperature();         
    Serial.print("Current humidity = ");
    Serial.print(h);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(t); 
    Serial.println("C  ");
    delay(800);
  
  ubidots.add("Temperature", t);// Change for your variable name  
  ubidots.add("Current humidity",h);
  ubidots.add("Mositure",output_value);
  ubidots.add("Motion",state);
  ubidots.add("Motor",output_value);
  
  
  bool bufferSent = false;
  bufferSent = ubidots.send(); // Will send data to a device label that matches the device Id
  if (bufferSent) {
   Serial.println("Values sent by the device");
  }
  delay(3000);
}

int irrigation_system(){
   output_value= analogRead(sensor_pin);
 output_value = map(output_value,550,10,0,100);
 
   if(output_value<0){
      digitalWrite(D1,HIGH);
     }
     else{
            digitalWrite(D1,LOW);
     }
   delay(1000);
   return output_value;
   
   }
