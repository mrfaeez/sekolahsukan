//FOR DHT
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

//FOR LED STRIPS


//FOR PULSE SENSOR




void setup() {
  Serial.begin(9600);

  dht.begin();

  
}

void loop() {

  //DHT
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  

  

}
