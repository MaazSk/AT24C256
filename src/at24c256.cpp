#include "at24c256.h"

#define EEPROM_PAGE_SIZE 16
AT24C256::AT24C256(int deviceAddress)
{
    Wire.begin();
    _AT24C256Address = deviceAddress;
}

void AT24C256::writeByte(uint16_t memoryAddress, byte data)
{
    Wire.beginTransmission(_AT24C256Address);
    Wire.write((byte)(memoryAddress >> 8)); // Shift before typecasting to byte
    Wire.write((byte)(memoryAddress & 0xFF));
    Wire.write(data);
    Wire.endTransmission();
    delay(5);
}

void AT24C256 ::readByte(uint16_t memoryAddress, byte *data)
{
    Wire.beginTransmission(_AT24C256Address);
    Wire.write((byte)(memoryAddress >> 8)); // Shift before typecasting to byte
    Wire.write((byte)(memoryAddress & 0xFF));
    Wire.endTransmission();
    Wire.requestFrom(_AT24C256Address, sizeof(byte));
    if (Wire.available())
    {
        *data = Wire.read();
    }
}
void AT24C256::writeData(uint16_t memoryAddress, void *data, int len)
{
    uint8_t buffer[EEPROM_PAGE_SIZE];
    uint8_t *recvData = (uint8_t *)data;
    int bytesWritten = 0;
    while (bytesWritten < len)
    {
        Wire.beginTransmission(_AT24C256Address);
        Wire.write((byte)(memoryAddress >> 8));
        Wire.write((byte)(memoryAddress & 0xFF));
        int chunkSize = (len - bytesWritten > EEPROM_PAGE_SIZE) ? EEPROM_PAGE_SIZE : (len - bytesWritten);
        memmove(buffer,recvData+bytesWritten,chunkSize);
        for (int i = 0; i < chunkSize; i++)
        {
            Wire.write((byte)(recvData[i+bytesWritten]));
        }
        bytesWritten += chunkSize;
        memoryAddress = memoryAddress + chunkSize;
        Wire.endTransmission();
        delay(5);
    }
}

void AT24C256::readData(uint16_t memoryAddress, void *data, int len)
{
    uint8_t buffer[EEPROM_PAGE_SIZE];
    int bytesWritten = 0;
    char *recvData = (char *)data;
    while (bytesWritten < len)
    {
        int chunkSize = (len - bytesWritten > EEPROM_PAGE_SIZE) ? EEPROM_PAGE_SIZE : (len - bytesWritten);
        Wire.beginTransmission(_AT24C256Address);
        Wire.write((byte)(memoryAddress >> 8));
        Wire.write((byte)(memoryAddress & 0xFF));
        Wire.endTransmission();
        Wire.requestFrom(_AT24C256Address, (byte)chunkSize);

        for (int i = 0; i < chunkSize; i++)
        {
            if (Wire.available())
            {
                buffer[i] = Wire.read();
            }
        }
        // delay(10);
        memmove(recvData+bytesWritten,buffer,chunkSize);
        bytesWritten += chunkSize;
        memoryAddress = memoryAddress + chunkSize;
    }
}

void AT24C256::writeInChunks(uint16_t len, uint8_t *recvData)
{
    uint16_t bytesWritten = 0;
    while (bytesWritten < len)
    {
        int chunkSize = (len - bytesWritten > EEPROM_PAGE_SIZE) ? EEPROM_PAGE_SIZE : (len - bytesWritten);
        for (int i = 0; i < chunkSize; i++)
        {
            Wire.write((byte)(recvData[bytesWritten + i]));
        }
        bytesWritten += chunkSize;
    }
}