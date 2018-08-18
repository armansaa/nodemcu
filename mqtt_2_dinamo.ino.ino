#include <ESP8266WiFi.h>
#include <PubSubClient.h>
 
const char* ssid = "Secret";
const char* password =  "123admin456";
const char* mqttServer = "m10.cloudmqtt.com";
const int mqttPort = 10676;
const char* mqttUser = "nodemcu";
const char* mqttPassword = "nodemcu";

int pinPWM = D5;
int pinA = D6;
int pinB = D7;
int pinPWMB =D1;
int pinC = D2;
int pinD = D3; 

WiFiClient espClient;
PubSubClient client(espClient);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(pinPWM, OUTPUT);
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinPWMB, OUTPUT);
  pinMode(pinC, OUTPUT);
  pinMode(pinD, OUTPUT);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to Wifi..");
  }
  Serial.println("Connected to the WiFi network");

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP Client", mqttUser, mqttPassword)) {

      Serial.println("connected");
    } else {

      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }

  client.publish("esp/test", "Hello from esp8266");
  client.subscribe("esp/test");

}

void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  Serial.println();
  Serial.println("---------------");

  if(payload[0] == '1'){
    digitalWrite(pinPWM, 50);
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, LOW);
    digitalWrite(pinPWMB, 50);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, LOW);
    client.publish("esp/test", "Roda maju");
  }
  else if (payload[0] == '2' ) {
    digitalWrite(pinPWM, 50);
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinPWMB, 50);
    digitalWrite(pinC, LOW);
    digitalWrite(pinD, HIGH);
    client.publish("esp/test", "Roda mundur"); 
  } 
  else if (payload[0] == '0' ) {
    digitalWrite(pinPWM, 0);
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, LOW);
    digitalWrite(pinPWMB, 0);
    digitalWrite(pinC, LOW);
    digitalWrite(pinD, LOW);
    client.publish("esp/test", "Roda berhenti");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  client.loop();

}
