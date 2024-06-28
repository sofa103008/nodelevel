//
// config.h
//

// Настройки WiFi
#define wifi_ssid     "OpenWrt1"
#define wifi_pass     "1234567890"

// Сервер MQTT
#define mqtt_server   "192.168.2.198"
#define mqtt_port     1883
#define mqtt_login    NULL
#define mqtt_pass     NULL

// MQTT топики
#define mqtt_topic_status  "tank1/status"
#define mqtt_topic_out     "tank1/level_out"
#define mqtt_topic_in      "tank1/level_in"
    