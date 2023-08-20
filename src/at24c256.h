#ifndef _AT24C256_H
#define _AT24C256_H
#include <Arduino.h>
#include <Wire.h>

class AT24C256{
    public:
    AT24C256(int deviceAddress);
    void writeByte(uint16_t memoryAddress, byte data);
    void readByte(uint16_t memoryAddress, byte *data);
   void writeData(uint16_t memoryAddress, void *data, int len);
   void readData(uint16_t memoryAddress, void *data, int len);
    private:
     uint8_t _AT24C256Address;
     void writeInChunks(uint16_t len,uint8_t *recvData);
};

// class hardwareDriver{

// }

#endif