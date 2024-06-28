#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "config.h"

WiFiClient espClient;
PubSubClient client(espClient);
String clientId = String(ESP.getChipId());

int  delayMS = 30000;  // Задаержка в мс между публикацией сообщений
long lastMsg = 0;      // Время публикации предыдущего сообщения  (мс) 
int  value = 0;        // Переменная для формирования публикуемого сообщения

const int levelSensorPin = 4; // the number of the level sensor pin
// variable for storing the level sensor status
int levelState = 0;


// Функция обработки входящих сообщений
void callback(char* topic, byte* payload, unsigned int length) {
  // Печать информации о полученном сообщенийй
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Если получено сообщение и у нас подписка на несколько топиков, то определяем в каком топике сообщение опубликовано
  if (strcmp(topic, mqtt_topic_in) == 0) { 
    // Определяем поведение MCU при различных значениях сообщения (payload)
    if ((char)payload[0] == '0') {
      digitalWrite(BUILTIN_LED, HIGH); // BUILTIN_LED имеет подтягивающий резистор, HIGH = OFF, LOW = ON
    }
    if ((char)payload[0] == '1') {
      digitalWrite(BUILTIN_LED, LOW);
    }
  }
}

void readLevelSensorStatus(){
  
}

void setup() {
  // put your setup code here, to run once:
  pinMode(BUILTIN_LED, OUTPUT);     // Установка BUILTIN_LED как порт вывода
  digitalWrite(BUILTIN_LED, HIGH);  // BUILTIN_LED имеет подтягивающий резистор, HIGH = OFF, LOW = ON
  // initialize the level sensor pin as an input
  pinMode(levelSensorPin, INPUT);

  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Публикация сообщения с заданной периодичностью
  long now = millis();
  if (now - lastMsg > delayMS) {
    lastMsg = now;
    ++value;

    // Формирование сообщения и его публикация 
    char msg[200];
    snprintf (msg, sizeof(msg), "heartbeat #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(mqtt_topic_status, msg);

    // read the state of the level sensor value
    levelState = digitalRead(levelSensorPin);
 
    if (levelState == HIGH) {
    // turn LED on
       Serial.print("Level high");
       client.publish(mqtt_topic_out, "Level high");
    } else {
    // turn LED off
      Serial.print("Level low");
      client.publish(mqtt_topic_out, "Level low");
    }
    
  }
}
