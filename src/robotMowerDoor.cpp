/* 
 * Project: Robot Mower Door
 * Author: Isaac Martinez
 * Date: 4-11-2024
 */

#include "Particle.h"
#include <DFRobot_PN532.h>

#define BLOCK_SIZE       16
#define  PN532_IRQ        2
#define  INTERRUPT        1
#define  POLLING          0
#define  READ_BLOCK_NO    2
int speakerPin =          D16; // The pin connected to the speaker
int lockPin =         D8; // The pin connected to the lock

DFRobot_PN532_IIC  nfc(PN532_IRQ, POLLING);
uint8_t dataRead[16] = {0};

SYSTEM_MODE(SEMI_AUTOMATIC);

void setup() {
  Serial.begin(9600);
  delay(2000);
  Serial.print("Initializing");
  while (!nfc.begin()) {
    Serial.print(".");
    delay (1000);
  }
  Serial.println();
  Serial.println("Waiting for a card......");

  pinMode(speakerPin, OUTPUT); // Set the speaker pin as output
  pinMode(lockPin, OUTPUT); // Set the lock pin as output
  digitalWrite(lockPin, HIGH); // Initially, the lock is locked
}

void loop() {
  if (nfc.scan()) {
    if (nfc.readData(dataRead, READ_BLOCK_NO) != 1) {
      Serial.print("Block ");
      Serial.print(READ_BLOCK_NO);
      Serial.println(" read failure!");
    }
    else {
      Serial.print("Block ");
      Serial.print(READ_BLOCK_NO);
      Serial.println(" read success!");

      Serial.print("Data read(string):");
      Serial.println((char *)dataRead);

      // If the read data is "Hello World", play a sound and unlock the lock
      if (strcmp((char *)dataRead, "Hello World !") == 0) {
        Serial.println("Playing tone");
        analogWrite(speakerPin, 128, 500); // Play a 20Hz tone
        delay(2000);
        analogWrite(speakerPin, 0, 500); // Stop the tone
        digitalWrite(lockPin, LOW); // Unlock the lock
      }

      Serial.print("Data read(HEX):");
      for (int i = 0; i < BLOCK_SIZE; i++) {
        Serial.print(dataRead[i], HEX);
        Serial.print(" ");
        dataRead[i] = 0;
      }
      Serial.println();
    }
    delay(500);
  }
}