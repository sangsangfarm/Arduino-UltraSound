/**
 * @file UltraSound.cpp
 * @brief 거리 측정을 위한 초음파 센서 
 */

#include <UltraSound.h>

/**
 * @fn UltraSound::UltraSound(int echo_pin, int trig_pin)
 * @brief UltraSound 생성자
 * @param echo_pin echo 핀 번호
 * @param trig_pin trig 핀 번호
 * @return 초음파센서 클래스
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
UltraSound::UltraSound(int echo_pin, int trig_pin)
{
  _echo_pin = echo_pin;
  _trig_pin = trig_pin;
  pinMode(_echo_pin, INPUT);
  pinMode(_trig_pin, OUTPUT);
}

/**
 * @fn long UltraSound::getDistance(void)
 * @brief 초음파 거리 계산후 가져오기
 * @return 초음파 센서 측정 거리
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
long UltraSound::getDistance(void)
{
  if (!_ultrasound_data.power)
    return -1;
  digitalWrite(_trig_pin, LOW);
  delayMicroseconds(5);
  digitalWrite(_trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(_trig_pin, LOW);

  //거리 계산
  _duration = pulseIn(_echo_pin, HIGH);
  _distance = _duration * 0.034 / 2;
  delayMicroseconds(10);

  // 측정 범위 초과시 에러 처리(-1)
  if (_distance > _ultrasound_data.range)
  {
    _distance = -1;
  }

  // 상태 설정
  // 적정 수위
  if (_ultrasound_data.min_distance < _distance && _distance < _ultrasound_data.max_distance)
    _state = GOOD_DISTANCE;
  // 수위가 높음
  else if (_distance <= _ultrasound_data.min_distance)
    _state = CLOSE_DISTANCE;
  // 수위가 낮음
  else if (_distance >= _ultrasound_data.max_distance)
    _state = FAR_DISTANCE;
  print(F("[UltraSound] Distance = "));
  printf("%d cm state: %s\n", _distance, _state == FAR_DISTANCE ? "FAR" : (_state == CLOSE_DISTANCE ? "CLOSE" : "GOOD"));
  return _distance;
}

/**
 * @fn void UltraSound::setEEPROMAddress(int eeprom_address)
 * @brief EEPROM 주소 설정
 * @param eeprom_address 설정할 EEPROM 주소
 * @return void
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void UltraSound::setEEPROMAddress(int eeprom_address)
{
  _eeprom_address = eeprom_address;
  printf("[UltraSound] EEPROM 주소: %d\n", _eeprom_address);
}

/**
 * @fn void UltraSound::loadData(void)
 * @brief EEPROM 데이터 불러오기
 * @param eeprom_address 설정할 EEPROM 주소
 * @return void
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void UltraSound::loadData(void)
{
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(_eeprom_address, _ultrasound_data);
  if (!_ultrasound_data.is_saved)
  {
    _ultrasound_data.power = false;
    _ultrasound_data.max_distance = -99999;
    _ultrasound_data.min_distance = 99999;
    _ultrasound_data.range = 0;
  }
  EEPROM.end();
}

/**
 * @fn void UltraSound::saveData(void)
 * @brief EEPROM 데이터 저장
 * @return void
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void UltraSound::saveData(void)
{
  _ultrasound_data.is_saved = true;
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.put(_eeprom_address, _ultrasound_data);
  EEPROM.commit();
  EEPROM.end();
}

/**
 * @fn bool UltraSound::getPower(void)
 * @brief 초음파 센서 전원 상태 가져오기
 * @return 초음파 센서 전원 상태
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
bool UltraSound::getPower(void)
{
  return _ultrasound_data.power;
}

/**
 * @fn void UltraSound::setPower(bool power)
 * @brief 초음파 센서 전원 상태 설정
 * @param power 전원 상태
 * @return void
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void UltraSound::setPower(bool power)
{
  _ultrasound_data.power = power;
}

/**
 * @fn int UltraSound::getMinDistance(void)
 * @brief 최소 거리 값 가져오기
 * @return 최소 거리
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
int UltraSound::getMinDistance(void)
{
  return _ultrasound_data.min_distance;
}

/**
 * @fn void UltraSound::setMinDistance(int min_distance)
 * @brief 최소 거리 값 설정
 * @param min_distance 설정할 최소 거리
 * @return void
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void UltraSound::setMinDistance(int min_distance)
{
  _ultrasound_data.min_distance = min_distance;
}

/**
 * @fn int UltraSound::getMaxDistance(void)
 * @brief 최대 거리 값 가져오기
 * @return 최대 거리
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
int UltraSound::getMaxDistance(void)
{
  return _ultrasound_data.max_distance;
}

/**
 * @fn void UltraSound::setMaxDistance(int max_distance)
 * @brief 최대 거리 값 설정
 * @param max_distance 설정할 최대 거리
 * @return void
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void UltraSound::setMaxDistance(int max_distance)
{
  _ultrasound_data.max_distance = max_distance;
}

/**
 * @fn int UltraSound::getRange(void)
 * @brief 측정 범위 가져오기
 * @return 측정 범위
 * @date 2019-09-20
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
int UltraSound::getRange(void)
{
  return _ultrasound_data.range;
}

/**
 * @fn void UltraSound::setRange(int range)
 * @brief 측정 범위 설정
 * @param range 설정할 측정 범위
 * @return void
 * @date 2019-09-20
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void UltraSound::setRange(int range)
{
  _ultrasound_data.range = range;
}

/**
 * @fn UltrasoundState UltraSound::getState(void)
 * @brief 측정 거리 상태 가져오기
 * @return 측정한 거리 상태
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
UltrasoundState UltraSound::getState(void)
{
  return _state;
}

/**
 * @fn bool UltraSound::isStateChanged(void)
 * @brief 측정 거리 상태 변화 확인
 * @return 측정 거리 상태 변화 유무
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
bool UltraSound::isStateChanged(void)
{
  return _state != _prev_state;
}

/**
 * @fn void UltraSound::savePrevState(void)
 * @brief 현재 상태를 이전 상태에 저장
 * @return void
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void UltraSound::savePrevState(void)
{
  _prev_state = _state;
}