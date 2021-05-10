#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "";
const char* password = "";

String response = "";

int LED_MERAH = 23;
int LED_KUNING = 22;
int LED_PUTIH = 1;

DynamicJsonDocument doc(2048);

unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 1 * 60 * 1000;

void setup() {
  Serial.begin(115200);

  pinMode(LED_MERAH, OUTPUT);
  pinMode(LED_KUNING, OUTPUT);
  pinMode(LED_PUTIH, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("WiFi connected with IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  HTTPClient http;
  currentTime = millis();

  if (currentTime - previousTime > timeoutTime){
    Serial.println("Send request");
    previousTime = currentTime;

    
    String request = "https://api.tomorrow.io/v4/timelines?location=-7.8116789,110.3465946,21&timezone=Asia/Bangkok&fields=temperature&timesteps=current&units=metric&apikey=pzhslEPAAAS8UIxh7eHAFT3i4Fgfjcw2";

    http.begin(request);

    http.GET();

    response = http.getString();

    DeserializationError error = deserializeJson(doc, response);
    if(error){
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }
    JsonObject data_timelines_0 = doc["data"]["timelines"][0];
    const char* data_timelines_0_intervals_0_startTime = data_timelines_0["intervals"][0]["startTime"];
    float data_timelines_0_intervals_0_values_temperature = data_timelines_0["intervals"][0]["values"]["temperature"];

    Serial.print("Waktu: ");
    Serial.println(data_timelines_0_intervals_0_startTime);
    Serial.print("Temperature: ");
    Serial.println(data_timelines_0_intervals_0_values_temperature);

    //Level 1
    if(data_timelines_0_intervals_0_values_temperature > 30){
      Serial.println("Level: Panas");
      digitalWrite(LED_MERAH, HIGH);
      digitalWrite(LED_KUNING, LOW);
      digitalWrite(LED_PUTIH, LOW);
      Serial.println("Suhu AC yang pas ada diantara 18-22 Derajat Celcius.");
    }else if(data_timelines_0_intervals_0_values_temperature <= 30 && data_timelines_0_intervals_0_values_temperature >= 25){
      Serial.println("Level: Sedang");
      digitalWrite(LED_MERAH, LOW);
      digitalWrite(LED_KUNING, HIGH);
      digitalWrite(LED_PUTIH, LOW);
      Serial.println("Suhu AC yang pas ada diantara 20-25 Derajat Celcius.");
    }else if(data_timelines_0_intervals_0_values_temperature < 25){
      Serial.println("Level: Dingin");
      digitalWrite(LED_MERAH, LOW);
      digitalWrite(LED_KUNING, LOW);
      digitalWrite(LED_PUTIH, HIGH);
      Serial.println("Suhu AC yang pas ada diantara 27-32 Derajat Celcius.");
    }
    
    http.end();
  }

  }
