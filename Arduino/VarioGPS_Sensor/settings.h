/*
  -----------------------------------------------------------
            Settings 
  -----------------------------------------------------------
  General settings for the VarioGPS sensor.
*/

//#define UNIT_US                             // uncomment to enable US units

#define V_REF                     5000        // set supply voltage from 1800 to 5000mV
#define JETI_SEND_CYCLE           75          // in ms to wait for sending the next telemetry frame, 75 is minimum, 150 is default
                                              // low values force lower latency
                                              // low values my force progems with the hardware JETIBOX. JETIBOX emulation in TX has no problems

// #define GPSBaud 9600                         // here a different interface speed to the GPS module can be specified up to 31250baud
// #define NO_ALTITUDE_VALUES                   // define this to disable altitude transmission to the Jeti telemetry interface
// #define GPS_LOCATION_PRIO_HIGH               // define this to increase the GPS location values prio at  the Jeti telemetry interface

// **** supported devices, comment to disable ****
// Turn on the compiler warnings in the arduino settings to indicate compatibility issues

//#define SUPPORT_EX_BUS                        // enable bidirectional EX Bus with 125kbaud, otherwise the old EX protocol with 9.6kbaud is enabled

//#define SUPPORT_BMx280                        // BMP280 and BME280 pressure sensors for altitude
#define SUPPORT_MS5611                        // MS5611 pressure sensor for altitude
//#define SUPPORT_LPS                           // LPSxxx (from STmicroelectronics) pressure sensors for altitude 

#define SUPPORT_LSM303                        // compass modul GY-511 / LSM303

//#define SUPPORT_GPS                           // GPS modul
//#define SUPPORT_GPS_EXTENDED                    // additional low prio sensor values
//#define SUPPORT_MPXV7002_MPXV5004             // MPXV7002 or MPXV5004 pressure sensor for airspeed
//#define SUPPORT_TEC                           // Vario TEC compensation

//#define SUPPORT_MAIN_DRIVE                    // current and voltage sensors
//#define SUPPORT_RX_VOLTAGE                    // Rx voltage sensors

//#define SUPPORT_EXT_TEMP                     // NTC temperature sensor
// #define SUPPORT_RXQ                          // cable to a RX servo output can provide receiver (RX) qaulitiyi(Q) information 

#define SUPPORT_JETIBOX_MENU                  // JetiBox menu
