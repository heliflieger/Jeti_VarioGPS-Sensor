/*
  ------------------------------------------------------------------
                    Jeti VarioGPS Sensor
  ------------------------------------------------------------------
            *** Universeller Jeti Telemetrie Sensor ***
  Vario, GPS, Strom/Spannung, Empfängerspannungen, Temperaturmessung

*/
#define VARIOGPS_VERSION "Version V2.4"
/*

  ******************************************************************
  Versionen:
  V2.4     26.12.22 Kompass GX-511 hinzugefügt, Trainingswerte und Luftdruck auf Meereshöhe einstellbar
  V2.3.6.6 24.11.22 fix compile error with GPS_LOCATION_PRIO_HIGH
  V2.3.6.5 23.11.22 fix to allow usage GPS device only
  V2.3.6.4 16.11.22 dynamic GPS speed and vario value transmission (V>30m/S GPS Speed prio is increased, vario values are decreased 
                    on the Jeti Telemetry Interface
                    more option in settings.h for GPS values
  V2.3.6.3 03.04.21 bugfix do the ms5611.run() only if MS5611 is detected
  V2.3.6.2 01.03.21 bugfix JetiExSensor ExBuf overrun fixed (lib added) and usage of new prio-defines
  V2.3.6.1 23.02.21 bug with wrong height values for VarioMS5611 fixed
  V2.3.6  19.02.21  priorized telemetry values introduced, now variometer value is  transmitted about 10 times/s
                    to reduce the memory footprint, a lot of the sensor values and code fragments are now in #define sections
  V2.3.5  11.02.21  optimized MS5611 Library
  V2.3.4  10.02.21  RX Q value support added 
  V2.3.3  27.04.19  Fehler behoben: Azimuth und Course war vertauscht
  V2.3.2  07.03.19  Fehler bei AirSpeed Sensor behoben, Geschwindigkeit wurde nur in 3-4Kmh Schritten angezeigt
  V2.3.1  22.08.18  Fehler bei Temepraturwert behoben (Dezimalstelle wurde nicht angezeigt)
  V2.3    09.07.18  MPXV7002/MPXV5004 für Air-Speed wird unterstützt 
                    TEK (Total Energie Kompensation) mit Air-Speed oder GPS-Speed (basierend auf Code von Rainer Stransky)
                    EX-Bus mit 125kbaud, Lib (v0.95) von Bernd Wokoeck
                    Stromsensor per JetiBox kalibrierbar (reset offset) 
                    separate Datei für Einstellungen (settings.h), Compillerwahrnung bei inkompatibilität der Softwareoptionen
                    Bezeichnungen der Telemetriewerte angepasst, damit diese vom Jeti Sender bestmöglich übersetzt werden
                    Fehler behoben: STATIC Variablen entfehrnt, konnte in einigen Fällen zu RAM-Problemen führen und ergab eine fehlerhafte Funktion 
                    Fehler behoben: 2D/3D GPS Distanz in den Einstellungen war verkehrt       
  V2.2.1  26.03.18  Bugfix bei float<->int casting Smoothing Factor (by RS)
  V2.2    15.02.18  Vario Tiefpass mit nur einem Smoothing Factor (by RS)
                    Jeder Sensor kann mit #define deaktiviert werden
  V2.1.1  13.01.18  kleine Fehlerbehebung mit libraries
  V2.1    23.12.17  Analog Messeingänge stark überarbeitet, NTC-Temperaturmessung hinzugefügt,
                    startup-/auto-/maual-reset für Kapazitätsanzeige, SRAM-Speicheroptimierung
  V2.0.2  03.12.17  Fehler in GPS Trip behoben
  V2.0.1  21.11.17  Fehler bei Spannungsmessung behoben
  V2.0    20.11.17  GPS Trip[km] und verbrauchte Kapazität[mAh] eingebaut, Stromsensoren ACS712 eingebaut
  V1.9    17.11.17  ACSxxx Stromsensoren eingebaut
  V1.8    17.11.17  Luftdrucksensor MS5611/LPS werden unterstützt
  V1.7.2  15.11.17  Fehlerbehebung mit BME280
  V1.7.1  15.11.17  Speicheroptimierung, kleinere Fehler behoben
  V1.7    12.11.17  Empfängerspannungen können gemessen werden
  V1.6    05.11.17  Luftdrucksensoren BMP085/BMP180/BMP280/BME280 eingebaut, und zu VarioGPS umbenannt
  V1.5    05.11.17  Code von RC-Thoughts(Jeti_GPS-Sensor) übernommen




  ******************************************************************
  Unterstützte Hardware:

  - Arduino Pro Mini 3.3V-8Mhz/5V-16Mhz
  - GPS-Modul mit NMEA Protokoll und UART@9600baud
  - Barometer Sensoren: BMP280, BME280, MS5611, LPSxxx
  - Airspeed Sensoren: MPXV7002, MPXV5004
  - Stromsensoren @3.3V/5V Betriebsspannung:        AttoPilot Module @3.3V: 45A/13.6V - 90A/50V - 180A/50V (@5V: 45A/20.6V - 90A/60V - 180A/60V)
                                                    APM2.5 PowerModul @5V: 90A/50V (@3.3V: 58A/33.4V)
                                                    ACS758_50B, ACS758_100B, ACS758_150B, ACS758_200B, ACS758_50U, ACS758_100U, ACS758_150U, ACS758_200U
  - zusätzliche Stromsensoren @5V Betriebsspannung: ACS712_05, ACS712_20, ACS712_30



  ******************************************************************
  Anzeige:

  Nur mit Luftdrucksensor werden die Werte angezeigt:
  - Rel. und Abs. Höhe
  - Vario
  - Luftdruck
  - Temperatur
  - Luftfeuchte (nur mit BME280)

  Im GPS Basic Mode werden die Werte angezeigt:
  - Position
  - Geschwindigkeit
  - Rel. und Abs. Höhe
  - Vario

  Im GPS Extended Mode werden zusätzlich die Werte angezeigt:
  - Distanz vom Modell zum Startpunkt (2D oder 3D)
  - zurückgelegte Strecke (Trip)
  - Flugrichtung (Heading)
  - Kurs vom Modell zum Startpunkt
  - Anzahl Satelliten
  - HDOP (Horizontaler Faktor der Positionsgenauigkeit)
  - Luftdruck
  - Temperatur
  - Luftfeuchtigkeit

  An den Analogeingängen sind folgende Messungen möglich:
  - Strom- und Spannung für Hauptantrieb mit verbrauchter Kapazität[mAh] und Leistung[W]
  - 2x Empfängerspannung
  - Temperatur mit NTC-Wiederstand von -55 bis +155°C

  Folgende Einstellungen können per Jetibox vorgenommen werden:
  - GPS: deaktiviert, Basic oder Extended
  - GPS Distanz: 2D oder 3D
  - Vario Filterparameter: Smoothing und Deadzone
  - Stromsensor für Hauptantrieb
  - Einstellung Reset der Kapazität:
        STARTUP(Wert ist nach jedem Einschalten auf 0)
        AUTO(Wert wird gespeichert und erst zurückgesetzt wenn ein geladener Akku angeschlossen wird)
        MANUAL(Wert muss manuell per Jetibox zurückgesetzt werden mit RESET OFFSET)
  - Rx1, Rx2 Empfängerspannungsmessung aktivieren
  - Temperaturmessung aktivieren

*/

