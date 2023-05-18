#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <WiFiClient.h>
#include "webserverPostTest3B5.h"; // file webserver
#include <ESP8266WebServer.h>
#include <LiquidCrystal_I2C.h> 
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);
 
const int sampleWindow = 50;                              // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
 
#define SENSOR_PIN A0 // Pin Microphone
#define PIN_QUIET D6 // Pin LED merah
#define PIN_MODERATE D7 // Pin LED kuning
#define PIN_LOUD D8 // Pin LED hijau
#define BUTTON_PIN D4 //Pin push button

bool ledOn = false;

// Konfigurasi WiFi

#define WIFI_SSID "Universitas Mulawarman"
#define WIFI_PASSWORD ""

// Konfigurasi Thingspeak
#define THINGSPEAK_CHANNEL_ID "2095833"
#define THINGSPEAK_API_KEY "NQ3H3HT96W00PQQ3"

// Konfigurasi Broker MQTT
#define MQTT_SERVER "broker.hivemq.com"
int MQTT_PORT = 1883;
#define MQTT_TOPIC "iot_unmul/iot_b_5"

// Buat instance WiFiClient dan PubSubClient
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

ESP8266WebServer server(80);

  void webpage()
  {
    server.send(200,"text/html", webserverPostTest2B5);
  }

void handleSave() {
  String mqttServer = server.arg("mqtt_server");
  String mqttPort = server.arg("mqtt_port");
  String mqttTopic = server.arg("mqtt_topic");
  String thingspeakChannel = server.arg("thingspeak_channel");
  String thingspeakAPIKey = server.arg("thingspeak_apikey");

  mqttServer.toCharArray(MQTT_SERVER, sizeof(MQTT_SERVER));
  MQTT_PORT = mqttPort.toInt();
  mqttTopic.toCharArray(MQTT_TOPIC, sizeof(MQTT_TOPIC));
  thingspeakChannel.toCharArray(THINGSPEAK_CHANNEL_ID, sizeof(THINGSPEAK_CHANNEL_ID));
  thingspeakAPIKey.toCharArray(THINGSPEAK_API_KEY, sizeof(THINGSPEAK_API_KEY));

  server.send(200, "text/plain", "Settings saved");
}

 
void setup ()  
{   
  pinMode (SENSOR_PIN, INPUT);  
  pinMode (BUTTON_PIN, INPUT);
  pinMode(PIN_QUIET, OUTPUT);
  pinMode(PIN_MODERATE, OUTPUT);
  pinMode(PIN_LOUD, OUTPUT); 
 
  digitalWrite(PIN_QUIET, LOW);
  digitalWrite(PIN_MODERATE, LOW);
  digitalWrite(PIN_LOUD, LOW);
  
  Serial.begin(115200);

  connectToWiFi();
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setCallback(mqttCallback);
  lcd.init();
 
  // Menyalakan backlight LED
  lcd.backlight();

  Serial.println("Menyambungkan ke ");
  Serial.println(WIFI_SSID);
  
  lcd.setCursor(0, 0); // LED baris atas
  lcd.print("Menyambungkan ke");
 
  lcd.setCursor(0, 1); // LED baris bawah
  lcd.print(WIFI_SSID);
 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
    Serial.println("");
    Serial.println("WiFi terhubung");
    Serial.println("IP Address: ");
    Serial.println(WiFi.localIP());

  server.on("/", webpage);
  server.on("/save", handleSave);
  server.begin();
  Serial.println("Web server started");
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi terhubung");
    delay(4000);
    lcd.clear();
}  
 
   
void loop ()  
{ 
     server.handleClient();

  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  mqttClient.loop();
  
   unsigned long startMillis= millis();                   // Start of sample window
   float peakToPeak = 0;                                  // peak-to-peak level
 
   unsigned int signalMax = 0;                            // value signal minimum 
   unsigned int signalMin = 1024;                         // value signal maksimum
 
                                                          // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(SENSOR_PIN);                    // Baca inputan dari sensor mic
      if (sample < 1024)                                  // Buang semua inputan signal diatas 1024
      {
         if (sample > signalMax)
         {
            signalMax = sample;                           // Simpan signal maksimum jika signal > 1024
         }
         else if (sample < signalMin)
         {
            signalMin = sample;                           // Simpan signal minimum jika signal < 1024
         }
      }
   }
 
   peakToPeak = signalMax - signalMin;                    // max - min = peak-peak amplitude
   int db = map(peakToPeak,20,900,49.5,90);             // Kalibrasi desibel 
    publishToMQTT(db);
 
  lcd.setCursor(0, 0);
  lcd.print("Intensitas: ");
  lcd.print(db);
  lcd.print("dB");

  int buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == HIGH) {
    if (ledOn) {
      digitalWrite(PIN_QUIET, HIGH);
      digitalWrite(PIN_MODERATE, HIGH);
      digitalWrite(PIN_LOUD, HIGH);
    } else {
      digitalWrite(PIN_QUIET, LOW);
      digitalWrite(PIN_MODERATE, LOW);
      digitalWrite(PIN_LOUD, LOW);
    }
    delay(50);
  }
  
  if (db <= 60)
  {
    lcd.setCursor(0, 1);
    lcd.print("Level: Sunyi");
    digitalWrite(PIN_QUIET, HIGH);
    digitalWrite(PIN_MODERATE, LOW);
    digitalWrite(PIN_LOUD, LOW);
  }
  else if (db > 60 && db<85)
  {
    lcd.setCursor(0, 1);
    lcd.print("Level: Sedang");
    digitalWrite(PIN_QUIET, LOW);
    digitalWrite(PIN_MODERATE, HIGH);
    digitalWrite(PIN_LOUD, LOW);
  }
  else if (db>=85)
  {
    lcd.setCursor(0, 1);
    lcd.print("Level: Tinggi");
    digitalWrite(PIN_QUIET, LOW);
    digitalWrite(PIN_MODERATE, LOW);
    digitalWrite(PIN_LOUD, HIGH);
 
  }
 
  server.handleClient(); 

   delay(1000);      
   lcd.clear();
}

void publishToMQTT(float dB) {
  if (mqttClient.publish(MQTT_TOPIC, String(dB).c_str())) {
    Serial.println("Published to MQTT: " + String(dB) + " dB");
  } else {
    Serial.println("Failed to publish to MQTT");
  }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  // Handle MQTT messages if needed
}

void connectToWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi terhubung");
    delay(4000);
    lcd.clear();
}

void reconnectMQTT() {
  while (!mqttClient.connected()) {
    Serial.println("Connecting to MQTT...");
    String clientId = "ESP8266Client-" + String(random(0xffff), HEX);
    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("Connected to MQTT");
    } else {
      Serial.print("MQTT connection failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
}
