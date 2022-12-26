// #include <Arduino.h>
// // #include <Wire.h>
// // #include <SPI.h>
// #include "secrets.h"
// // #include <HTTPClient.h>
// // #include <EEPROM.h>
// #include <vector>
// #include <cstdint>
// #include <string>
// #include <vector>



// using byte_array = std::vector<uint8_t>;

// #include <string>

// using byte_array = std::vector<uint8_t>;

// class EthereumTransaction {
//   public:
//     uint64_t nonce;
//     uint64_t gasPrice;
//     uint64_t gasLimit;
//     uint64_t chainId;
//     std::string to;
//     uint64_t value;
//     std::string data;
//     uint8_t v;
//     byte_array r;
//     byte_array s;
//     byte_array rlp;

//     // Define the rlp_encode and sign methods for the EthereumTransaction class.
//     byte_array rlp_encode();
//     void sign(byte_array privateKey);
// };
// byte_array EthereumTransaction::rlp_encode() {
//   // The RLP-encoded data will be stored in this buffer.
//   byte_array buffer;

//   // Encode the nonce as a single-byte integer.
//   if (nonce < 0x80) {
//     buffer.push_back((uint8_t)nonce);
//   } else {
//     // The nonce is too large to fit in a single byte.
//     // Encode it as a multi-byte integer.
//     buffer.push_back(0x82);
//     buffer.push_back((nonce >> 8) & 0xff);
//     buffer.push_back(nonce & 0xff);
//   }

//   // Encode the gas price as a single-byte integer.
//   if (gasPrice < 0x80) {
//     buffer.push_back((uint8_t)gasPrice);
//   } else {
//     // The gas price is too large to fit in a single byte.
//     // Encode it as a multi-byte integer.
//     buffer.push_back(0x82);
//     buffer.push_back((gasPrice >> 8) & 0xff);
//     buffer.push_back(gasPrice & 0xff);
//   }

//   // Encode the gas limit as a single-byte integer.
//   if (gasLimit < 0x80) {
//     buffer.push_back((uint8_t)gasLimit);
//   } else {
//     // The gas limit is too large to fit in a single byte.
//     // Encode it as a multi-byte integer.
//     buffer.push_back(0x82);
//     buffer.push_back((gasLimit >> 8) & 0xff);
//     buffer.push_back(gasLimit & 0xff);
//   }

//   // Encode the recipient address as a string.
//   buffer.push_back(to.length());
//   for (size_t i = 0; i < to.length(); i++) {
//     buffer.push_back(to[i]);
//   }

//   // Encode the value as a single-byte integer.
//   if (value < 0x80) {
//     buffer.push_back((uint8_t)value);
//   } else {
//     // The value is too large to fit in a single byte.
//     // Encode it as a multi-byte integer.
//     buffer.push_back(0x82);
//     buffer.push_back((value >> 8) & 0xff);
//     buffer.push_back(value & 0xff);
//   }

//   // Encode the data as a string.
//   buffer.push_back(data.length());
//   for (size_t i = 0; i < data.length(); i++) {
//     buffer.push_back(data[i]);
//   }

//   // Encode the signature as a list of 3 elements.
//   buffer.push_back(0xc3);
//   buffer.push_back(v);
//   buffer.push_back(0x82);
//   buffer.push_back((r.size() >> 8) & 0xff);
//   buffer.push_back(r.size() & 0xff);
//   for (size_t i = 0; i < r.size(); i++) {
//     buffer.push_back(r[i]);
//   }
//   buffer.push_back(0x82);
//   buffer.push_back((s.size() >> 8) & 0xff);
//   buffer.push_back(s.size() & 0xff);
//   for (size_t i = 0; i < s.size(); i++) {
//     buffer.push_back(s[i]);
//   }

//   // Save the RLP-encoded data in the transaction object.
//   rlp = buffer;

//   // Return the RLP-encoded data.
//   return buffer;
// }

// #include "IoTeX-blockchain-client.h"
// std::vector<uint8_t> from_hex(const std::string& hex) {
//   std::vector<uint8_t> bytes;

//   if (hex.length() % 2 != 0) {
//     return {};
//   }

//   for (size_t i = 0; i < hex.length(); i += 2) {
//     uint8_t value = 0;

