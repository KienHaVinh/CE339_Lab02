#include "AHT20.h"

void AHT20::begin() {
    delay(40);
    Wire.begin();
    Wire.beginTransmission(0x38); // transmit to device #8
    Wire.write(0xBE);
    Wire.write(0x08);
    Wire.write(0x00);
    delay(10);
    Wire.endTransmission();    // stop transmitting
}

bool AHT20::startSensor() {
    Wire.beginTransmission(0x38); // transmit to device #8
    Wire.write(0xac);
    Wire.write(0x33);
    Wire.write(0x00);
    Wire.endTransmission();    // stop transmitting

    unsigned long timer_s = millis();
    while(1) {
        if(millis()-timer_s > 80) return false;        // timeout
        Wire.requestFrom(0x38, 1);                      
        while(Wire.available()) {
            unsigned char data = Wire.read();
            if(data&0x80 != 0) return true;          // busy
        }
        delay(50);
    }
}


bool AHT20::getSensor(float *h, float *t) {
    startSensor();
    Wire.requestFrom(0x38, 6);
    unsigned char rawValues[6];
    int index = 0;
    while (Wire.available()) {
        rawValues[index++] = Wire.read();
    }
    if (rawValues[0] & 0x80) {
        return false;
    }

    unsigned long rawHumidity = 0;
    unsigned long rawTemperature = 0;

// Giá trị độ ẩm được chứa trong 20 bit đầu tiên của dữ liệu nhị phân trả về từ cảm biến AHT20 
    rawHumidity = rawValues[1];                  
    rawHumidity <<= 8;                           
    rawHumidity += rawValues[2];                 
    rawHumidity <<= 4;                          
    rawHumidity += rawValues[3] >> 4;            

    *h = (float)rawHumidity / 1048576.0; 

// Giá trị nhiệt độ được chứa trong 20 bit tiếp theo của dữ liệu nhị phân trả về từ cảm biến AHT20
    rawTemperature = rawValues[3] & 0x0f;       
    rawTemperature <<= 8;                        
    rawTemperature += rawValues[4];              
    rawTemperature <<= 8;                        
    rawTemperature += rawValues[5];              

    *t = (float)rawTemperature / 1048576.0 * 200.0 - 50.0;

    return true;
}

// Lấy dữ liệu từ cảm biến AHT20 và trả về giá trị true nếu thành công, false nếu thất bại 
