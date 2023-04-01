#include "AHT20.h"

AHT20 aht;
const float A = 20.0;
const float B = 27.0;
const float C = 33.0;
const int LED_PIN = 2;

void setup() {
    Serial.begin(9600);
    aht.begin();
}

void printTemperature(float temperature) {
    Serial.print("\nNhiệt độ: ");
    Serial.print(temperature);
    Serial.print(" °C \n");
}

void printErrorMessage() {
    Serial.println("Lấy dữ liệu từ cảm biến AHT20 thất bại!");
}

void printTime(int time) {
    Serial.print(time);
    Serial.print(" giây \n");
}

void setLed(int dutyCycle, int seconds) {
    analogWrite(LED_PIN, dutyCycle);
    Serial.print("Đèn sáng với duty cycle: ");
    Serial.println(dutyCycle);
    for (int i = 0; i <= seconds; i++) {
        printTime(i);
        delay(1000);
    }
    analogWrite(LED_PIN, 0);
    Serial.println("Đèn tắt");
    delay(5000);
}

void loop() {
    float humidity, temperature;
    int result = aht.getSensor(&humidity, &temperature);
    if (result) {
        printTemperature(temperature);
    } else {
        printErrorMessage();
    }
    delay(1000);

    if (temperature <= A) {
        Serial.println("Khoảng nhiệt độ: Dưới A");
        setLed(64, 10); // 25% duty cycle (255 * 25% = 64), 10 giây
    } else if (temperature <= B) {
        Serial.println("Khoảng nhiệt độ: Từ A đến B");
        setLed(102, 30); // 40% duty cycle (255 * 40% = 102), 30 giây
    } else if (temperature <= C) {
        Serial.println("Khoảng nhiệt độ: Từ B đến C");
        setLed(153, 40); // 60% duty cycle (255 * 60% = 153), 40 giây
    } else {
        Serial.println("Khoảng nhiệt độ: Trên C");
        setLed(191, 60); // 75% duty cycle (255 * 75% = 191), 60 giây
    }
}