//     if (hex[i] >= '0' && hex[i] <= '9') {
//       value += (hex[i] - '0') * 16;
//     } else if (hex[i] >= 'a' && hex[i] <= 'f') {
//       value += (hex[i] - 'a' + 10) * 16;
//     } else {
//       return {};
//     }

//     if (hex[i + 1] >= '0' && hex[i + 1] <= '9') {
//       value += (hex[i + 1] - '0');
//     } else if (hex[i + 1] >= 'a' && hex[i + 1] <= 'f') {
//       value += (hex[i + 1] - 'a' + 10);
//     } else {
//       return {};
//     }

//     bytes.push_back(value);
//   }

//   return bytes;
// }

// template <typename T>
// std::vector<uint8_t> to_bytes(T value) {
//   std::vector<uint8_t> bytes;

//   for (size_t i = 0; i < sizeof(T); ++i) {
//     bytes.push_back((value >> (i * 8)) & 0xff);
//   }

//   return bytes;
// }

// std::vector<uint8_t> _serialize(const EthereumTransaction& transaction) {
//   std::vector<std::vector<uint8_t>> raw;

//   // Add the transaction fields to the raw array
//   raw.push_back(to_bytes(transaction.nonce));
//   raw.push_back(to_bytes(transaction.gasPrice));
//   raw.push_back(to_bytes(transaction.gasLimit));
//   raw.push_back(from_hex(transaction.to));
//   raw.push_back(to_bytes(transaction.value));
//   raw.push_back(from_hex(transaction.data));

// // loop over transaction.nonce
// auto nonce = transaction.nonce;
// auto gasPrice = transaction.gasPrice;
// auto gasLimit = transaction.gasLimit;
// auto to = transaction.to;
// auto value = transaction.value;
// auto data = transaction.data;
// Serial.println("nonce:");
// for (int i = 0; i < sizeof(nonce); ++i) {
//     Serial.print((nonce >> (i * 8)) & 0xff, HEX);
//     Serial.print(" ");
// }
// Serial.println("==========");
// for (int i = 0; i < sizeof(gasPrice); ++i) {
//     Serial.print((gasPrice >> (i * 8)) & 0xff, HEX);
//     Serial.print(" ");
// }

// Serial.println("==========");
// for (int i = 0; i < sizeof(gasLimit); ++i) {
//     Serial.print((gasLimit >> (i * 8)) & 0xff, HEX);
//     Serial.print(" ");
// }

// Serial.println("==========");
// for (int i = 0; i < to.length(); i++) {
//     Serial.print(to[i], HEX);
//     Serial.print(" ");
// }
// Serial.println("==========");
// for (int i = 0; i < sizeof(value); ++i) {
//     Serial.print((value >> (i * 8)) & 0xff, HEX);
//     Serial.print(" ");
// }

// Serial.println("==========");
// for (int i = 0; i < data.length(); i++) {
//     Serial.print(data[i], HEX);
//     Serial.print(" ");
// }







//   // Check for a chain ID
//   uint64_t chainId = 0;
//   if (transaction.chainId) {
//     chainId = transaction.chainId;
//   }

//   // If a chain ID was specified or derived, add it to the raw array
//   if (chainId) {
//     raw.push_back(to_bytes(chainId));
//     raw.push_back({});
//     raw.push_back({});
//   }
    
//   Serial.println("==========");
//   // print chainId
  

//   // Return the RLP-encoded raw array
//     //   return rlp_encode(raw);
//     // debug print all raw
//     Serial.println("RAW:");
//     for (int i = 0; i < raw.size(); i++) {
//         for (int j = 0; j < raw[i].size(); j++) {
//             Serial.print(raw[i][j], HEX);
//             Serial.print(" ");
//         }
//         Serial.println();
//     }
//     return {};
// }

// void setup()
// {
//   Serial.begin(115200);
//   while (!Serial)
//   {
//     delay(1000);
//   }
//   Serial.println();
//   Serial.println();
//   Serial.println();
//   Serial.println();
//   Serial.println();
//   Serial.println();
//   Serial.println();
//   Serial.println();
//   Serial.println();
// EthereumTransaction tx;

// tx.nonce = 2;
// tx.gasPrice = 100000000000;
// tx.gasLimit = 4 * 21000;
// tx.chainId = 170;
// tx.to = "0x70997970C51812dc3A010C7d01b50e0d17dc79C8";
// tx.value = 1000000000000000000;
// tx.data = "0x68656c6c6f20776f726c6421";

