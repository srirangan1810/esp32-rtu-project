#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT11.h>
#define DHTPIN 21
DHT11 dht11(DHTPIN);
const char* ssid = "sri";
const char* password = "12345678";

String writeAPI = "http://api.thingspeak.com/update?api_key=BP9K287SF6NUESAM";
String readAPI  = "https://api.thingspeak.com/channels/3306518/fields/3/last.txt?api_key=E8ION3L5TYAGH88E";

#define MAX_BUFFER 15
int tempBuffer[MAX_BUFFER];
int humBuffer[MAX_BUFFER];
int bufferIndex = 0;


unsigned long lastSendTime = 0;
unsigned long lastReadTime = 0;

long sendInterval = 15000;   // default 15 sec
long readInterval = 20000;   // command check 20 sec


void connectWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;

  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");

  int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < 10) {
    delay(500);
    Serial.print(".");
    retry++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n WiFi Connected");
  } else {
    Serial.println("\n WiFi Failed");
  }
}

bool sendToCloud(int temp, int hum) {
  HTTPClient http;

  String url = writeAPI + "&field1=" + String(temp) + "&field2=" + String(hum);
  http.begin(url);

  int code = http.GET();
  http.end();

  if (code > 0) {
    Serial.println("SENT → " + String(temp) + "," + String(hum));
    return true;
  } else {
    Serial.println("SEND FAIL");
    return false;
  }
}

void storeData(int temp, int hum) {
  if (bufferIndex < MAX_BUFFER) {
    tempBuffer[bufferIndex] = temp;
    humBuffer[bufferIndex] = hum;

    Serial.println("BUFFER STORED → " + String(temp) + "," + String(hum));
    bufferIndex++;
  }
}


void sendBufferedData() {
  if (bufferIndex == 0) return;

  Serial.println("RESENDING BUFFER...");

  int i = 0;
  while (i < bufferIndex) {

    if (sendToCloud(tempBuffer[i], humBuffer[i])) {

      Serial.println("BUFFER SENT → " + String(tempBuffer[i]));

      for (int j = i; j < bufferIndex - 1; j++) {
        tempBuffer[j] = tempBuffer[j + 1];
        humBuffer[j] = humBuffer[j + 1];
      }

      bufferIndex--;
    } else {
      i++;
    }

    delay(15000);
  }
}


void getCommand() {

  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  http.begin(readAPI);

  int code = http.GET();

  if (code > 0) {

    String payload = http.getString();
    payload.trim();

    Serial.print("COMMAND RAW → ");
    Serial.println(payload);

    int newDelay = payload.toInt();

    if (newDelay > 0 && newDelay != sendInterval) {
      sendInterval = newDelay;
      Serial.println("NEW SEND INTERVAL → " + String(sendInterval));
    }

  } else {
    Serial.println("COMMAND READ FAIL");
  }

  http.end();
}


void setup() {
  Serial.begin(115200);
  delay(1000);

  connectWiFi();
}

void loop() {

  connectWiFi();

 
  if (millis() - lastSendTime > sendInterval) {

    int temp = 0, hum = 0;

    int result = dht11.readTemperatureHumidity(temp, hum);

    if (result == 0) {

      Serial.println("CURRENT → " + String(temp) + "," + String(hum));

      if (WiFi.status() == WL_CONNECTED) {

        if (!sendToCloud(temp, hum)) {
          storeData(temp, hum);
        }
        sendBufferedData();
      } else {
        storeData(temp, hum);
      }

    } else {
      Serial.println("SENSOR ERROR");
    }

    lastSendTime = millis();
  }
  if (millis() - lastReadTime > readInterval) {
    Serial.println("Checking command...");
    getCommand();
    lastReadTime = millis();
  }
}
