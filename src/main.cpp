#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

typedef struct struct_message {
    int x;
    int y;
} struct_message;


const int joyX = 1;
const int joyY = 2;
uint8_t remoteAddress[] = {0x58, 0xCF, 0x79, 0xD7, 0x2C, 0x5C};
struct_message myData = {0, 0};
const int actualCenter = 2300;


void setup() {
    Serial.begin(9600);

    pinMode(joyX, INPUT);
    pinMode(joyY, INPUT);

    WiFiClass::mode(WIFI_STA);

    if (esp_now_init() != ESP_OK) {
        return;
    }

    // ESP-NOW peer information
    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, remoteAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    // Add peer
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        return;
    }
}

void loop() {
    // 179 accounts for some weird behavior with the bounds on the joystick
    myData.x = constrain((analogRead(joyX) - actualCenter) / 10, -179, 179);
    myData.y = constrain((analogRead(joyY) - actualCenter) / 10, -179, 179);

//    Serial.printf("x: %d\ty:%d\n", myData.x, myData.y);

    esp_err_t result = esp_now_send(remoteAddress, (uint8_t *) &myData, sizeof(myData));
}