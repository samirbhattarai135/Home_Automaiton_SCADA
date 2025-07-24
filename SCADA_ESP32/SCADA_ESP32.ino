/*********
  Author: Samir Bhattarai
  This code is built upon the example code in pubsubclient library 
  Complete project details at https://helloworld.co.in
*********/

#include <WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Pin definitions
#define FLAME_SENSOR 13
#define FLASH 4
#define LIGHT 5
#define BUZZER 12
#define ONE_WIRE_BUS 2
#define FAN 14

// Setup for temp sensor DS18B20
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Wi-Fi credentials
const char* ssid = "we";
const char* password = "wewewewe";

//MQTT Broker IP address:
const char* mqtt_server = "10.0.0.156";

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
long lastTempMsg = 0;

void blink_led(unsigned int times, unsigned int duration){
  for (int i = 0; i < times; i++) {
    digitalWrite(FLASH, HIGH);
    delay(duration); 
    digitalWrite(FLASH, LOW);
    delay(200);
  }
}

void setup_wifi() {
  delay(50);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    blink_led(2,200); //blink LED twice (for 200ms ON time) to indicate that wifi not connected
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void connect_mqttServer() {
  // Loop until we're reconnected
  while (!client.connected()) {
    if(WiFi.status() != WL_CONNECTED){
      setup_wifi();
    }
    Serial.print("Attempting MQTT connection...");
    if (client.connect("Esp32 Client")) { 
      Serial.println("connected");
      // Subscribe to topics here
      client.subscribe("esp32/led");
      client.subscribe("esp32/fan");
      Serial.println("Subscribed to esp32/led and esp32/fan topics");
      client.publish("Esp32/Data", "Hello World");
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" trying again in 2 seconds");
      blink_led(3,200);
      delay(2000);
    }
  }
}

//this function will be executed whenever there is data available on subscribed topics
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // LED control from MQTT
  if (String(topic) == "esp32/led") {
    Serial.println("Processing LED command");
    if (messageTemp == "ON") {
      digitalWrite(LIGHT, HIGH);
      Serial.println("LED turned ON");
    } else if (messageTemp == "OFF") {
      digitalWrite(LIGHT, LOW);
      Serial.println("LED turned OFF");
    }
  }

  // Fan control from MQTT
  if (String(topic) == "esp32/fan") {
    Serial.println("Processing FAN command");
    if (messageTemp == "ON") {
      digitalWrite(FAN, HIGH);
      Serial.println("Fan turned ON");
    } else if (messageTemp == "OFF") {
      digitalWrite(FAN, LOW);
      Serial.println("Fan turned OFF");
    }
  }

  // Check if a message is received on the topic "Esp32/Data"
  if (String(topic) == "Esp32/Data") {
    if(messageTemp == "10"){
      Serial.println("Action: blink LED");
      blink_led(1,1250);
    }
  }
}

float readTemperature() {
  sensors.requestTemperatures();
  
  // Check if any sensors are found
  if (sensors.getDeviceCount() == 0) {
    Serial.println("Error: No DS18B20 sensors found");
    return -999.0;
  }
  
  float temperature = sensors.getTempCByIndex(0);
  
  if (temperature == DEVICE_DISCONNECTED_C || temperature == 85.0) {
    Serial.println("Error: Could not read temperature data - Check wiring and pull-up resistor");
    return -999.0;
  }
  
  return temperature;
}

void setup() {
  Serial.begin(115200);
  pinMode(FLASH, OUTPUT);
  pinMode(LIGHT, OUTPUT);
  pinMode(FLAME_SENSOR, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(FAN, OUTPUT);

  // Initialize DS18B20 sensor
  sensors.begin();
  Serial.print("DS18B20 Temperature Sensor initialized. Found ");
  Serial.print(sensors.getDeviceCount());
  Serial.println(" sensors.");
  
  setup_wifi();
  client.setServer(mqtt_server,1883);
  client.setCallback(callback);
}

void loop() {
 
  if (!client.connected()) {
    connect_mqttServer();
  }

  client.loop();

  int flameDetected = digitalRead(FLAME_SENSOR);
  bool flame = (flameDetected == HIGH);
  
  if(flame == true){
    Serial.println("Fire Detected!!! Fire Alarm ON");
  }

  digitalWrite(BUZZER, flame ? HIGH : LOW);
  digitalWrite(FLASH, flame ? HIGH : LOW);

  long now = millis();
  if (now - lastMsg > 4000) {
    lastMsg = now;

    // Convert flameDetected value to string and publish
    char payload[8];
    snprintf(payload, sizeof(payload), "%d", flameDetected);
    client.publish("esp32/sensor1", payload);
  }
  
  if (now - lastTempMsg > 5000) {
    lastTempMsg = now;
    
    float temperature = readTemperature();
    if (temperature != -999.0) {
      char tempPayload[10];
      dtostrf(temperature, 6, 2, tempPayload); 
      client.publish("esp32/temperature", tempPayload);
      
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.println("°C");
    }
  }
}
