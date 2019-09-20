/**
 * @file UltraSound.h
 * @brief 거리 측정을 위한 초음파 센서 
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */

#ifndef ULTRA_SOUND_H
#define ULTRA_SOUND_H

#include <Arduino.h>
#include <EEPROM.h>

#ifndef EEPROM_SIZE
#define EEPROM_SIZE 4096
#endif

//Debug
#ifndef ULTRA_SOUND_DEBUG_PORT
#define ULTRA_SOUND_DEBUG_PORT Serial
#endif

#ifdef ULTRA_SOUND_DEBUG
#define print(...) ULTRA_SOUND_DEBUG_PORT.print(__VA_ARGS__)
#define printf(...) ULTRA_SOUND_DEBUG_PORT.printf(__VA_ARGS__)
#define println(...) ULTRA_SOUND_DEBUG_PORT.println(__VA_ARGS__)

#else
#define print(...)
#define printf(...)
#define println(...)
#endif

/**
 * @brief 초음파 센서 거리 상태
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
enum UltrasoundState
{
  /** WaterLevelState의 NONE_WATER_LEVEL과 같음 */
  NONE_DISTANCE,
  /** WaterLevelState의 GOOD_WATER_LEVEL과 같음 */
  GOOD_DISTANCE,
  /** WaterLevelState의 FLOOD_WATER_LEVEL과 같음 */
  CLOSE_DISTANCE,
  /** WaterLevelState의 LACK_WATER_LEVEL과 같음 */
  FAR_DISTANCE,
};
/**
 * @struct UltrasoundData
 * @brief 초음파 센서 EEPROM 데이터 
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
struct UltrasoundData
{
  /** 초음파 센서 사용 유무 */
  bool power;
  /** 데이터 저장 유무 */
  bool is_saved;
  /** 최소 거리 */
  int min_distance;
  /** 최대 거리 */
  int max_distance;
  /** 측정 범위 */
  int range;
};

/**
 * @brief 초음파 센서
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
class UltraSound
{
private:
  int _echo_pin, _trig_pin;
  long _duration, _distance;
  int _eeprom_address = 0;
  UltrasoundState _prev_state = NONE_DISTANCE, _state = NONE_DISTANCE;
  UltrasoundData _ultrasound_data;

public:
  UltraSound(int echo_pin, int trig_pin);

  long getDistance(void);

  bool getPower(void);
  void setPower(bool power);

  int getMinDistance(void);
  void setMinDistance(int min_distance);

  int getMaxDistance(void);
  void setMaxDistance(int max_distance);

  int getRange(void);
  void setRange(int range);

  void setEEPROMAddress(int _eeprom_address);

  UltrasoundState getState(void);

  bool isStateChanged(void);
  void savePrevState(void);
  void loadData(void);
  void saveData(void);
};

#endif /** ULTRA_SOUND_H **/