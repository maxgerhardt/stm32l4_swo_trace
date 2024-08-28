#include <Arduino.h>
#define LED LED_BUILTIN

extern void setup_trace();

void setup() {
    Serial.begin(115200);
    pinMode(LED, OUTPUT);
    setup_trace();
    Serial.println("Trace enabled! Clock speed is " + String(SystemCoreClock));
}

void loop() {
    digitalWrite(LED, HIGH);
    delay(1000);
    digitalWrite(LED, LOW);
    delay(1000);
}