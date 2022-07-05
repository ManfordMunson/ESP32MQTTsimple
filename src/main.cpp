#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

#define LED 2

const char* ssid = "network";
const char* pass = "password";
const char* mqtt_server = "address";
const char* topic = "topic/subtopic";

WiFiClient espClient2;
PubSubClient client2(espClient2);

//Initial connection to WiFi
void initWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,pass);
  Serial.println("Connecting to WiFi.");
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
  }
  Serial.println("Connected!");
}

void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  String output = "";
  Serial.print("Message: ");
  for(int i = 0; i < length; i++)
  {
    output += (char)payload[i];
    //Serial.print((char)payload[i]);
  }
  Serial.println(output);
  if(output == "BOUGHT")
  {
    digitalWrite(LED, HIGH);
  }
  else if(output == "NOT BOUGHT")
  {
    digitalWrite(LED, LOW);
  }
  Serial.println("------------"); 
}

void setup() 
{
  Serial.begin(115200);
  initWiFi();
  pinMode(LED, OUTPUT);
  client2.subscribe(topic);
  client2.setCallback(callback);
}

void reconnect()
{
  Serial.println("Connecting to MQTT SERVER");
  while(!client2.connected())
  {
    Serial.print(".");
    client2.setServer(mqtt_server,1883);
    String clientID = "ESP32Client-";
    clientID += String(random(0xffff), HEX);

    if (client2.connect(clientID.c_str()))
    {
      Serial.println("Connected to MQTT SEVER!");
      client2.subscribe("groceries/beans");
    }
  }
}

void loop() 
{
  if(!client2.connected()){
    reconnect();
  }
  client2.loop();
}