#include "settings.h"

// Check supply voltage
#if V_REF < 1800 || V_REF > 5500
  #error unsupported supply voltage
#endif

// Check airspeed-sensor supply voltage
#if defined(SUPPORT_MPXV7002_MPXV5004) && V_REF < 4750 
  #warning Supply voltage is below 4750 mV, the airspeed sensor (MPXV7002 / MPXV5004) may not work properly
#endif

// Check the compatibility of EX-Bus and GPS sensor
#if defined(SUPPORT_EX_BUS) && defined(SUPPORT_GPS)
  #undef SUPPORT_GPS
  #warning GPS sensor is not compatible with EX-Bus protocol, GPS sensor is disabled
#endif

// Check speed sensor is available for TEC
#if defined(SUPPORT_TEC) && !defined(SUPPORT_MPXV7002_MPXV5004) && !defined(SUPPORT_GPS)
  #undef SUPPORT_TEC
  #warning no speed sensor available, TEC is disabled
#endif

// Check pressure sensor is available for TEC
#if defined(SUPPORT_TEC) && !defined(SUPPORT_BMx280) && !defined(SUPPORT_MS5611) && !defined(SUPPORT_LPS)
  #undef SUPPORT_TEC
  #warning no pressure sensor available, TEC is disabled
#endif

#ifdef SUPPORT_EX_BUS
  #include <JetiExBusProtocol.h>
  JetiExBusProtocol jetiEx;
#else
  #include <JetiExSerial.h>
  #include <JetiExProtocol.h>
  JetiExProtocol jetiEx;
#endif

#include <EEPROM.h>
#include <Wire.h>
#include "defaults.h"

#ifdef SUPPORT_GPS
  #include <TinyGPSPlus.h>
  #include <AltSoftSerial.h>
  TinyGPSPlus gps;
  AltSoftSerial gpsSerial;
#endif

#ifdef SUPPORT_BMx280
  #include "BMx_Sensor.h"
  BMx_Sensor boschPressureSensor;
#endif

#ifdef SUPPORT_MS5611
  #include <VarioMS5611.h>
  VarioMS5611 ms5611;
#endif

#ifdef SUPPORT_LPS
  #include <LPS.h>
  LPS lps;
#endif

#ifdef SUPPORT_LSM303
  #include <LSM303.h>
  LSM303 compass;
#endif

struct {
  uint8_t mode = DEFAULT_GPS_MODE;
  bool distance3D = DEFAULT_GPS_3D_DISTANCE;
} gpsSettings;

struct {
  uint8_t type = unknown;
  float smoothingValue;
  uint8_t deadzone;
  long seaLevelPressure;
} pressureSensor;

// set defaults
#ifdef SUPPORT_MAIN_DRIVE
uint8_t currentSensor = DEFAULT_CURRENT_SENSOR;
uint8_t capacityMode = DEFAULT_CAPACITY_MODE;
unsigned long lastTimeCapacity = 0;
float cuAmp;
int ampOffsetCalibration = 0;
float capacityConsumption = 0;
const float timefactorCapacityConsumption = (1000.0/MEASURING_INTERVAL*60*60)/1000;
#endif
bool enableRx1 = DEFAULT_ENABLE_Rx1;
bool enableRx2 = DEFAULT_ENABLE_Rx2;
bool enableExtTemp = DEFAULT_ENABLE_EXT_TEMP;
#ifdef SUPPORT_LSM303
bool compassEnabled = DEFAULT_ENABLE_COMPASS;
bool trainingMode = DEFAULT_ENABLE_TRAINING_MODE;
#endif
uint8_t TECmode = DEFAULT_TEC_MODE;
uint8_t airSpeedSensor = DEFAULT_AIRSPEED_SENSOR;
float compassPos = -1.0;
bool compassInitalized = false;

unsigned long lastTime = 0;
// Main drive variables

// pressure sensor variables
long startAltitude = 0;
long uRelAltitude = 0;
long uTrainingAltitude = 0;
long uAbsAltitude = 0;
long uPressure = PRESSURE_SEALEVEL;
float uTemperature = 20;
float lastVariofilter = 0;
long lastAltitude = 0;

// airspeed variables
int refAirspeedPressure;
float uAirSpeed = 0;
float lastAirSpeed = 0;

// TEC variables
unsigned long dT = 0;
float dV;
float uSpeedMS;
long  lastTimeSpeed = 0;
float lastGPSspeedMS = 0;

// GPS variables
int homeSetCount = 0;
float home_lat;
float home_lon;
float last_lat;
float last_lon;
long lastAbsAltitude = 0;
unsigned long tripDist;


// Restart CPU
void(* resetCPU) (void) = 0;

#include "HandleMenu.h"


const float factorVoltageDivider[] = { float(voltageInputR1[0]+voltageInputR2[0])/voltageInputR2[0],
                                       float(voltageInputR1[1]+voltageInputR2[1])/voltageInputR2[1],
                                       float(voltageInputR1[2]+voltageInputR2[2])/voltageInputR2[2],
                                       float(voltageInputR1[3]+voltageInputR2[3])/voltageInputR2[3],
                                       float(voltageInputR1[4]+voltageInputR2[4])/voltageInputR2[4],
                                       float(voltageInputR1[5]+voltageInputR2[5])/voltageInputR2[5],
                                       float(voltageInputR1[6]+voltageInputR2[6])/voltageInputR2[6],
                                       float(voltageInputR1[7]+voltageInputR2[7])/voltageInputR2[7]  };


// read mV from voltage sensor
long readAnalog_mV(uint8_t sensorVolt, uint8_t pin){
  return (analogRead(pin)/1024.0)*V_REF*factorVoltageDivider[sensorVolt];
}

#ifdef SUPPORT_MAIN_DRIVE
// get voltage sensor type
uint8_t getVoltageSensorTyp(){
  if (currentSensor <= APM25_A){
    return currentSensor-1;
  #if V_REF >= 4500
  }else if(currentSensor >= ACS712_05 && currentSensor <= ACS712_30){
    return ACS712_voltage;
  #endif
  }else if(currentSensor >= ACS758_50B){
    return ACS758_voltage;
  }
  return 0;
}
#endif

