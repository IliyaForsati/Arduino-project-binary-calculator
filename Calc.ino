#include "parser.h"
void setup() {
        Serial.begin(9600);
        pinMode(BUILTIN_LED, OUTPUT);
}

String current_str = "";

void sendData(String data) {
        Serial.print("R<");
        Serial.print(data);
        Serial.println(">");
}

void loop() {

        if (Serial.available()) {
                String recivedData = Serial.readString();
                if (recivedData.startsWith("K<")) {
                        String key = recivedData.substring(2, recivedData.length() - 2); // Extract the key text
                        current_str = current_str + key;
                        sendData(current_str);

                        digitalWrite(BUILTIN_LED, LOW);
                        delay(500);
                        digitalWrite(BUILTIN_LED, HIGH);
                        delay(500);
                        digitalWrite(BUILTIN_LED, LOW);

                }
        }
}

