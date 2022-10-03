#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>

#include <Adafruit_VL53L0X.h>
#include <Adafruit_BNO055.h>

class _gyro {
   public:
    Adafruit_BNO055 bno = Adafruit_BNO055(0x28 >> 1);

    int deg;

    void init(void) {
        if (!bno.begin()) {
            Serial.print("BNO055 not found");
            while (1)
                ;
        }
        bno.setExtCrystalUse(true);
    };

    int read(void) {
        sensors_event_t event;
        bno.getEvent(&event);

        return constrain(round(event.orientation.x), 0, 359);
    };
} gyro;

class _ToF {
   public:
    Adafruit_VL53L0X lox = Adafruit_VL53L0X();

    int dist;
    void init(void) {
        if (!lox.begin()) {
            Serial.println("VL53L0X not found");
            while (1)
                ;
        }
        lox.startRangeContinuous();
    };

    int read(void) {
        int data = lox.readRange();

        if (data != 8190) {  // if the data is valid
            return data;
        } else {
            return 0;
        }
    };

    bool isAvailable(void) {
        return lox.isRangeComplete();
    };
} ToF;

void setup() {
    Serial.begin(115200);
    Wire.begin();

    gyro.init();
    ToF.init();
}

void loop() {
    Serial.write('F');
    gyro.deg = gyro.read();
    Serial.write(highByte(gyro.deg));
    Serial.write(lowByte(gyro.deg));

    while (!ToF.isAvailable())
        ;

    ToF.dist = ToF.read();
    Serial.write(highByte(ToF.dist));
    Serial.write(lowByte(ToF.dist));
}