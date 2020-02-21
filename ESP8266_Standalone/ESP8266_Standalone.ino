
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
BlynkTimer timer;

#include "DHT.h"
#define DHTPIN D7
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

float h;
float t;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "1JAdGp1n4nKZLAPbDivMSMRqvemy68VX";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "stdio.h";
char pass[] = "cubaHack_23";

void setup()
{
  // Debug console
  Serial.begin(9600);
  dht.begin();
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, sendSensor);
  
}

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.println("Humidity and temperature\n\n");
  Serial.print("Current humidity = ");
  Serial.print(h);
  Serial.print("%  ");
  Serial.print("temperature = ");
  Serial.print(t);
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);
}

void loop()
{
  Blynk.run();
  timer.run();
}
