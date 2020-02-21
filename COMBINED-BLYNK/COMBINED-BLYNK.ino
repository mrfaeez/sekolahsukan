//FOR BLYNk
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
BlynkTimer timer;

char auth[] = "1JAdGp1n4nKZLAPbDivMSMRqvemy68VX";
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


//FOR PULSE SENSOR
#define pulsePin A0
int rate[10];
unsigned long sampleCounter = 0;
unsigned long lastBeatTime = 0;
unsigned long lastTime = 0, N;
int myBPM = 0;
int IBI = 0;
int P = 512;
int T = 512;
int thresh = 512;
int amp = 100;
int Signal;
boolean Pulse = false;
boolean firstBeat = true;
boolean secondBeat = true;
boolean QS = false;

int age;

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

void readPulse() {

  Signal = analogRead(pulsePin);
  sampleCounter += 2;
  int N = sampleCounter - lastBeatTime;

  detectSetHighLow();

  if (N > 250) {
    if ( (Signal > thresh) && (Pulse == false) && (N > (IBI / 5) * 3) )
      pulseDetected();
  }

  if (Signal < thresh && Pulse == true) {
    Pulse = false;
    amp = P - T;
    thresh = amp / 2 + T;
    P = thresh;
    T = thresh;
  }

  if (N > 2500) {
    thresh = 512;
    P = 512;
    T = 512;
    lastBeatTime = sampleCounter;
    firstBeat = true;
    secondBeat = true;
  }

}

void detectSetHighLow() {

  if (Signal < thresh && N > (IBI / 5) * 3) {
    if (Signal < T) {
      T = Signal;
    }
  }

  if (Signal > thresh && Signal > P) {
    P = Signal;
  }

}

void pulseDetected() {
  Pulse = true;
  IBI = sampleCounter - lastBeatTime;
  lastBeatTime = sampleCounter;

  if (firstBeat) {
    firstBeat = false;
    return;
  }
  if (secondBeat) {
    secondBeat = false;
    for (int i = 0; i <= 9; i++) {
      rate[i] = IBI;
    }
  }

  word runningTotal = 0;

  for (int i = 0; i <= 8; i++) {
    rate[i] = rate[i + 1];
    runningTotal += rate[i];
  }

  rate[9] = IBI;
  runningTotal += rate[9];
  runningTotal /= 10;
  myBPM = 60000 / runningTotal;
  QS = true;
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
  Blynk.begin(auth, ssid, pass);

  dht.begin();
  pixels.begin();

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

  //BPM
  if (QS == true) {
    Serial.println("BPM: " + String(myBPM));
    QS = false;
  } else if (millis() >= (lastTime + 2)) {
    readPulse();
    lastTime = millis();
    Serial.println("no pulse");

  }

  if (ageSelect == 1)
    age = 18;
  else if (ageSelect == 2)
    age = 26;
  else
    age = 30;

  int maxBPM = 220 - age;
  int idealBPMHigh = maxBPM * (85 / 100);
  int idealBPMLow = maxBPM * (70 / 100);
  int highTemp = 38;
  int lowTemp = 35;

  if (myBPM > idealBPMHigh || t >= highTemp) {
    //red
    colorwipe(50, 0, 0);
  } else if (myBPM >= idealBPMLow && myBPM <= idealBPMHigh || t > lowTemp && t < highTemp) {
    //yellow
    colorwipe(50, 50, 0);
  } else {
    //green
    colorwipe(0, 50, 0);
  }

  delay(20);

  Blynk.run();
  timer.run();


}
