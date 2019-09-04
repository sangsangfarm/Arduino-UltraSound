# Arduino-Ultrasound

Control ultrasound in Arduino.

## Support platform

- esp8226
- esp32

## Usage

```cpp
int echo_pin = 1, trig_pin = 2;
UltraSound ultrasound = UltraSound(echo_pin, trig_pin);

void setup() {
  bool power = true;
  int min_distance = 10;
  int max_distance = 100;

  ultrasound.setPower(power);
  ultrasound.setMinDistance(min_distance);
  ultrasound.setMaxDistance(max_distance);

  //If you load or save ultrasound info, use this.
  int eeprom_address = 1000;
  ultrasound.setEEPORMAddress(eeprom_address);
  ultrasound.loadData();
  ultrasound.saveData();
}

void loop() {
  long distance = ultrasound.getDistance();
}
```
