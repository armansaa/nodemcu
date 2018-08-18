    #include <ESP8266WiFi.h>
    #include <ArduinoJson.h>

    const char* ssid = "Secret";
    const char* password = "123admin456";

    const char* host = "line-php-slim-framework-laptop.herokuapp.com"; 
    String path = "/motor.json"; 
    const int pinPWM = D5;
    int pinA = D6;
    int pinB = D7;
    const int pinPWMB = D1;
    int pinC = D2;
    int pinD = D3;     

    void setup() {
    pinMode(pinPWM, OUTPUT);
    pinMode(pinA, OUTPUT);
    pinMode(pinB, OUTPUT);
    pinMode(pinPWMB, OUTPUT);
    pinMode(pinC, OUTPUT);
    pinMode(pinD, OUTPUT);
    Serial.begin(115200);

    delay(100);
    Serial.print("Connecting to ");

    WiFi.begin(ssid, password);
    int wifi_ctr = 0;
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }

    Serial.println("WiFi connected");
    Serial.println("IP address: " + WiFi.localIP());
    }

    void loop() {
    Serial.print("connecting to ");
    Serial.println(host);
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
    }

    client.print(String("GET ") + path + " HTTP/1.1\r\n" +
    "Host: " + host + "\r\n" +
    "Connection: keep-alive\r\n\r\n");

    delay(500); // wait for server to respond

    // read response
    String section="header";
    while(client.available()){
    String line = client.readStringUntil('\r');
    // Serial.print(line);
    // we'll parse the HTML body here
    if (section=="header") { // headers..
    Serial.print(".");
    if (line=="\n") { // skips the empty space at the beginning
    section="json";
    }
    }
    else if (section=="json") { // print the good stuff
    section="ignore";
    String result = line.substring(1);

    // Parse JSON
    int size = result.length() + 1;
    char json[size];
    result.toCharArray(json, size);
    StaticJsonBuffer<200> jsonBuffer;
    //while (!Serial.available()) {} 
    //DynamicJsonBuffer jsonBuffer;
    JsonObject& json_parsed = jsonBuffer.parseObject(json);
    if (!json_parsed.success()) 
    {
    //Serial.println(line);  
    Serial.println("parseObject() failed");
    return;
    }

    /*const char* json = "{\"type\":\"text\",\"id\":\"8202089738911\",\"text\":\"\"}";
    StaticJsonBuffer<200> jsonBuffer;

    JsonObject& json_parsed = jsonBuffer.parseObject(json);

    if (!json_parsed.success()) {
      Serial.println("ERROR");
      return;
    }

    const char* text = json_parsed["text"];

    Serial.println(text);*/
    

    // Make the decision to turn off or on the LED
    if (strcmp(json_parsed["roda 1"], "maju") == 0) {
    //Serial.println(line);)
    digitalWrite(pinPWM, HIGH);
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, LOW);
    digitalWrite(pinPWMB, HIGH);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, LOW);
    Serial.println("RODA 1 MAJU");
    }
    else if (strcmp(json_parsed["roda 1"], "mundur") == 0){
    digitalWrite(pinPWM, HIGH);
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinPWMB, HIGH);
    digitalWrite(pinC, LOW);
    digitalWrite(pinD, HIGH);
    Serial.println("RODA 1 MUNDUR");
    }
    else if (strcmp(json_parsed["roda 1"], "stop") == 0){
      digitalWrite(pinPWM, LOW);
      digitalWrite(pinA, LOW);
      digitalWrite(pinB, LOW);
      digitalWrite(pinPWMB, LOW);
      digitalWrite(pinC, LOW);
      digitalWrite(pinD, LOW);
      Serial.println("RODA 1 BERHENTI");
    }
    }
    }
    Serial.print("closing connection. ");
    }

