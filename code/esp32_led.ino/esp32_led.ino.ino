#include <ArduinoJson.h>

/*
  Wifi setup
*/

#ifdef ESP8266
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#else
#error "Board not found"
#endif

#include <PubSubClient.h>

const char* WIFI_SSID = "REEMO SPEEDO";
const char* WIFI_PASSWORD = "REMOHULUKA";
const char* MQTT_SERVER = "10.0.0.7";  // Local IP address of Raspberry Pi
const int PORT = 1883;

const char* MQTT_USERNAME = "Anudeep-esp32";
const char* MQTT_PASS = "";


WiFiClient espClient;
PubSubClient client(espClient);

// #define LED      2
// #define RELAY_1 12
// #define RELAY_2 26
// #define RELAY_3 25
// #define RELAY_4  3
// #define RELAY_5 15

#define PIN_1   1
#define PIN_LED 2
#define PIN_3   3
#define PIN_4   4
#define PIN_5   5
#define PIN_6   6
#define PIN_7   7
#define PIN_8   8
#define PIN_9   9
#define PIN_10  10
#define PIN_11  11
#define PIN_12  12
#define PIN_13  13
#define PIN_14  14
#define PIN_15  15
#define PIN_16  16
#define PIN_17  17
#define PIN_18  18
#define PIN_19  19
#define PIN_20  20
#define PIN_21  21
#define PIN_22  22
#define PIN_23  23
#define PIN_24  24
#define PIN_25  25
#define PIN_26  26
#define PIN_27  27
#define PIN_28  28
#define PIN_29  29
#define PIN_30  30
#define PIN_31  31
#define PIN_32  32
#define PIN_33  33

#define TOPIC_TURN_OFF "esp_wroom/off"
#define TOPIC_UPDATE "esp_wroom/update"

DynamicJsonDocument doc(1024);


void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// Connecting to MQTT broker
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-" + String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), MQTT_USERNAME, MQTT_PASS)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe(TOPIC_TURN_OFF);
      client.subscribe(TOPIC_UPDATE);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      // Wait 2 seconds before retrying
      delay(2000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Topic recvd [");
  Serial.print(topic);
  Serial.print("]: Payload = ");

  if (strstr(topic, "esp_wroom/off")) {
    Serial.print((int) payload);
    Serial.print(" turned off!");
    digitalWrite((int) payload, LOW);
    return;
  }

  deserializeJson(doc, payload);
  int pin = doc["pin"];

  Serial.println(pin);
  digitalWrite(pin, HIGH);
}

void setup() {
  /*
     Helps opening a serial connection so we can output the result
  */
  Serial.begin(9600);

  // Set pin mode
  Serial.println("Setting up PIN(s)...");
  pinMode(PIN_1, OUTPUT);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_3, OUTPUT);
  pinMode(PIN_4, OUTPUT);
  pinMode(PIN_5, OUTPUT);
  // pinMode(PIN_6, OUTPUT);
  pinMode(PIN_7, OUTPUT);
  pinMode(PIN_8, OUTPUT);
  pinMode(PIN_9, OUTPUT);
  pinMode(PIN_10, OUTPUT);
  pinMode(PIN_11, OUTPUT);
  pinMode(PIN_12, OUTPUT);
  pinMode(PIN_13, OUTPUT);
  pinMode(PIN_14, OUTPUT);
  pinMode(PIN_15, OUTPUT);
  pinMode(PIN_16, OUTPUT);
  pinMode(PIN_17, OUTPUT);
  pinMode(PIN_18, OUTPUT);
  pinMode(PIN_19, OUTPUT);
  pinMode(PIN_20, OUTPUT);
  pinMode(PIN_21, OUTPUT);
  pinMode(PIN_22, OUTPUT);
  pinMode(PIN_23, OUTPUT);
  pinMode(PIN_24, OUTPUT);
  pinMode(PIN_25, OUTPUT);
  pinMode(PIN_26, OUTPUT);
  pinMode(PIN_27, OUTPUT);
  pinMode(PIN_28, OUTPUT);
  pinMode(PIN_29, OUTPUT);
  pinMode(PIN_30, OUTPUT);
  pinMode(PIN_31, OUTPUT);
  pinMode(PIN_32, OUTPUT);
  pinMode(PIN_33, OUTPUT);
  pinMode(34, OUTPUT);
  pinMode(35, OUTPUT);
  pinMode(36, OUTPUT);
  pinMode(37, OUTPUT);
  pinMode(38, OUTPUT);
  pinMode(39, OUTPUT);
  pinMode(40, OUTPUT);
  Serial.println("PIN setup finished...");

  setup_wifi();

  client.setServer(MQTT_SERVER, PORT);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  delay(2000);    // 2000ms
  digitalWrite(PIN_LED, HIGH);  // Turn off LEDs

  delay(500);     // 500ms
  digitalWrite(PIN_LED, LOW);  // Turn on LED
}