// auto serialized = _serialize(tx);
//   // Encode the transaction using RLP.
//   byte_array rlp = tx.rlp_encode();


//   Serial.println("RLP DEBUG:");
//   // debug rlp data
//   for (int i = 0; i < rlp.size(); i++)
//   {
//     Serial.print(rlp[i], HEX);
//     Serial.print(" ");
//   }
//   Serial.println("---------");

// }

// void loop()
// {
//   // Private key of the origin address
//   const char pkString[] = SECRET_PRIVATE_KEY;

//   // Convert the private key and address hex strings to byte arrays
//   uint8_t pkBytes[IOTEX_PRIVATE_KEY_SIZE];
//   signer.str2hex(pkString, pkBytes, IOTEX_SIGNATURE_SIZE);

//   // Message to sign as a byte array
//   // const uint8_t message[] = {0xab, 0xcd};
//   // 0xf83b0285174876e800830148209470997970c51812dc3a010c7d01b50e0d17dc79c889056bc75e2d631000008c68656c6c6f20776f726c642181aa8080
//   // f83b0285174876e800830148209470997970c51812dc3a010c7d01b50e0d17dc79c889056bc75e2d631000008c68656c6c6f20776f726c642181aa8080
//   // const String messageString = "f83b0285174876e800830148209470997970c51812dc3a010c7d01b50e0d17dc79c889056bc75e2d631000008c68656c6c6f20776f726c642181aa8080";
//   const uint8_t message[] = {0xf8, 0x3b, 0x02, 0x85, 0x17, 0x48, 0x76, 0xe8, 0x00, 0x83, 0x01, 0x48, 0x20, 0x94, 0x70, 0x99, 0x79, 0x70, 0xc5, 0x18, 0x12, 0xdc, 0x3a, 0x01, 0x0c, 0x7d, 0x01, 0xb5, 0x0e, 0x0d, 0x17, 0xdc, 0x79, 0xc8, 0x89, 0x05, 0x6b, 0xc7, 0x5e, 0x2d, 0x63, 0x10, 0x00, 0x00, 0x8c, 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x21, 0x81, 0xaa, 0x80, 0x80};
//   // const uint8_t message[] = { 0x0f, 0x83, 0xb0, 0x28, 0x51, 0x74, 0x87, 0x6e, 0x80, 0x00, 0x83, 0x01, 0x48, 0x20, 0x94, 0x70, 0x99, 0x79, 0x70, 0xc5, 0x18, 0x12, 0xdc, 0x3a, 0x01, 0x0c, 0x7d, 0x01, 0xb5, 0x0e, 0x0d, 0x17, 0xdc, 0x79, 0xc8, 0x89, 0x05, 0x6b, 0xc7, 0x5e, 0x2d, 0x63, 0x10, 0x00, 0x00, 0x8c, 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x21, 0x81, 0xaa, 0x80, 0x80};
//   uint8_t payloadhash[IOTEX_HASH_SIZE] = {0};

//   Serial.println("Message: ");
//   for (int i = 0; i < sizeof(message); i++)
//   {
//     Serial.printf("%02x", message[i]);
//   }

//   Serial.println();
//   // get hash
//   signer.getHash(message, sizeof(message), payloadhash);

//   Serial.println("HASH: ");
//   for (int i = 0; i < IOTEX_HASH_SIZE; i++)
//   {
//     Serial.printf("%02x", payloadhash[i]);
//   }
//   Serial.println();

//   // Sign the message
//   uint8_t signature[IOTEX_SIGNATURE_SIZE] = {0};
//   // sign hash
//   signer.signHash(payloadhash, pkBytes, signature);

//   // signer.signMessage(message, sizeof(message), pkBytes, signature);

//   // // Print the signature
//   Serial.print("Signature: ");
//   for (int i = 0; i < IOTEX_SIGNATURE_SIZE; i++)
//   {
//     char buf[3] = "";
//     sprintf(buf, "%02x", signature[i]);
//     Serial.print(buf);
//   }
//   Serial.println();

//   Serial.println("Program finished");
//   Serial.println();
//   Serial.println();
//   Serial.println();
//   Serial.println();
//   Serial.println();
//   Serial.println();
//   Serial.println();
//   Serial.println();
//   Serial.println();
//   while (true)
//   {
//     // deep sleep 10 seconds

//     ESP.deepSleep(1000000 * 5);

//     delay(1000);
//   }
// }