#ifdef SUPPORT_RXQ

#define RXQ_SERVO_SIGNAL_LOSS_GAP 100
#include <util/atomic.h> // this library includes the ATOMIC_BLOCK macro.

volatile static unsigned long  ourServoSignalPulseWidth = 0;
volatile static unsigned long  ourServoSignalRisingEdge = 0;
volatile static int ourServoSignalLossCount = 0;
volatile static int ourServoSignalGapMax = 0;
volatile static int ourServoSignalGap = 0;
volatile unsigned long  ourPulsCnt = 0;
static int ourServoSignalsPerSecond = 0;



void sv_rising() {
  unsigned long now_us = micros();
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    // calculate the servo signal duration (time between two PWM signals) in ms
    // value depends on the TX signal frequency (100Hz, 50Hz, ...) and the loss of signals
    // typical values are 10ms (for 100Hz) or 20ms (for 50Hz)
    ourServoSignalGap = (now_us - ourServoSignalRisingEdge)/1000;
    ourServoSignalRisingEdge = now_us;
  }


  // time of signal puls starting at the raising edge in ms
  attachInterrupt(digitalPinToInterrupt(RXQ_SIGNAL_PIN), sv_falling, FALLING);
}

void sv_falling() {
  unsigned long now_us = micros();
  ourServoSignalPulseWidth = now_us - ourServoSignalRisingEdge;
  ourPulsCnt++;
  attachInterrupt(digitalPinToInterrupt(RXQ_SIGNAL_PIN), sv_rising, RISING);
}

bool checkRCServoSignal() {

  // start the RC servo signal handling after 10s to 
  // avoid wrong durations before RX is bound to TX
  unsigned long now_ms = millis();
  if (now_ms > 10000) {
    static bool sv_SignalLossState = false;
    // if there is no signal, no raising interrupt will call the interrupt routine,
    // so check if the last rising measure is maximum, if not use the current gap
    int sigGap = (micros() - ourServoSignalRisingEdge) / 1000;
    ourServoSignalGap = max(sigGap, ourServoSignalGap);

    if (ourServoSignalGapMax < ourServoSignalGap ) {
      ourServoSignalGapMax = ourServoSignalGap;
    }
    if (ourServoSignalGap > RXQ_SERVO_SIGNAL_LOSS_GAP) {
      if (!sv_SignalLossState) {
        ourServoSignalLossCount++;
      }
      sv_SignalLossState = true;
      return false;
    }
    sv_SignalLossState = false;
  }


  static unsigned long last;
  int minterval = now_ms - last;
  if (minterval > 1000) {
    static unsigned long lastPulses = 0 ;
    ourServoSignalsPerSecond = (ourPulsCnt - lastPulses) * 1000 / minterval;
    lastPulses = ourPulsCnt;
    last = now_ms;
  }

  return true;
}
#endif // SUPPORT_RXQ


