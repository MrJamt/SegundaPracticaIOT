#include <WiFi.h>

const char *WIFI_SSID = "Adriel";
const char *WIFI_PASS = "12345678";

const char *SERVER_ADDRESS = "192.168.100.57";
const int SERVER_PORT = 1000;

int GREEN_LED = 33;
int YELLOW_LED1 = 25;
int YELLOW_LED2 = 26;
int RED_LED = 27;

void setLedStateFromString(String ledStateString) {
  String values[4];
  int index = 0;
  int separatorIndex = 0;

  while (separatorIndex >= 0 && index < 4) {
    separatorIndex = ledStateString.indexOf('|');
    if (separatorIndex >= 0) {
      values[index] = ledStateString.substring(0, separatorIndex);
      ledStateString = ledStateString.substring(separatorIndex + 1);
    } else {
      values[index] = ledStateString;
    }
    index++;
  }

  int greenState = (values[0] == "HIGH") ? HIGH : LOW;
  int yellow1State = (values[1] == "HIGH") ? HIGH : LOW;
  int yellow2State = (values[2] == "HIGH") ? HIGH : LOW;
  int redState = (values[3] == "HIGH") ? HIGH : LOW;

  digitalWrite(GREEN_LED, greenState);
  digitalWrite(YELLOW_LED1, yellow1State);
  digitalWrite(YELLOW_LED2, yellow2State);
  digitalWrite(RED_LED, redState);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED1, OUTPUT);
  pinMode(YELLOW_LED2, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  Serial.print("Connecting to: ");
  Serial.print(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
  }

  Serial.print("\nIP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  Serial.print("Connecting to: ");
  Serial.println(SERVER_ADDRESS);

  WiFiClient client;

  if (!client.connect(SERVER_ADDRESS, SERVER_PORT)) {
    Serial.println("Connection failed");
    delay(300);
    return;
  }

  String get = "GET\n";
  client.print(get.c_str());

  unsigned long previousMillis = millis();
  while (!client.available() && millis() - previousMillis < 1000);

  if (client.available() > 0) {
    String line = client.readStringUntil('\n');

    Serial.print("Received value from server: ");
    Serial.println(line);

    setLedStateFromString(line);

  } else {
    Serial.println("Server timeout.");
  }

  Serial.println("Closing connection.");
  client.stop();
}
