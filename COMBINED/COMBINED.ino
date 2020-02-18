//FOR DHT
#include "DHT.h"
#define DHTPIN 8
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);


//FOR LED STRIPS
#include <Adafruit_NeoPixel.h>
#define PIN 6
#define NUMPIXELS 16
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500
bool memoryled = true;


//FOR PULSE SENSOR
#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>
const int PulseWire = 0;
const int LED13 = 13;
int Threshold = 550;

PulseSensorPlayground pulseSensor;

void colorwipe(int x, int y, int z) {
  pixels.clear();
  pixels.setPixelColor(0, pixels.Color(x, y, z));
//  pixels.setPixelColor(1, pixels.Color(x, y, z));
  pixels.setPixelColor(2, pixels.Color(x, y, z));
//  pixels.setPixelColor(3, pixels.Color(x, y, z));
  pixels.setPixelColor(4, pixels.Color(x, y, z));
//  pixels.setPixelColor(5, pixels.Color(x, y, z));
  pixels.setPixelColor(6, pixels.Color(x, y, z));
//  pixels.setPixelColor(7, pixels.Color(x, y, z));
  pixels.setPixelColor(8, pixels.Color(x, y, z));
//  pixels.setPixelColor(9, pixels.Color(x, y, z));
  pixels.show();
  delay(2000);
}

void setup() {
  Serial.begin(9600);

  dht.begin();
  pixels.begin();

  pulseSensor.analogInput(PulseWire);
  pulseSensor.blinkOnPulse(LED13);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);
  pulseSensor.begin();


}

void loop() {

  //DHT
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  //START
  if (memoryled) {
    pixels.clear();
    pixels.show();
    delay(2000);
    for (int i = 0; i < NUMPIXELS && memoryled; i += 2) {

      pixels.setPixelColor(i, pixels.Color(0, 150, 0));

      pixels.show();   // Send the updated pixel colors to the hardware.

      delay(DELAYVAL); // Pause before next pass through loop

      if (i >= 8)
        memoryled = false;
    }
  }




  //BPM
  int myBPM = pulseSensor.getBeatsPerMinute();

  
    Serial.print("BPM: ");
    Serial.println(myBPM);

     int age = /*param.asInt()*/;
     int maxBPM = 220-age;
     int idealBPMHigh = int(85*maxBPM/100);
     int idealBPMLow = int(70*maxBPM/100);
     int highTemp = 38;
     int lowTemp = 35;
    
    if(myBPM > idealBPMHigh || t >= highTemp){
      //red
      colorwipe(50, 0, 0);
    }else if(myBPM >= idealBPMLow && myBPM <=idealBPMHigh || t > lowTemp && t< highTemp){
      //yellow
      colorwipe(50, 50, 0);
    }else {
      //green
      colorwipe(0, 50, 0); 
    }  
  

  delay(20);


}
