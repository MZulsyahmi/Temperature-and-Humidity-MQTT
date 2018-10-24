#include "DHT.h"

#define DHTPIN 0
#define DHTTYPE DHT11
#define buzzer 14
DHT dht(DHTPIN,DHTTYPE);

int ledG = 15;
int ledY = 13;
int ledR = 12;

void setup() {
  Serial.println("Device Started");
  Serial.println("-----------------------------");
  Serial.println("Running DHT!");
  Serial.println("-----------------------------");

  Serial.begin(9600);

  pinMode(ledR,OUTPUT);
  pinMode(ledY,OUTPUT);
  pinMode(ledG,OUTPUT);
  pinMode(buzzer,OUTPUT);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print("  Humidity: ");
  Serial.println(h);

  if(t==24){
    digitalWrite(ledG,HIGH);
    digitalWrite(ledY,LOW);
    digitalWrite(ledR,LOW);
    digitalWrite(buzzer,LOW);
  }

  else if(t==25){
    digitalWrite(ledG,LOW);
    digitalWrite(ledY,HIGH);
    digitalWrite(ledR,LOW);
    digitalWrite(buzzer,LOW);
  }

  else if(t>25){
    digitalWrite(ledG,LOW);
    digitalWrite(ledY,LOW);
    digitalWrite(ledR,HIGH);
    digitalWrite(buzzer,HIGH);
  }
  
  delay(1000);
}