void setup()
{
  // identify pressure sensor
  #ifdef SUPPORT_BMx280
    pressureSensor.type = boschPressureSensor.begin(0x76);
    if(pressureSensor.type == unknown){
      pressureSensor.type = boschPressureSensor.begin(0x77);
    }
  #endif
  #if defined(SUPPORT_MS5611) || defined(SUPPORT_LPS)
    if(pressureSensor.type == unknown){
      #ifdef SUPPORT_LPS
      if (lps.init()) {
        Wire.begin();
        lps.enableDefault();
        pressureSensor.type = LPS_;
      } else {
      #endif
        Wire.begin();
        Wire.beginTransmission(MS5611_ADDRESS); // if no Bosch sensor, check if return an ACK on MS5611 address
        if (Wire.endTransmission() == 0) {
          ms5611.begin();
          // settings tested and ok, with a vario deadzone of +-10cm/s there is almost no failure tone of the vario
          ms5611.setOversampling(MS5611_ULTRA_HIGH_RES);
          ms5611.setVerticalSpeedSmoothingFactor(0.96);
          ms5611.setPressureSmoothingFactor(0.92);
          pressureSensor.type = MS5611_;
        }
      #ifdef SUPPORT_LPS
      }
      #endif
    }
  #endif

  #if defined(SUPPORT_BMx280) || defined(SUPPORT_MS5611) || defined(SUPPORT_LPS)
  switch (pressureSensor.type){
    case BMP280:
      pressureSensor.smoothingValue = BMx280_SMOOTHING;
      pressureSensor.deadzone = BMx280_DEADZONE;
      break;
    case BME280:
      pressureSensor.smoothingValue = BMx280_SMOOTHING;
      pressureSensor.deadzone = BMx280_DEADZONE;
      break;
    case MS5611_ :
      pressureSensor.smoothingValue = MS5611_SMOOTHING;
      pressureSensor.deadzone = MS5611_DEADZONE;
      pressureSensor.seaLevelPressure = MS5611_SEALEVELPRESSURE;
      break;
    case LPS_ :
      pressureSensor.smoothingValue = LPS_SMOOTHING;
      pressureSensor.deadzone = LPS_DEADZONE;
      break;
  }
  #endif
 
  // read settings from eeprom   
  #ifdef SUPPORT_GPS
  if (EEPROM.read(P_GPS_MODE) != 0xFF) {
    gpsSettings.mode = EEPROM.read(P_GPS_MODE);
  }
  if (EEPROM.read(P_GPS_3D) != 0xFF) {
    gpsSettings.distance3D = EEPROM.read(P_GPS_3D);
  }
  #endif

  #ifdef SUPPORT_MAIN_DRIVE
  if (EEPROM.read(P_CURRENT_SENSOR) != 0xFF) {
    currentSensor = EEPROM.read(P_CURRENT_SENSOR);
  }
  if (EEPROM.read(P_CURRENT_CALIBRATION) != 0xFF) {
    ampOffsetCalibration = EEPROM.read(P_CURRENT_CALIBRATION)-127;
  }
  if (EEPROM.read(P_CAPACITY_MODE) != 0xFF) {
    capacityMode = EEPROM.read(P_CAPACITY_MODE);
  }
  #endif

  #ifdef SUPPORT_RX_VOLTAGE
  if (EEPROM.read(P_ENABLE_RX1) != 0xFF) {
    enableRx1 = EEPROM.read(P_ENABLE_RX1);
  }
  if (EEPROM.read(P_ENABLE_RX2) != 0xFF) {
    enableRx2 = EEPROM.read(P_ENABLE_RX2);
  }
  #endif

  #ifdef SUPPORT_EXT_TEMP
  if (EEPROM.read(P_ENABLE_TEMP) != 0xFF) {
    enableExtTemp = EEPROM.read(P_ENABLE_TEMP);
  }
  #endif

  #ifdef SUPPORT_LSM303
  if (EEPROM.read(P_ENABLE_COMPASS) != 0xFF) {
    compassEnabled = EEPROM.read(P_ENABLE_COMPASS);
    trainingMode = EEPROM.read(P_ENABLE_TRAINING);
    
    if (compass.init()) {
      compass.enableDefault();
      /*
      Calibration values; the default values of +/-32767 for each axis
      lead to an assumed magnetometer bias of 0. Use the Calibrate example
      program to determine appropriate values for your particular unit.
      */
      compass.m_min = DEFAULT_COMPAS_MIN;
      compass.m_max = DEFAULT_COMPAS_MAX;
      compassInitalized = true;
    } else {
      compassEnabled = false;
      EEPROM.write(P_ENABLE_COMPASS, compassEnabled);
    } 
  }
  #endif

  if (EEPROM.read(P_VARIO_SMOOTHING) != 0xFF) {
    pressureSensor.smoothingValue = (float)EEPROM.read(P_VARIO_SMOOTHING) / 100;
  }
  if (EEPROM.read(P_VARIO_DEADZONE) != 0xFF) {
    pressureSensor.deadzone = EEPROM.read(P_VARIO_DEADZONE);
  }
  if (EEPROM.read(P_VARIO_SEALEVELPRESSURE) != 0xFF) {
    pressureSensor.seaLevelPressure = ((long)EEPROM.read(P_VARIO_SEALEVELPRESSURE) << 24) +
         ((long)EEPROM.read(P_VARIO_SEALEVELPRESSURE + 1) << 16) +
         ((long)EEPROM.read(P_VARIO_SEALEVELPRESSURE + 2) << 8) +
         (long)EEPROM.read(P_VARIO_SEALEVELPRESSURE + 3);
  }

  #ifdef SUPPORT_MPXV7002_MPXV5004
  if (EEPROM.read(P_AIRSPEED_SENSOR) != 0xFF) {
    airSpeedSensor = EEPROM.read(P_AIRSPEED_SENSOR);
  }
  #endif
  
  #ifdef SUPPORT_TEC
  if (EEPROM.read(P_TEC_MODE) != 0xFF) {
    TECmode = EEPROM.read(P_TEC_MODE);
  }
  #endif
  
  #ifdef SUPPORT_GPS
    // init GPS
    gpsSerial.begin(GPSBaud);
  #endif

  #ifdef SUPPORT_MPXV7002_MPXV5004
  // init airspeed sensor
  if(airSpeedSensor){
    //delay(50);
    refAirspeedPressure = analogRead(AIRSPEED_PIN);
  }
  #endif

  // Setup telemetry sensors
  if(pressureSensor.type == unknown){
    jetiEx.SetSensorActive( ID_VARIO, false, sensors );
  }
  #ifndef SUPPORT_EX_BUS
  if(pressureSensor.type == MS5611_){
    // to speedup the fine grained vario values set the buffer send cycle to a minimum value
    jetiEx.SetJetiSendCycle(JETI_SEND_CYCLE);
  }
  #endif

  #ifdef SUPPORT_BMx280
  if(gpsSettings.mode == GPS_basic || pressureSensor.type != BME280){
    jetiEx.SetSensorActive( ID_HUMIDITY, false, sensors );
  }
  #endif
  #ifdef SUPPORT_GPS
  if(gpsSettings.mode == GPS_basic || pressureSensor.type == unknown){
    jetiEx.SetSensorActive( ID_PRESSURE, false, sensors );
    jetiEx.SetSensorActive( ID_TEMPERATURE, false, sensors );
  }

  if(gpsSettings.mode == GPS_disabled){
    jetiEx.SetSensorActive( ID_GPSLAT, false, sensors );
    jetiEx.SetSensorActive( ID_GPSLON, false, sensors );
    jetiEx.SetSensorActive( ID_GPSSPEED, false, sensors );
    jetiEx.SetSensorActive( ID_DIST, false, sensors );
  }

  if(gpsSettings.mode == GPS_disabled && pressureSensor.type == unknown){
    jetiEx.SetSensorActive( ID_ALTREL, false, sensors );
    jetiEx.SetSensorActive( ID_ALTABS, false, sensors );
    jetiEx.SetSensorActive( ID_TRAINING_ALTIT, false, sensors );
  }

  #ifdef SUPPORT_GPS_EXTENDED
  if(gpsSettings.mode != GPS_extended){
    jetiEx.SetSensorActive( ID_TRIP, false, sensors );
    jetiEx.SetSensorActive( ID_AZIMUTH, false, sensors );
    jetiEx.SetSensorActive( ID_COURSE, false, sensors );
    jetiEx.SetSensorActive( ID_SATS, false, sensors );
    jetiEx.SetSensorActive( ID_HDOP, false, sensors );
  }
  #endif
  #endif

  #ifdef SUPPORT_MPXV7002_MPXV5004
  if(!airSpeedSensor){
    jetiEx.SetSensorActive( ID_AIRSPEED, false, sensors );
  }
  #endif

  #ifdef SUPPORT_MAIN_DRIVE
  if(currentSensor == mainDrive_disabled){
    jetiEx.SetSensorActive( ID_VOLTAGE, false, sensors );
    jetiEx.SetSensorActive( ID_CURRENT, false, sensors );
    jetiEx.SetSensorActive( ID_CAPACITY, false, sensors );
    jetiEx.SetSensorActive( ID_POWER, false, sensors );
  }else{
    if(capacityMode > startup){
      // read capacity from eeprom
      EEPROM.get(P_CAPACITY_VALUE, capacityConsumption);
      if(capacityMode == automatic){
        float cuVolt = readAnalog_mV(getVoltageSensorTyp(),VOLTAGE_PIN)/1000.0;
        float oldVolt;
        EEPROM.get(P_VOLTAGE_VALUE, oldVolt);
        if(cuVolt >= oldVolt * ((100.0+VOLTAGE_DIFFERENCE)/100.0)){
          capacityConsumption = 0;
        }
      }
    }
  }
  #endif

  #ifdef SUPPORT_RX_VOLTAGE
  if(!enableRx1){
    jetiEx.SetSensorActive( ID_RX1_VOLTAGE, false, sensors );
  }

  if(!enableRx2){
    jetiEx.SetSensorActive( ID_RX2_VOLTAGE, false, sensors );
  }
  #endif

  #ifdef SUPPORT_EXT_TEMP
  if(!enableExtTemp){
    jetiEx.SetSensorActive( ID_EXT_TEMP, false, sensors );
  }
  #endif

  #ifdef SUPPORT_RXQ
    #ifdef RXQ_SIGNAL_PIN_PULLUP
      pinMode(RXQ_SIGNAL_PIN, INPUT_PULLUP);
    #endif
    // when the signal D2 goes high, call the rising function
    attachInterrupt(digitalPinToInterrupt(RXQ_SIGNAL_PIN), sv_rising, RISING);
  #endif

  #ifdef SUPPORT_LSM303
  if(!compassEnabled){
    jetiEx.SetSensorActive( ID_COMPASS, false, sensors );
    jetiEx.SetSensorActive( ID_REL_COMPASS, false, sensors );
  }
  if (!trainingMode) {
    jetiEx.SetSensorActive( ID_TRAINING_ORIENTATION, false, sensors );
    jetiEx.SetSensorActive( ID_TRAINING_ALTIT, false, sensors );
  }
  #endif

  // init Jeti EX Bus
  jetiEx.SetDeviceId( 0x76, 0x32 );
  
  #ifdef SUPPORT_EX_BUS
  jetiEx.Start( "VarioGPS", sensors, 0 );
  #else
  jetiEx.Start( "VarioGPS", sensors, JetiExProtocol::SERIAL2 );
  #endif

}


