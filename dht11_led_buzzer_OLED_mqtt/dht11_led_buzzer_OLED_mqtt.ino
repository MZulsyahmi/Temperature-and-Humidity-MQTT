/*---------------------KRENOVATOR---------------------
  TEMPERATURE and HUMIDITY SENSOR + LED + BUZZER + OLED
  display + MQTT Adafruit

  Demonstrates the DHT11 or DHT22 to know its function
  and how to code it with the trigger of LED and
  buzzer. The temperature and humidity will display
  in OLED LCD display with the connection to the
  Adafruit MQTT. Making it as an IoT

  Get the code at github
  https://github.com/MZulsyahmi/dht11.git

  by M.Zulsyahmi @krenovator
  October 2018
 */

#include "DHT.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DHTPIN 0
#define DHTTYPE DHT22
#define buzzer 14
#define OLED_RESET LED_BUILTIN
Adafruit_SSD1306 display(OLED_RESET);

DHT dht(DHTPIN,DHTTYPE);

int ledG = 15;
int ledY = 13;
int ledR = 12;

#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#if (SSD1306_LCDHEIGHT != 64)   //to declare the pixel of OLED
#error("Height incorrect, please fix Adafruit_SSD1306.h!");   //if got an error, the codes need to be changed
#endif

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "Your SSID"  //Remote Year / MyEdTech
#define WLAN_PASS       "Your Password" //RemoteYearKL / myedtechpnt

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "Your Username"
#define AIO_KEY         "Your AIO key"

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish Temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/feeds_for_temperature");
Adafruit_MQTT_Publish Humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/feeds_for_humidity");

// Setup a feed called 'onoff' for subscribing to changes.
//Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/onoff");

/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

void setup() {
  Serial.println("Device Started");
  Serial.println("-----------------------------");
  Serial.println("Running DHT!");
  Serial.println("-----------------------------");

  display.clearDisplay();

  Serial.begin(115200);
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)

  pinMode(ledR,OUTPUT);
  pinMode(ledY,OUTPUT);
  pinMode(ledG,OUTPUT);
  pinMode(buzzer,OUTPUT);

  // Clear the buffer.
  display.clearDisplay();

  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  //mqtt.subscribe(&onoffbutton);
}

uint32_t x=0;

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  // Now we can publish stuff!
  Serial.print(F("\nSending photocell val "));
  Serial.print(x);
  Serial.print("...");
  
//---------------------------------------------------------------------------

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print("  Humidity: ");
  Serial.println(h);

  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.print(t);
  display.print(" C");

  display.setTextColor(WHITE);
  display.setCursor(0, 32);
  display.setTextSize(2);
  display.print(h);
  display.print(" %");
  display.display();
  
  if(t>=24 && t<26){
    digitalWrite(ledG,HIGH);
    digitalWrite(ledY,LOW);
    digitalWrite(ledR,LOW);
    digitalWrite(buzzer,LOW);
  }

  else if(t>=26 && t<28){
    digitalWrite(ledG,LOW);
    digitalWrite(ledY,HIGH);
    digitalWrite(ledR,LOW);
    digitalWrite(buzzer,LOW);
  }

  else if(t>=28){
    digitalWrite(ledG,LOW);
    digitalWrite(ledY,LOW);
    digitalWrite(ledR,HIGH);
    digitalWrite(buzzer,HIGH);
  }

  //---------------------------------------------------------------------------
  
  if (!Humidity.publish(h)) {
    Serial.println(F("Failed"));
  } 
  if (!Temperature.publish(t)){
    Serial.println(F("Failed"));
  }
  else {
    Serial.println(F("OK!"));
  }

  delay(4000);
  display.clearDisplay();

  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  */
}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
