#define S0 D3
#define S1 D2
#define S2 D1
#define S3 D0
#define analogpin A0

void setup() {

  pinMode(analogpin, INPUT);
  pinMode(S0,OUTPUT);
  pinMode(S1,OUTPUT);
  pinMode(S2,OUTPUT);
  pinMode(S3,OUTPUT);  
  Serial.begin(115200);

  // put your setup code here, to run once:
}

void loop() {

  digitalWrite(S0,LOW);
  digitalWrite(S1,LOW);
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  Serial.print("Sensor 13 ");Serial.println(analogRead(analogpin));

  digitalWrite(S0,HIGH);
  digitalWrite(S1,HIGH);
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  Serial.print("Sensor 16 ");Serial.println(analogRead(analogpin));

  delay(5000);
  // put your main code here, to run repeatedly:

}
