/*---------------------KRENOVATOR---------------------
  TEMPERATURE and HUMIDITY SENSOR + LED + BUZZER

  Demonstrates the DHT11 or DHT22 to know its function
  and how to code it with the trigger of LED and
  buzzer

  Get the code at github
  https://github.com/MZulsyahmi/dht11.git

  by M.Zulsyahmi @krenovator
  October 2018
 */

#include "DHT.h"      //include the library of DHT

#define DHTPIN 0
#define DHTTYPE DHT11
#define buzzer 14             //declare the buzzer

DHT dht(DHTPIN,DHTTYPE);

/*initialize the LED*/
int ledG = 15;
int ledY = 13;
int ledR = 12;

void setup() {
  Serial.println("Device Started");
  Serial.println("-----------------------------");
  Serial.println("Running DHT!");
  Serial.println("-----------------------------");

  Serial.begin(9600);
  
/*declare the LED and buzzer*/
  pinMode(ledR,OUTPUT);
  pinMode(ledY,OUTPUT);
  pinMode(ledG,OUTPUT);
  pinMode(buzzer,OUTPUT);
}

void loop() {
  float h = dht.readHumidity();       //initialize the measure humidity
  float t = dht.readTemperature();    //initialize the measured temperature
  
  Serial.print("Temperature: ");
  Serial.print(t);                  //recall the temperature
  Serial.print("  Humidity: ");
  Serial.println(h);                //recall the humidity

  /*condition that needed to be met*/
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
  
  delay(2000);          //delay time for 2sec
}
