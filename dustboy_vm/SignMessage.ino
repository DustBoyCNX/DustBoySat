// #include <Arduino.h>
// // #include <Wire.h>
// // #include <SPI.h>
// #include "secrets.h"
// // #include <HTTPClient.h>
// // #include <EEPROM.h>
// #include "IoTeX-blockchain-client.h"

// void setup()
// {
//     Serial.begin(115200);
//     while (!Serial)
//     {
//         delay(1000);
//     }
//     Serial.println();
//     Serial.println();
//     Serial.println();
//     Serial.println();
//     Serial.println();
//     Serial.println();
//     Serial.println();
//     Serial.println();
//     Serial.println();
// }

// void loop()
// {
//     // Private key of the origin address
//     const char pkString[] = SECRET_PRIVATE_KEY;

//     // Convert the private key and address hex strings to byte arrays
//     uint8_t pkBytes[IOTEX_PRIVATE_KEY_SIZE];
//     signer.str2hex(pkString, pkBytes, IOTEX_SIGNATURE_SIZE);

//     // Message to sign as a byte array
//     // const uint8_t message[] = {0xab, 0xcd};
//     // 0xf83b0285174876e800830148209470997970c51812dc3a010c7d01b50e0d17dc79c889056bc75e2d631000008c68656c6c6f20776f726c642181aa8080
//     // f83b0285174876e800830148209470997970c51812dc3a010c7d01b50e0d17dc79c889056bc75e2d631000008c68656c6c6f20776f726c642181aa8080
//     // const String messageString = "f83b0285174876e800830148209470997970c51812dc3a010c7d01b50e0d17dc79c889056bc75e2d631000008c68656c6c6f20776f726c642181aa8080";
//     const uint8_t message[] = {0xf8, 0x3b, 0x02, 0x85, 0x17, 0x48, 0x76, 0xe8, 0x00, 0x83, 0x01, 0x48, 0x20, 0x94, 0x70, 0x99, 0x79, 0x70, 0xc5, 0x18, 0x12, 0xdc, 0x3a, 0x01, 0x0c, 0x7d, 0x01, 0xb5, 0x0e, 0x0d, 0x17, 0xdc, 0x79, 0xc8, 0x89, 0x05, 0x6b, 0xc7, 0x5e, 0x2d, 0x63, 0x10, 0x00, 0x00, 0x8c, 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x21, 0x81, 0xaa, 0x80, 0x80};
//     // const uint8_t message[] = { 0x0f, 0x83, 0xb0, 0x28, 0x51, 0x74, 0x87, 0x6e, 0x80, 0x00, 0x83, 0x01, 0x48, 0x20, 0x94, 0x70, 0x99, 0x79, 0x70, 0xc5, 0x18, 0x12, 0xdc, 0x3a, 0x01, 0x0c, 0x7d, 0x01, 0xb5, 0x0e, 0x0d, 0x17, 0xdc, 0x79, 0xc8, 0x89, 0x05, 0x6b, 0xc7, 0x5e, 0x2d, 0x63, 0x10, 0x00, 0x00, 0x8c, 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x21, 0x81, 0xaa, 0x80, 0x80};
//     uint8_t payloadhash[IOTEX_HASH_SIZE] = {0};

//     Serial.println("Message: ");
//     for (int i = 0; i < sizeof(message); i++)
//     {
//         Serial.printf("%02x", message[i]);
//     }
    
//     Serial.println();
//     // get hash
//     signer.getHash(message, sizeof(message), payloadhash);

//     Serial.println("HASH: ");
//     for (int i = 0; i < IOTEX_HASH_SIZE; i++)
//     {
//         Serial.printf("%02x", payloadhash[i]);
//     }
//     Serial.println();

//     // Sign the message
//     uint8_t signature[IOTEX_SIGNATURE_SIZE] = {0};
//     // sign hash
//     signer.signHash(payloadhash, pkBytes, signature);

//     // signer.signMessage(message, sizeof(message), pkBytes, signature);

//     // // Print the signature
//     Serial.print("Signature: ");
//     for (int i = 0; i < IOTEX_SIGNATURE_SIZE; i++)
//     {
//         char buf[3] = "";
//         sprintf(buf, "%02x", signature[i]);
//         Serial.print(buf);
//     }
//     Serial.println();

//     Serial.println("Program finished");
//     Serial.println();
//     Serial.println();
//     Serial.println();
//     Serial.println();
//     Serial.println();
//     Serial.println();
//     Serial.println();
//     Serial.println();
//     Serial.println();
//     while (true)
//     {
//         // deep sleep 10 seconds

//         ESP.deepSleep(1000000 * 5);

//         delay(1000);
//     }
// }