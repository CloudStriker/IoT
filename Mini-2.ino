#include <WiFi.h>
#include <MQTT.h>
#include <ESP32Servo.h>

const char ssid[] = "XperiaXCompact";
const char pass[] = "12345678";

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

int ldr = 34, batas = 30, val, valAkhir, i, valueBatas;
char stringLux[10], valBatas[10];
Servo servo23;
bool terbuka = false;

void lux(){
  val = analogRead(ldr);
  valAkhir = 0.009768*val+10;
}

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("ClickNowBoard", "clicknow", "SmrW2FHwiMQMGw63")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("/hello");
  client.subscribe("/batas");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  if(topic == "/batas"){
    batas = payload.toInt();
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  servo23.attach(23);
  
  client.begin("public.cloud.shiftr.io", net);
  client.onMessage(messageReceived);

  connect();
}

void loop() {
  client.loop();
  delay(1000);  // <- fixes some issues with WiFi stability
  
  lux();
  
  if (!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    Serial.println(valAkhir);
    client.publish("/hello", itoa(valAkhir, stringLux, 10));
    
    if(!terbuka){
      if(valAkhir >= batas){
        for(i=0; i<=180; i++){
          servo23.write(i);
          Serial.print("Terbuka");
          if(i%3==0)Serial.print(".");
            delay(25);
        }
        Serial.println(batas);
        terbuka = true;
      }
    }else{
      if(valAkhir <= batas){
        for(i=180; i>0; i--){
          servo23.write(i);
          Serial.print("Tertutup");
          if(i%3==0)Serial.print(".");
            delay(25);
        }
        Serial.println(batas);
        terbuka = false;
      }
    }
  }
}
