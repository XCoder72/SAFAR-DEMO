#include <WiFi.h>
#include <HTTPClient.h>

// --- CONFIGURATION ---
const char* ssid = "Wokwi-GUEST";
const char* password = "";
// 👇 ENSURE THIS URL IS CORRECT
const char* firebase_url = "https://safar-hackathon-default-rtdb.asia-southeast1.firebasedatabase.app/devices/BOX_001.json";

// --- PINS ---
const int PIN_SOS_BTN = 15;    
const int PIN_PAIR_SW = 4;     
const int PIN_LED_SOS = 2;     
const int PIN_LED_PAIR = 5;    

// --- VARIABLES ---
float lat = 26.2183; 
float lon = 78.1828;
int speed = 0;

// COUNTER variable (Not boolean)
// When 0 = SOS OFF. When > 0 = SOS ON.
volatile int sosTransmissionCount = 0;

// INTERRUPT: Set counter to 2 when pressed
void IRAM_ATTR handleSOS() {
  sosTransmissionCount = 2; 
}

void setup() {
  Serial.begin(115200);
  
  pinMode(PIN_SOS_BTN, INPUT_PULLUP);
  pinMode(PIN_PAIR_SW, INPUT_PULLUP);
  pinMode(PIN_LED_SOS, OUTPUT);
  pinMode(PIN_LED_PAIR, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(PIN_SOS_BTN), handleSOS, FALLING);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
}

void loop() {
  // 1. READ PAIRING SWITCH
  bool isPaired = digitalRead(PIN_PAIR_SW) == LOW; 
  digitalWrite(PIN_LED_PAIR, isPaired ? HIGH : LOW);

  // 2. CHECK SOS COUNTER
  bool currentSosState = false;

  if (sosTransmissionCount > 0) {
    currentSosState = true;
    sosTransmissionCount--; // Use up 1 "credit"
    
    // Turn Red LED ON while transmitting SOS
    digitalWrite(PIN_LED_SOS, HIGH); 
  } else {
    currentSosState = false;
    digitalWrite(PIN_LED_SOS, LOW);
  }

  // 3. SEND TO FIREBASE
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Simulate Movement
    if (isPaired) {
      lat += (random(-20, 20) / 100000.0);
      lon += (random(-20, 20) / 100000.0);
      speed = random(10, 60);
    } else {
      speed = 0;
    }

    // Prepare JSON
    String json = "{";
    json += "\"lat\":" + String(lat, 6) + ",";
    json += "\"lon\":" + String(lon, 6) + ",";
    json += "\"speed\":" + String(speed) + ",";
    json += "\"paired\":" + String(isPaired ? "true" : "false") + ",";
    json += "\"sos\":" + String(currentSosState ? "true" : "false"); 
    json += "}";

    http.begin(firebase_url);
    http.addHeader("Content-Type", "application/json");
    int response = http.PUT(json);
    
    Serial.print("Sent Data | SOS Status: ");
    Serial.println(currentSosState ? "TRUE (ALERT)" : "FALSE (Normal)");
    
    http.end();
  }

  // Wait 2 seconds before next send
  delay(500); 
}