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

    if (myBPM > 70) {
      pixels.clear();
      pixels.setPixelColor(1, pixels.Color(150, 0, 0));
      pixels.setPixelColor(3, pixels.Color(150, 0, 0));
      pixels.show();
      delay(2000);
    }

    else {
      pixels.clear();
      pixels.setPixelColor(1, pixels.Color(0, 0, 150));
      pixels.setPixelColor(3, pixels.Color(0, 0, 150));
      
      pixels.show();
      delay(2000);

    }
  

  delay(20);


}
