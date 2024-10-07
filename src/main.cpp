#include <WiFi.h>
#include <HTTPClient.h>
#include <MFRC522.h>
#include <SPI.h>

#define SS_PIN 5
#define RST_PIN 22

MFRC522 rfid(SS_PIN, RST_PIN);

const char* ssid = "FTTx_ABED-2G";
const char* password = "EDE7ABED";
const char* serverURL = "http://192.168.1.34/rfid_group8/rfid_db.php";

void connectToWiFi();
void sendToServer(String rfid_id);

void setup() {
  Serial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();
  connectToWiFi();
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    delay(500);
    return;
  }

  String rfid_id = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    rfid_id += String(rfid.uid.uidByte[i], HEX);
  }
  rfid_id.toUpperCase();

  Serial.println("RFID Scanned: " + rfid_id);
  sendToServer(rfid_id);
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");
}

void sendToServer(String rfid_id) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String postData = "rfid_id=" + rfid_id;
    http.begin(serverURL);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int httpResponseCode = http.POST(postData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Response from server: " + response);
    } else {
      Serial.println("Error on HTTP request");
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}