void loop()
{
  #ifdef SUPPORT_EX_BUS
  //if(jetiEx.IsBusReleased()){
  #endif
  
  #ifdef SUPPORT_RXQ
    checkRCServoSignal();
  #endif

  if((millis() - lastTime) > MEASURING_INTERVAL) {

    #ifdef SUPPORT_MPXV7002_MPXV5004
    if(airSpeedSensor){
      
      // get air speed from MPXV7002/MPXV5004
      // based on code from johnlenfr, http://johnlenfr.1s.fr
      int airSpeedPressure = analogRead(AIRSPEED_PIN);
      if (airSpeedPressure < refAirspeedPressure) airSpeedPressure = refAirspeedPressure;

      // differential pressure in Pa, 1 V/kPa, max 3920 Pa
      float pitotpressure = 5000.0 * ((airSpeedPressure - refAirspeedPressure) / 1024.0f) + uPressure;    
      float density = (uPressure * DRY_AIR_MOLAR_MASS) / ((uTemperature + 273.16) * UNIVERSAL_GAS_CONSTANT);
      uAirSpeed = sqrt ((2 * (pitotpressure - uPressure)) / density);
      
      // IIR Low Pass Filter
      uAirSpeed = uAirSpeed + AIRSPEED_SMOOTHING * (lastAirSpeed - uAirSpeed);

      // TEC 
      #ifdef SUPPORT_TEC
      if(TECmode == TEC_airSpeed){
        uSpeedMS = uAirSpeed;
        dV = uAirSpeed - lastAirSpeed;     // delta speed in m/s
      }
      #endif
      
      lastAirSpeed = uAirSpeed;

      #ifdef UNIT_US
      #ifdef SUPPORT_EX_BUS
        jetiEx.SetSensorValue( ID_AIRSPEED, uAirSpeed*2.23694);    // speed in mph
      #else
        jetiEx.SetSensorValue( ID_AIRSPEED, uAirSpeed*2.23694, JEP_PRIO_LOW);    // speed in mph
      #endif
      #else
      #ifdef SUPPORT_EX_BUS
        jetiEx.SetSensorValue( ID_AIRSPEED, uAirSpeed*3.6);        // speed in km/h
      #else
        jetiEx.SetSensorValue( ID_AIRSPEED, uAirSpeed*3.6, JEP_PRIO_LOW);        // speed in km/h
      #endif
      #endif
    }
    #endif

    #if defined(SUPPORT_BMx280) || defined(SUPPORT_MS5611) || defined(SUPPORT_LPS)
    if(pressureSensor.type != unknown){
      long curAltitude;
      long uVario;

      // Read sensormodule values
      switch (pressureSensor.type){
        #ifdef SUPPORT_MS5611
        case MS5611_:
          // OLD uPressure = ms5611.readPressure(true); // In Pascal (100 Pa = 1 hPa = 1 mbar)
          // OLD curAltitude = ms5611.getAltitude(uPressure, PRESSURE_SEALEVEL) * 100; // In Centimeter
          // OLD uTemperature = ms5611.readTemperature(true); // In Celsius
          // varioMS5611
          ms5611.setVerticalSpeedSmoothingFactor(pressureSensor.smoothingValue);
          uTemperature = ms5611.getTemperature();
          uPressure = ms5611.getSmoothedPressure();
          if(gpsSettings.mode == GPS_disabled){
            uAbsAltitude = ms5611.calcAltitude(uPressure, pressureSensor.seaLevelPressure);
          }
          uRelAltitude = ms5611.calcRelAltitude(uPressure)* 10; // we want rel. altitude in decimeter;
          uVario = ms5611.getVerticalSpeed();

          // Training values. 2m / 4,5m / 7m
          if (uRelAltitude == 20 || uRelAltitude == 45 || uRelAltitude == 70) {
            uTrainingAltitude = 1;
          } else {
            uTrainingAltitude = 0;
          }
          break;
        #endif
        #ifdef SUPPORT_LPS
        case LPS_:
          uPressure = lps.readPressureMillibars(); // In Pascal (100 Pa = 1 hPa = 1 mbar)
          curAltitude = lps.pressureToAltitudeMeters(uPressure) * 100; // In Centimeter
          uTemperature = lps.readTemperatureC(); // In Celsius
          break;
        #endif
        #ifdef SUPPORT_BMx280
        default:
          uPressure = boschPressureSensor.readPressure(); // In Pascal (100 Pa = 1 hPa = 1 mbar)
          curAltitude = boschPressureSensor.readAltitude() * 100; // In Centimeter
          uTemperature = boschPressureSensor.readTemperature(); // In Celsius
          if(pressureSensor.type == BME280){
            #ifdef SUPPORT_EX_BUS
              jetiEx.SetSensorValue( ID_HUMIDITY, boschPressureSensor.readHumidity() * 10); // In %rH
            #else
              jetiEx.SetSensorValue( ID_HUMIDITY, boschPressureSensor.readHumidity() * 10, JEP_PRIO_ULTRA_LOW); // In %rH
            #endif
          }
          break;
        #endif
      }

      #ifndef SUPPORT_MS5611
      if (startAltitude == 0) {
        // Set start-altitude in sensor-start
        startAltitude = curAltitude;
        lastAltitude = curAltitude;
      }else{
        // Altitude
        uRelAltitude = (curAltitude - startAltitude) / 10;
        uAbsAltitude = curAltitude / 100;
      }
      #endif
     
      // Vario calculation
      unsigned long dTvario = millis() - lastTime;     // delta time in ms
      #ifndef SUPPORT_MS5611
      uVario = (curAltitude - lastAltitude) * (1000.0 / dTvario);
      lastAltitude = curAltitude;      
      #endif

      // TEC (Total energie compensation)  
      // based on code from Rainer Stransky
      // see: http://www.how2soar.de/images/H2S_media/02_pdf/TE-Vario_im_Stroemungsfeld.pdf  
      #ifdef SUPPORT_TEC
     
      if(TECmode == TEC_airSpeed){
        dT = dTvario;
      }

      if (dT != 0 && dT < 3000) { // avoid divison by zero
        long tvario = uSpeedMS  * 10194 * dV / dT;
        uVario += tvario;
      }
      #endif
    
      #ifndef SUPPORT_MS5611
      // Vario Filter
      // IIR Low Pass Filter
      // y[i] := α * x[i] + (1-α) * y[i-1]
      //      := y[i-1] + α * (x[i] - y[i-1])
      // mit α = 1- β
      // y[i] := (1-β) * x[i] + β * y[i-1]
      //      := x[i] - β * x[i] + β * y[i-1]
      //      := x[i] + β (y[i-1] - x[i])
      // see: https://en.wikipedia.org/wiki/Low-pass_filter#Simple_infinite_impulse_response_filter
      uVario = uVario + pressureSensor.smoothingValue * (lastVariofilter - uVario);
      lastVariofilter = uVario;
      #endif
      
      // Vario dead zone filter
      if (uVario > pressureSensor.deadzone) {
        uVario -= pressureSensor.deadzone;
      } else if (uVario < (pressureSensor.deadzone * -1)) {
        uVario -= (pressureSensor.deadzone * -1);
      } else {
        uVario = 0;
      }

      #ifdef UNIT_US
        // EU to US conversions
        // ft/s = m/s / 0.3048
        // inHG = hPa * 0,029529983071445
        // ft = m / 0.3048
        #ifdef SUPPORT_EX_BUS
          jetiEx.SetSensorValue( ID_VARIO, uVario /= 0.3048);                 // ft/s
          jetiEx.SetSensorValue( ID_PRESSURE, uPressure * 0.029529983071445);  // inHG
          jetiEx.SetSensorValue( ID_TEMPERATURE, (uTemperature * 1.8 + 320) * 10);    // °F
        #else
          jetiEx.SetSensorValue( ID_VARIO, uVario /= 0.3048, JEP_PRIO_STANDARD);                 // ft/s
          jetiEx.SetSensorValue( ID_PRESSURE, uPressure * 0.029529983071445, JEP_PRIO_ULTRA_LOW );  // inHG
          jetiEx.SetSensorValue( ID_TEMPERATURE, (uTemperature * 1.8 + 320) * 10, JEP_PRIO_ULTRA_LOW);    // °F
        #endif
      #else
        #ifdef SUPPORT_EX_BUS
          jetiEx.SetSensorValue( ID_VARIO, uVario);
          jetiEx.SetSensorValue( ID_PRESSURE, uPressure);
          jetiEx.SetSensorValue( ID_TEMPERATURE, uTemperature * 10);
        #else
          jetiEx.SetSensorValue( ID_VARIO, uVario, JEP_PRIO_STANDARD);
          jetiEx.SetSensorValue( ID_PRESSURE, uPressure, JEP_PRIO_ULTRA_LOW );
          jetiEx.SetSensorValue( ID_TEMPERATURE, uTemperature * 10, JEP_PRIO_ULTRA_LOW);
        #endif
      #endif
      
    }
    #endif

    #ifdef SUPPORT_RXQ
      #ifdef SUPPORT_EX_BUS
        // ID_SV_SIG_LOSS_CNT : count of servo signal gap measures > 100ms
        jetiEx.SetSensorValue( ID_SV_SIG_LOSS_CNT, ourServoSignalLossCount);
        // ID_SV_SIGNAL_GAP : time gap in ms between two servo pulses
        jetiEx.SetSensorValue( ID_SV_SIGNAL_GAP, ourServoSignalGap);
        // ID_SV_SIGNAL_GAP_MAX : maximum time gap in ms between two servo pulses
        jetiEx.SetSensorValue( ID_SV_SIGNAL_GAP_MAX, ourServoSignalGapMax);
        // number of servo signals / transmitted signals per second
        jetiEx.SetSensorValue( ID_SV_SIGNALS_PER_SECOND, ourServoSignalsPerSecond);
      #else
        // ID_SV_SIG_LOSS_CNT : count of servo signal gap measures > 100ms
        jetiEx.SetSensorValue( ID_SV_SIG_LOSS_CNT, ourServoSignalLossCount, JEP_PRIO_ULTRA_LOW);
        // ID_SV_SIGNAL_GAP : time gap in ms between two servo pulses
        jetiEx.SetSensorValue( ID_SV_SIGNAL_GAP, ourServoSignalGap, JEP_PRIO_STANDARD);
        // ID_SV_SIGNAL_GAP_MAX : maximum time gap in ms between two servo pulses
        jetiEx.SetSensorValue( ID_SV_SIGNAL_GAP_MAX, ourServoSignalGapMax, JEP_PRIO_ULTRA_LOW);
        // number of servo signals / transmitted signals per second
        jetiEx.SetSensorValue( ID_SV_SIGNALS_PER_SECOND, ourServoSignalsPerSecond, JEP_PRIO_STANDARD);
      #endif

    #endif

    lastTime = millis();

    // analog input
    #ifdef SUPPORT_MAIN_DRIVE
    if(currentSensor){
      
      // Voltage calculation
      float cuVolt = readAnalog_mV(getVoltageSensorTyp(),VOLTAGE_PIN)/1000.0;
      #ifdef SUPPORT_EX_BUS
        jetiEx.SetSensorValue( ID_VOLTAGE, cuVolt*10);
      #else
        jetiEx.SetSensorValue( ID_VOLTAGE, cuVolt*10, JEP_PRIO_STANDARD);
      #endif

      // Current calculation
      uint16_t ampOffset;

      if (currentSensor <= APM25_A){
        ampOffset = Atto_APM_offset;
      }else if (currentSensor > ACS758_200B){
        ampOffset = ACS_U_offset;
      }else{
        ampOffset = ACS_B_offset;
      }

      ampOffset += ampOffsetCalibration;

      float mVanalogIn = (analogRead(CURRENT_PIN) / 1024.0) * V_REF; // mV
      cuAmp = (mVanalogIn - ampOffset) / mVperAmp[currentSensor-1];
      if (currentSensor > APM25_A){
        cuAmp *= 5000.0/V_REF;
      }
      #ifdef SUPPORT_EX_BUS
        jetiEx.SetSensorValue( ID_CURRENT, cuAmp*10);
      #else
        jetiEx.SetSensorValue( ID_CURRENT, cuAmp*10, JEP_PRIO_STANDARD);
      #endif

      // Capacity consumption calculation
      capacityConsumption += cuAmp/timefactorCapacityConsumption;
      #ifdef SUPPORT_EX_BUS
        jetiEx.SetSensorValue( ID_CAPACITY, capacityConsumption);
      #else
        jetiEx.SetSensorValue( ID_CAPACITY, capacityConsumption, JEP_PRIO_ULTRA_LOW);
      #endif

      // save capacity and voltage to eeprom
      if(capacityMode > startup && (millis() - lastTimeCapacity) > CAPACITY_SAVE_INTERVAL){
        if(cuAmp <= MAX_CUR_TO_SAVE_CAPACITY){
          EEPROM.put(P_CAPACITY_VALUE, capacityConsumption);
          EEPROM.put(P_VOLTAGE_VALUE, cuVolt);
        }
        lastTimeCapacity = millis();
      }

      // Power calculation
      #ifdef SUPPORT_EX_BUS
        jetiEx.SetSensorValue( ID_POWER, cuAmp*cuVolt);
      #else
        jetiEx.SetSensorValue( ID_POWER, cuAmp*cuVolt, JEP_PRIO_ULTRA_LOW);
      #endif
    }
    #endif

    #ifdef SUPPORT_RX_VOLTAGE
    if(enableRx1){
      #ifdef SUPPORT_EX_BUS
        jetiEx.SetSensorValue( ID_RX1_VOLTAGE, readAnalog_mV(Rx1_voltage,RX1_VOLTAGE_PIN)/10);
      #else
        jetiEx.SetSensorValue( ID_RX1_VOLTAGE, readAnalog_mV(Rx1_voltage,RX1_VOLTAGE_PIN)/10, JEP_PRIO_LOW);
      #endif
    }

    if(enableRx2){
      #ifdef SUPPORT_EX_BUS
        jetiEx.SetSensorValue( ID_RX2_VOLTAGE, readAnalog_mV(Rx2_voltage,RX2_VOLTAGE_PIN)/10);
      #else
        jetiEx.SetSensorValue( ID_RX2_VOLTAGE, readAnalog_mV(Rx2_voltage,RX2_VOLTAGE_PIN)/10, JEP_PRIO_LOW);
      #endif
    }
    #endif

    // temperature calculation
    #ifdef SUPPORT_EXT_TEMP
    if(enableExtTemp){
      // convert the value to resistance
      float aIn = 1023.0 / analogRead(TEMPERATURE_PIN) - 1.0;
      aIn = SERIESRESISTOR / aIn;

      // convert resistance to temperature
      float steinhart;
      steinhart = aIn / THERMISTORNOMINAL;                // (R/Ro)
      steinhart = log(steinhart);                         // ln(R/Ro)
      steinhart /= BCOEFFICIENT;                          // 1/B * ln(R/Ro)
      steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15);   // + (1/To)
      steinhart = 1.0 / steinhart;                        // Invert
      steinhart -= 273.15 - SELF_HEAT;                    // convert to °C and self-heat compensation

      #ifdef UNIT_US
        // EU to US conversions
        steinhart = steinhart * 1.8 + 320;
      #endif
      #ifdef SUPPORT_EX_BUS
        jetiEx.SetSensorValue( ID_EXT_TEMP, steinhart*10);
      #else
        jetiEx.SetSensorValue( ID_EXT_TEMP, steinhart*10, JEP_PRIO_ULTRA_LOW);
      #endif
    }
    #endif
  }

  #ifdef SUPPORT_LSM303
    if (compassEnabled && compassInitalized) {
      compass.read();
      /*
      to use the +Z axis as a reference.
      */
      float heading = compass.heading();
      if (compassPos == -1) {
        // save actual value as home position
        compassPos = heading;
      }
      float relCompass = heading - compassPos;
      if (relCompass < 0.0) {
        relCompass = 360.0 + relCompass;
      }
      #ifdef SUPPORT_EX_BUS
        jetiEx.SetSensorValue( ID_COMPASS, heading * 10);
        jetiEx.SetSensorValue( ID_REL_COMPASS, relCompass);
      #else
        jetiEx.SetSensorValue( ID_COMPASS, heading * 10, JEP_PRIO_STANDARD);
        jetiEx.SetSensorValue( ID_REL_COMPASS, relCompass, JEP_PRIO_STANDARD);
      #endif
      if (trainingMode) {
        int orientation = ((int) relCompass) % 90;
        if (orientation > 0) {
          orientation = 0;
        } else {
          orientation = 1;
        }
        #ifdef SUPPORT_EX_BUS
          jetiEx.SetSensorValue( ID_TRAINING_ORIENTATION, orientation);
        #else
          jetiEx.SetSensorValue( ID_TRAINING_ORIENTATION, orientation, JEP_PRIO_ULTRA_HIGH);
        #endif
      }
    }
  #endif

  #ifdef SUPPORT_GPS
  if(gpsSettings.mode != GPS_disabled){
    unsigned long distToHome;

    // read data from GPS
    while(gpsSerial.available() )
    {
      char c = gpsSerial.read();
      if(gps.encode(c)){
        break;
      }else{
        return;
      }
    }

    // if GPS-Fix
    if (gps.location.isValid() && gps.location.age() < 2000) { 

      
      uint8_t locationPrio = JEP_PRIO_LOW;
      #ifdef GPS_LOCATION_PRIO_HIGH
      locationPrio = JEP_PRIO_HIGH;
      #endif
      // Position
      jetiEx.SetSensorValueGPS( ID_GPSLAT, false, gps.location.lat(), locationPrio );
      jetiEx.SetSensorValueGPS( ID_GPSLON, true, gps.location.lng(), locationPrio );

      // Altitude
      uAbsAltitude = gps.altitude.meters();

      // GPS TEC
      #ifdef SUPPORT_TEC
      if(TECmode == TEC_GPS){
        if (gps.speed.isUpdated()) {
          uSpeedMS = gps.speed.kmph() / 3.6;
          dV = uSpeedMS - lastGPSspeedMS;     // delta speed in m/s
          lastGPSspeedMS = uSpeedMS;
          long uTimeSpeed = millis();
          dT = uTimeSpeed - lastTimeSpeed; // dT in ms
          lastTimeSpeed = uTimeSpeed;
        }
        
      }
      #endif

      // Speed
      // dynamic Jeti transfer rate: at high speed higher prio
      uint8_t prio = JEP_PRIO_LOW;
      double speed = gps.speed.mps();
      if (speed > 30.0 ) {
         prio = JEP_PRIO_HIGH;
      }
      
      #ifdef UNIT_US
        jetiEx.SetSensorValue( ID_GPSSPEED, gps.speed.mph(), prio);
      #else
        jetiEx.SetSensorValue( ID_GPSSPEED, gps.speed.kmph(), prio);
      #endif

      #ifdef SUPPORT_GPS_EXTENDED
      // Course
      jetiEx.SetSensorValue( ID_COURSE, gps.course.deg(), JEP_PRIO_ULTRA_LOW );
      #endif

      // set home position
      if (homeSetCount < 3000) {  
        ++homeSetCount;
        home_lat = gps.location.lat();
        home_lon = gps.location.lng();
        last_lat = home_lat;
        last_lon = home_lon;
        lastAbsAltitude = gps.altitude.meters();
        tripDist = 0;
        if(pressureSensor.type == unknown){
          startAltitude = gps.altitude.meters();
        }

      }else{

        // Rel. Altitude
        if(pressureSensor.type == unknown){
          uRelAltitude = (uAbsAltitude - startAltitude)*10;
        }

        // Distance to model
        distToHome = gps.distanceBetween(
                                      gps.location.lat(),
                                      gps.location.lng(),
                                      home_lat,
                                      home_lon);
        if(gpsSettings.distance3D){
          distToHome = sqrt(pow(uRelAltitude/10,2) + pow(distToHome,2));
        }

        #ifdef SUPPORT_GPS_EXTENDED
        // Azimuth (course from home to model)
        jetiEx.SetSensorValue( ID_AZIMUTH, gps.courseTo(home_lat,home_lon,gps.location.lat(),gps.location.lng()), JEP_PRIO_ULTRA_LOW);
        #endif

        // Trip distance
        float distLast = gps.distanceBetween(
                                      gps.location.lat(),
                                      gps.location.lng(),
                                      last_lat,
                                      last_lon);
        if(gpsSettings.distance3D){
          distLast = sqrt(pow(uAbsAltitude-lastAbsAltitude,2) + pow(distLast,2));
          lastAbsAltitude = uAbsAltitude;
        }
        tripDist += distLast;
        last_lat = gps.location.lat();
        last_lon = gps.location.lng();
      }

    } else { // If Fix end
      jetiEx.SetSensorValueGPS( ID_GPSLAT, false, 0, JEP_PRIO_LOW );
      jetiEx.SetSensorValueGPS( ID_GPSLON, true, 0, JEP_PRIO_LOW );
      if(pressureSensor.type == unknown){
        uRelAltitude = 0;
        uTrainingAltitude = 0;
      }
      uAbsAltitude = 0;
      distToHome = 0;
      jetiEx.SetSensorValue( ID_GPSSPEED, 0, JEP_PRIO_LOW );
      #ifdef SUPPORT_GPS_EXTENDED
      jetiEx.SetSensorValue( ID_COURSE, 0, JEP_PRIO_ULTRA_LOW );
      jetiEx.SetSensorValue( ID_AZIMUTH, 0, JEP_PRIO_ULTRA_LOW );
      #endif
    }

    #ifdef SUPPORT_GPS_EXTENDED
    jetiEx.SetSensorValue( ID_SATS, gps.satellites.value(), JEP_PRIO_ULTRA_LOW );
    jetiEx.SetSensorValue( ID_HDOP, gps.hdop.value(), JEP_PRIO_ULTRA_LOW);
    #endif
    #ifndef UNIT_US
      //EU units
      #ifdef SUPPORT_GPS_EXTENDED
      jetiEx.SetSensorValue( ID_TRIP, tripDist/10 , JEP_PRIO_ULTRA_LOW);
      #endif
      jetiEx.SetSensorValue( ID_DIST, distToHome, JEP_PRIO_ULTRA_LOW );
    #endif
    #ifdef UNIT_US
      //US units
      #ifdef SUPPORT_GPS_EXTENDED
      jetiEx.SetSensorValue( ID_TRIP, tripDist*0.06213, JEP_PRIO_ULTRA_LOW );
      #endif
      jetiEx.SetSensorValue( ID_DIST, distToHome*3.2808399, JEP_PRIO_ULTRA_LOW);
    #endif

    #ifdef UNIT_US
      // EU to US conversions
      // ft/s = m/s / 0.3048
      // inHG = hPa * 0,029529983071445
      // ft = m / 0.3048
      uRelAltitude /= 0.3048;
      uAbsAltitude /= 0.3048;
      uTrainingAltitude /= 0.3048;
    #endif

  }
  #endif

  #ifndef NO_ALTITUDE_VALUES
    #ifdef SUPPORT_EX_BUS
      jetiEx.SetSensorValue( ID_ALTREL, uRelAltitude);
      jetiEx.SetSensorValue( ID_ALTABS, uAbsAltitude);
      jetiEx.SetSensorValue( ID_TRAINING_ALTIT, uTrainingAltitude);
    #else
      jetiEx.SetSensorValue( ID_ALTREL, uRelAltitude, JEP_PRIO_STANDARD);
      jetiEx.SetSensorValue( ID_ALTABS, uAbsAltitude, JEP_PRIO_ULTRA_LOW);
      jetiEx.SetSensorValue( ID_TRAINING_ALTIT, uTrainingAltitude, JEP_PRIO_ULTRA_HIGH);
    #endif
  #endif
  
  #ifdef SUPPORT_MS5611
  if (pressureSensor.type == MS5611_) {
    ms5611.run();
    // to get a good responsiveness of the variometer value get set the variometer value very often
    static unsigned long lastVarioTime = 0;
    if (millis() > lastVarioTime + 10) {
      setFastVariometerValues();
      lastVarioTime = millis();
    }
  } 
  #endif


  #ifdef SUPPORT_JETIBOX_MENU
  HandleMenu();
  #endif

  #ifdef SUPPORT_EX_BUS
  //}
  jetiEx.DoJetiExBus();
  #else
  jetiEx.DoJetiSend();
  #endif
  
}

#ifdef SUPPORT_MS5611
void setFastVariometerValues() {
  if (pressureSensor.type == MS5611_) {
    int variometer = ms5611.getVerticalSpeed();
    #ifndef SUPPORT_EX_BUS
      uint8_t prio = JEP_PRIO_ULTRA_HIGH;
        #ifdef SUPPORT_GPS
        // dynamic Jeti transfer rate: at high speed higher prio
        double speed = gps.speed.mps();
        if (speed > 30.0 ) {
          prio = JEP_PRIO_STANDARD;
        }
        #endif
    #endif    
    #ifdef UNIT_US
      // EU to US conversions
      // ft/s = m/s / 0.3048
      // inHG = hPa * 0,029529983071445
      // ft = m / 0.3048
      variometer = variometer /= 0.3048
    #endif
    #ifdef SUPPORT_EX_BUS
      jetiEx.SetSensorValue( ID_VARIO, variometer); // m/S
    #else
      jetiEx.SetSensorValue( ID_VARIO, variometer, prio); // m/S
    #endif
  }
}
#endif
