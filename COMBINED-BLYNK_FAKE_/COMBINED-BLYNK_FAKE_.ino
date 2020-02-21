//FOR BLYNk
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
BlynkTimer timer;

char auth[] = "jTh4MI3JSkkOuq4W-1Lmo6mNkIqXP8ub";
char ssid[] = "RamadhanTiba";
char pass[] = "abc12345";

int ageSelect;

BLYNK_WRITE(V6) {
  switch (param.asInt())
  {
    case 1: // Item 1
      ageSelect = 1;
      break;
    case 2: // Item 2
      ageSelect = 2;
      break;
    default:
      ageSelect = 0;
  }
}

//FOR DHT
#include "DHT.h"
#define DHTPIN D6
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);


//FOR LED STRIPS
#include <Adafruit_NeoPixel.h>
#define PIN D7
#define NUMPIXELS 16
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500
bool memoryled = true;

int age;
int myBPM = 55;

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.print("Age : ");
  Serial.println(age);
  Serial.print("Current humidity = ");
  Serial.print(h);
  Serial.print("%  ");
  Serial.print("temperature = ");
  Serial.println(t);
  Serial.print("BPM : ");
  Serial.println(myBPM);
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);
  Blynk.virtualWrite(V3, myBPM);

}

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
  
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, sendSensor);

}


void loop() {

  //DHT
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  //START
  if (memoryled) {
    pixels.clear();
    pixels.show();
    delay(1000);

    for (int i = 0; i < NUMPIXELS && memoryled; i += 2) {

      pixels.setPixelColor(i, pixels.Color(150, 150, 150));

      pixels.show();   // Send the updated pixel colors to the hardware.

      delay(DELAYVAL); // Pause before next pass through loop

      if (i >= 8)
        memoryled = false;
    }

  }

  if (ageSelect == 1)
    age = 18;
  else if (ageSelect == 2)
    age = 26;
  else
    age = 30;

  int highTemp = 38;
  int lowTemp = 35;

  if (myBPM > 160)
    myBPM = 55;

  if (myBPM > 100 && myBPM < 130)
    colorwipe(150, 8, 67);

  else if (myBPM >= 130)
    colorwipe(50, 0, 0);

  else if (myBPM > 100 && t >= highTemp) {
    //red
    colorwipe(50, 0, 0);
  } 
  
  else if (myBPM >= 55 && myBPM <= 80 || t > lowTemp && t < highTemp) {
    //yellow
    colorwipe(50, 50, 0);
  } 
  
  else {
    //green
    colorwipe(0, 50, 0);
  }

  delay(20);

  Blynk.run();
  timer.run();

  myBPM += 5;


}
