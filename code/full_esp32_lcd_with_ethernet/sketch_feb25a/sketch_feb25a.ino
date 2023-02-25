#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <EthernetENC.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#else
#error "Board not found"
#endif

const char* MQTT_SERVER = "192.168.1.101";  // Local IP address of Raspberry Pi
const int PORT = 1883;

const char* MQTT_USERNAME = "homeserv";
const char* MQTT_PASS = "password";

#define IP 192, 168, 1, 109
#define DNS 192, 168, 1, 1
#define GATEWAY 192, 168, 1, 1
#define SUBNET 255, 255, 255, 0


int lcdColumns = 16;
int lcdRows = 2;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

const char* title = "";
const char* msg = "";

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);
EthernetClient client;
PubSubClient psClient(client);
DynamicJsonDocument doc(1024);

void connect() {

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting...");
  Serial.println("Connecting...");

  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1);  // do nothing, no point running without Ethernet hardware
    }
  }


  Serial.println("Eth being mac...");
  if (Ethernet.begin(mac)) {
    lcd.setCursor(0, 1);
    lcd.print("Ethernet D OK");
    Serial.println("Ethernet D Ok.");
  } else {
    Serial.println("Eth being Static...");
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
      delay(2000);
    }

    IPAddress ip(IP);
    IPAddress dns(DNS);
    IPAddress gw(GATEWAY);
    IPAddress sn(SUBNET);

    Ethernet.begin(mac, ip, dns, gw, sn);
    lcd.setCursor(0, 1);
    lcd.print("Ethernet S OK");
    Serial.println("Ethernet S Ok.");
  }

  delay(5000);
  printInfo();
  randomSeed(micros());

  if (client.connect("google.com", 80)) {
    Serial.println("Connected!");
    // Make a HTTP request:
    client.println("GET /get HTTP/1.1");
    client.println("Host: httpbin.org");
    client.println("Connection: close");
    client.println();
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}

void loopInfo() {
  if (strlen(title) && strlen(msg)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(title);
    if (strlen(msg) > 15) {
      scrollText(1, msg, 200, 16);
    } else {
      lcd.setCursor(0, 1);
      lcd.print(msg);
    }
  }
}

void scrollText(int row, String message, int delayTime, int lcdColumns) {
  for (int i = 0; i < lcdColumns; i++) {
    message = " " + message;
  }
  message = message + " ";
  for (int pos = 0; pos < message.length(); pos++) {
    lcd.setCursor(0, row);
    lcd.print(message.substring(pos, pos + lcdColumns));
    delay(delayTime);
  }
}

void printInfo() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(Ethernet.localIP());

  lcd.setCursor(0, 1);
  lcd.print(Ethernet.dnsServerIP());

  Serial.print("Local IP : ");
  Serial.println(Ethernet.localIP());
  Serial.print("Subnet Mask : ");
  Serial.println(Ethernet.subnetMask());
  Serial.print("Gateway IP : ");
  Serial.println(Ethernet.gatewayIP());
  Serial.print("DNS Server : ");
  Serial.println(Ethernet.dnsServerIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  if (strstr(topic, "display")) {
    deserializeJson(doc, payload);

    title = doc["title"];
    msg = doc["msg"];
  }
}

void setupMqtt() {
  Serial.println("Setrting up mqtt");
  psClient.setServer(MQTT_SERVER, PORT);
  psClient.setCallback(callback);
}

void reconnectMqtt() {
  while (!psClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-" + String(random(0xffff), HEX);

    if (psClient.connect(clientId.c_str(), MQTT_USERNAME, MQTT_PASS)) {
      Serial.println("connected");

      psClient.publish("outTopic", "hello world");
      psClient.subscribe("display");
    } else {
      Serial.print("failed, rc=");
      Serial.print(psClient.state());
      Serial.println(" try again in 2 seconds");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  delay(1000);

  lcd.init();
  Ethernet.init(5);
  Serial.println("Eth init...");
  lcd.backlight();

  connect();
  setupMqtt();
}

void loop() {
  // if (!client.connected()) {
  //   connect();
  // }

  reconnectMqtt();
  psClient.loop();
  loopInfo();
}
