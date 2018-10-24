/*---------------------KRENOVATOR---------------------
  TEMPERATURE and HUMIDITY SENSOR

  Demonstrates the DHT11 or DHT22 to know its function
  and how to code it.

  by M.Zulsyahmi @krenovator
  October 2018
 */

#include "DHT.h"

#define DHTPIN 5     // what digital pin the DHT22 is conected to
#define DHTTYPE DHT11   // there are multiple kinds of DHT sensors DHT11 or DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(2000);

  // Wait for serial to initialize.
  while(!Serial) { }

  Serial.println("Device Started");
  Serial.println("-------------------------------------");
  Serial.println("Running DHT!");
  Serial.println("-------------------------------------");

}

void loop() {
  float t = dht.readTemperature();    //initialize the measured temperature
  float h = dht.readHumidity();       //initialize the measure humidity

  Serial.print("Temperature= ");
  Serial.print(t);                  //recall the temperature
  Serial.print("  Humidity = ");
  Serial.println(h);                //recall the humidity
  delay(2000);          //delay time for 2sec
}
