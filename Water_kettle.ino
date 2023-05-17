// include necessary libraries
#include <DHT.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// define constants
#define DHTPIN 4
#define DHTTYPE DHT11
#define RELAY_PIN 14
#define BOILING_TEMPERATURE 35 // change this to the boiling temperature of your kettle
#define BLYNK_TEMPLATE_ID "TMPL5S88xLXej"
#define BLYNK_TEMPLATE_NAME "Water Kettle"

// initialize DHT sensor object
DHT dht(DHTPIN, DHTTYPE);

// Blynk authentication credentials and WiFi network details
char auth[] = "Z0Q-bl5Ci0yTcDlE72mWtePkuZyNW-of";
char ssid[] = "SLT-Fiber-9987";
char pass[] = "ks2499881";

// initialize Blynk timer
BlynkTimer timer;

// setup function
void setup() {
// start serial communication
Serial.begin(9600);

// connect to Blynk server with authentication and WiFi details
Blynk.begin(auth, ssid, pass);

// start DHT sensor
dht.begin();

// set relay pin as output and turn it off
pinMode(RELAY_PIN, OUTPUT);
digitalWrite(RELAY_PIN, LOW);

// set interval for timer to read temperature
timer.setInterval(1000L, readTemperature);
}

// function to read temperature and humidity and update Blynk app widgets
void readTemperature() {
// read temperature and humidity
float temperature = dht.readTemperature();
float humidity = dht.readHumidity();

// update temperature and humidity widgets in Blynk app
Blynk.virtualWrite(V1, temperature);
Blynk.virtualWrite(V2, humidity);

// check if temperature is at or above boiling temperature and turn off relay and update button widget in app
if (temperature >= BOILING_TEMPERATURE) {
digitalWrite(RELAY_PIN, LOW);
Blynk.virtualWrite(V3, LOW);
Blynk.logEvent("your_water_is_ready", ""); // log event
}
}

// function to control relay using button widget in Blynk app
BLYNK_WRITE(V3) {
int state = param.asInt();
if (state == HIGH) {
digitalWrite(RELAY_PIN, HIGH);
} else {
digitalWrite(RELAY_PIN, LOW);
}
}

// loop function to run Blynk and timer functions
void loop() {
Blynk.run();
timer.run();
}
