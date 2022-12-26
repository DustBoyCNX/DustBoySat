#include <Arduino.h>

#define MAX_HEX_LENGTH 256
#define MAX_BYTE_ARRAY_LENGTH 32

const char HexCharacters[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

char *hexlify(const unsigned char *value, int valueLength) {
    char *hex = (char *)malloc(MAX_HEX_LENGTH);
    memset(hex, 0, MAX_HEX_LENGTH);
    strcpy(hex, "0x");

    for (int i = 0; i < valueLength; i++) {
        char byteHex[3];
        memset(byteHex, 0, 3);
        sprintf(byteHex, "%02x", value[i]);
        strcat(hex, byteHex);
    }

    return hex;
}

void setup() {
    Serial.begin(115200);

    // unsigned char value[MAX_BYTE_ARRAY_LENGTH] = { 0x30, 0x39, 0x32, 0x35, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30 };
    unsigned char testData[MAX_BYTE_ARRAY_LENGTH] = {
        239,   1, 133,  23,  72, 118, 232,   0, 131,  1,
        72,  32, 148, 112, 153, 121, 112, 197,  24, 18,
        220,  58,   1,  12, 125,   1, 181,  14,  13, 23,
        220, 121, 200, 137,   5, 107, 199,  94,  45, 99,
        16,   0,   0, 128, 129, 255, 128, 128 };
    int testDataLength = 48;
    int valueLength = 32;
    // char *hex = hexlify(value, valueLength);
    char *hex = hexlify(testData, testDataLength);
    Serial.println(hex);
    // 0xef01851 7729be 8008370 14870499170c511612c00c7d008d0e0d17dc79c88957c75e2d6          310010000 8181ff8080
    // 0xef01851 74876e 800830  148209470997970c51812dc3a010c7d01b50e0d17dc79c889056bc75e2d6 310000080   81ff8080
    free(hex);
}

void loop() {}


// #include <Arduino.h>

// #include <vector>

// #include <Arduino.h>

// std::vector<uint8_t> arrayifyInteger(int value) {
//   std::vector<uint8_t> result;
//   while (value) {
//     result.insert(result.begin(), value & 0xff);
//     value >>= 8;
//   }
//   return result;
// }

// int unarrayifyInteger(const std::vector<uint8_t> &data, int offset, int length) {
//   int result = 0;
//   for (int i = 0; i < length; i++) {
//     result = result * 256 + data[offset + i];
//   }
//   return result;
// }

// std::vector<uint8_t> _encode(const std::vector<uint8_t> &object) {
//   std::vector<uint8_t> payload;

//   for (const auto &child : object) {
//     std::vector<uint8_t> childPayload = _encode(child);
//     payload.insert(payload.end(), childPayload.begin(), childPayload.end());
//   }

//   if (payload.size() <= 55) {
//     payload.insert(payload.begin(), 0xc0 + payload.size());
//     return payload;
//   }

//   std::vector<uint8_t> length = arrayifyInteger(payload.size());
//   length.insert(length.begin(), 0xf7 + length.size());

//   std::vector<uint8_t> result;
//   result.insert(result.end(), length.begin(), length.end());
//   result.insert(result.end(), payload.begin(), payload.end());
//   return result;
// }

// std::vector<uint8_t> _encode(const std::string &object) {
//   std::vector<uint8_t> data(object.begin(), object.end());

//   if (data.size() === 1 && data[0] <= 0x7f) {
//     return data;
//   } else if (data.size() <= 55) {
//     data.insert(data.begin(), 0x80 + data.size());
//     return data;
//   }

//   std::vector<uint8_t> length = arrayifyInteger(data.size());
//   length.insert(length.begin(), 0xb7 + length.size());

//   std::vector<uint8_t> result;
//   result.insert(result.end(), length.begin(), length.end());
//   result.insert(result.end(), data.begin(), data.end());
//   return result;
// }

// const uint64_t object[] = {
//   0x02,
//   0x174876e800,
//   0x014820,
//   0x70997970c51812dc3a010c7d01b50e0d17dc79c8,
//   0x056bc75e2d63100000,
//   0x68656c6c6f20776f726c6421,
//   0xaa,
//   0x00,
//   0x00
// };

// void setup() {
//   Serial.begin(115200);
// }

// void loop() {
//   // Do something with the object here...
// }











// #include <sstream>
// #include <iomanip>
// #include <array>
// #include <vector>


// typedef struct {
//   uint32_t nonce;
//   uint32_t gasPrice;
//   uint32_t gasLimit;
//   uint8_t to[20];
//   uint32_t value;
//   uint8_t data[256];
// //   size_t dataLength;
//   uint32_t chainId;
// } EthereumTransaction;

// std::vector<uint8_t> serializeAndHexifyTransaction(const EthereumTransaction& tx) {
//   std::vector<uint8_t> serialized;

//   // Serialize the nonce field
//   uint32_t nonce = tx.nonce;
//   for (int i = 0; i < 32; i += 8) {
//     serialized.push_back((nonce >> i) & 0xff);
//   }

//   // Serialize the gasPrice field
//   uint32_t gasPrice = tx.gasPrice;
//   for (int i = 0; i < 32; i += 8) {
//     serialized.push_back((gasPrice >> i) & 0xff);
//   }

//   // Serialize the gasLimit field
//   uint32_t gasLimit = tx.gasLimit;
//   for (int i = 0; i < 32; i += 8) {
//     serialized.push_back((gasLimit >> i) & 0xff);
//   }

// //   // Serialize the to field
// //   for (const uint8_t& byte : tx.to) {
// //     serialized.push_back(byte);
// //   }

// //   // Serialize the value field
// //   uint32_t value = tx.value;
// //   for (int i = 0; i < 32; i += 8) {
// //     serialized.push_back((value >> i) & 0xff);
// //   }

// //   // Serialize the data field
// //     for (const uint8_t& byte : tx.data) {
// //         serialized.push_back(byte);
// //     }


// //   // Convert the serialized data to a hexadecimal string
//   std::stringstream hexString;
//   hexString << std::hex << std::setfill('0');
//   for (const uint8_t& byte : serialized) {
//     hexString << std::setw(2) << static_cast<int>(byte);
//   }

//   // Return the hexadecimal string as a vector of bytes
//   return std::vector<uint8_t>(hexString.str().begin(), hexString.str().end());
// }

// #include <Arduino.h>

// void setup() {
//     Serial.begin(115200);

// Serial.println("hello");

// EthereumTransaction tx;
// tx.nonce = 5;
// tx.gasPrice = 10;
// tx.gasLimit = 21000;
// // tx.to = {0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef, 0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef, 0x12, 0x34, 0x56, 0x78};
// // set {0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef, 0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef, 0x12, 0x34, 0x56, 0x78}; to tx.to
// uint8_t to[20] = {0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef, 0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef, 0x12, 0x34, 0x56, 0x78};
// memcpy(tx.to, to, 20);
// tx.value = 100;
// // tx.data = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
// //            0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20,
// //            // ...
// //            0xef, 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff};
// tx.chainId = 1;

//     std::vector<uint8_t> serializedAndHexified = serializeAndHexifyTransaction(tx);

//     // Print the serialized and hexified transaction
//     for (const uint8_t& byte : serializedAndHexified) {
//         Serial.print(static_cast<char>(byte));
//     }


// }

// void loop() {
//     delay(1000);

// }