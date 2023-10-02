#include <WiFi.h>

const char *WIFI_SSID = "Adriel";
const char *WIFI_PASS = "12345678";

const char *SERVER_ADDRESS = "192.168.100.57";
const int SERVER_PORT = 1000;

int triggerPin = 14;
int echoPin = 12;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.print("Connecting to: ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
  }

  Serial.print("\IP Address: ");
  Serial.println(WiFi.localIP());
}

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  return pulseIn(echoPin, HIGH);
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

  int distance = readUltrasonicDistance(triggerPin, echoPin) * 0.01723;
  Serial.print("Distance in cm: ");
  Serial.println(distance);

  client.println(distance);
  delay(500);

  Serial.println("Closing connection.");
  client.stop();
}
