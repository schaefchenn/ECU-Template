#include <CAN.h>

#define TX_GPIO_NUM   17  // Connects to CTX
#define RX_GPIO_NUM   16  // Connects to CRX


//==================================================================================//

void setupCANBUS() {
  Serial.println ("CAN Receiver/Receiver");

  // Set the pins
  CAN.setPins (RX_GPIO_NUM, TX_GPIO_NUM);

  // start the CAN bus at 500 kbps
  if (!CAN.begin (1E6)) {
    Serial.println ("Starting CAN failed!");
    while (1);
  }
  else {
    Serial.println ("CAN Initialized");
  }
}


//==================================================================================//

void canSender() {
  Serial.print("Sending packet ... ");

  CAN.beginPacket(0x12);  // Sets the ID and clears the transmit buffer

  int8_t val1 = 1; // Example negative 8-bit value
  int16_t val2 = -2000; // Example negative 16-bit value
  int8_t val3 = 0; // Example 8-bit value

  CAN.write(val1); // Write 1 byte

  // Break val2 into two bytes and write them
  CAN.write((uint8_t)(val2 >> 8)); // High byte
  CAN.write((uint8_t)(val2 & 0xFF)); // Low byte

  CAN.write(val3); // Write 1 byte

  CAN.endPacket();

  Serial.println("done");
}

void canReceiver() {
  // try to parse packet
  int packetSize = CAN.parsePacket();

  if (packetSize) {
    // received a packet
    Serial.print ("Received ");

    if (CAN.packetExtended()) {
      Serial.print ("extended ");
    }

    if (CAN.packetRtr()) {
      // Remote transmission request, packet contains no data
      Serial.print ("RTR ");
    }

    Serial.print ("packet with id 0x");
    Serial.print (CAN.packetId(), HEX);

    if (CAN.packetRtr()) {
      Serial.print (" and requested length ");
      Serial.println (CAN.packetDlc());
    } else {
      Serial.print (" and length ");
      Serial.println (packetSize);

      // only print packet data for non-RTR packets
      while (CAN.available()) {
        Serial.print ((char) CAN.read());
      }
      Serial.println();
    }

    Serial.println();
  }
}