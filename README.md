# VarioGPS-Sensor

Universeller Jeti Telemetrie Sensor mit vielen Möglichkeiten: Vario(TEK), GPS, Strom/Spannung/Kapazität/Leistung für Hauptantrieb, Air Speed mit Staudruckrohr, Empfängerspannung und Temperaturmessung. Der Sensor ist total einfach nachbaubar, und sollte auch von Elektronik-Anfängern problemlos zu bewerkstelligen sein. 



## neue Features
Version 2.3.6
* da trotz der deutlich verbesserten MS5611 Variometer Werte in der Version V2.3.5, die Übertragung zum Sender deutlich verzögert war, ergab eine Problem-Analyse eine deutliche Überlastung der mit 9600Baud arbeitenden JetiExSensor-Schnittstelle, bei gleichzeitiger Nutzung von RXQ, GPS und Vario, da die JetiExSensor-Library alle Werte gleichwertig überträgt. Die große Anzahl an Sensor-Werten (z.B. eines GPS Moduls (im Modus Extended) ) ergab eine Variometer-Wert-Übertragung von ca. 1 Wert/s. 
* Lösung: Priorisierbare Sensor-Wert Schnittstelle
** Durch die Erweiterung der Bibliothek JetiExSensor (aktueller Code im Zip-File (oder unter https://github.com/Pulsar07/JetiExSensor ), um eine priorisierbare Sensor-Wert-übertragung und die Vergrößerung der Sendefrequenz, konnten mehr als 20 Telemetrie-Werte mehr oder weniger häufig, je nach Wichtigkeit, übertragen werden und gleichzeitig noch  6-10 Variometer-Werte/s, abhängig von Anzahl anderer Telemetriewerte.
** Ein kleiner Nachteil ist, dass Werte die selten übertragen werden (z.B. Luftdruck, nur jeden 15.Frame) im Display des Jeti-Senders manchmal blinkend dargestellt werden, da die Werte nicht schnell genug (für den Sender) aktualisiert werden.
* aufgrund des grenzwertigen Heap-Speicherverbrauchs, wurden viele Code-Bereiche nun mit bedingter Compilierung versehen, ohne jedoch die Sensor-IDs zu ändern. 
* Vergleich zwischen den VarioGPS-Sensor V2.3.4, V2.3.5 und V2.3.6 geloggten Variowerten, jeweils zu einem parallel aktivierten JETI-REX-Assist-Vario:
    * Vergleich mit originalem VarioGPS-Sensor V2.3.4 eines MS5611 Sensors (ohne VarioMS5611 Lib und ohne verbesserter Telemetrieübertragung) mit einem JETI-REX-Assist-Vario:
    * ![VarioGPS-SensorV2.3.4](https://raw.githubusercontent.com/Pulsar07/Jeti_VarioGPS-Sensor/master/Doc/img/JGV_2.3.4_0.88_1.1VSps.png)
    * Vergleich mit VarioGPS-Sensor V2.3.5 eines MS5611 Sensors (mit neuer VarioMS5611 Lib aber ohne verbesserter Telemetrieübertragung) mit einem JETI-REX-Assist-Vario:
    * ![VarioGPS-SensorV2.3.5](https://raw.githubusercontent.com/Pulsar07/Jeti_VarioGPS-Sensor/master/Doc/img/JGV_2.3.5_0.96_1.3VSps.png)
    * Vergleich mit VarioGPS-Sensor V2.3.6 eines MS5611 Sensors (mit neuer VarioMS5611 Lib und mit verbesserter Telemetrieübertragung) mit einem JETI-REX-Assist-Vario:
    * ![VarioGPS-SensorV2.3.6](https://raw.githubusercontent.com/Pulsar07/Jeti_VarioGPS-Sensor/master/Doc/img/JGV_2.3.6_0.96_6.5VSps.png)


Version 2.3.5
* Neue VarioMS5611 Library für geringeres Rauschen und deutlich besseres Signal-Rausch-Verhältnis
    * Library : VarioMS5611 unter https://github.com/Pulsar07/VarioMS5611 verfügbar initial auch in der VarioGPS_Libraries.zip enthalten
    * deutliche Verbesserung des Signal-Rausch-Verhältnisses und ca. um Faktor 3 verbessertes Signalrauschen
    * ![VarioMS5611-Signalvergleich](https://raw.githubusercontent.com/Pulsar07/Jeti_VarioGPS-Sensor/master/Doc/img/VergleichVarioSignal.png)
    * erreicht wird diese Verbesserung durch deutliche Steigerung der Oversampling-Rate von ca. 5*4096 auf 40*4096 Samples bei gleichzeitig verkleinertem/minimiertem Laufzeitimpakt durch Vermeidung von jeglichem delay() bei der Kommunikation mit dem MS5611 (kooperatives run() in der main loop()). Damit kann das Signalrauschen deutlich besser gedämpft werden ohne die Reaktionszeit zu verschlechtern.
    * Feature wird weiterhin mittels #define SUPPORT_MS5611 aktiviert und ersetzt die bisherige MS5611 Implementierung

Version 2.3.4
* RXQ - Empfangsqualität, durch Messung von PWM Impuls eines Servoausgangs
    * #define SUPPORT_RXQ : muss in setting.h definiert werden
    * Es muss Arduion PIN 2 mit einem Servo-Ausgang verbunden werden. Ohne 47kOhm Widerstand muss noch eein #define RXQ_SERVO_PIN_PULLUP in setting.h definiert werden.
    * Siehe auch : http://www.so-fa.de/nh/JetiSensorRXQ

## Telemetrie

![EX-vario](https://raw.githubusercontent.com/nightflyer88/Jeti_VarioGPS-Sensor/master/Doc/img/EX_vario.bmp)
![EX-GPS](https://raw.githubusercontent.com/nightflyer88/Jeti_VarioGPS-Sensor/master/Doc/img/EX_gps.bmp)
![EX-GPS2](https://raw.githubusercontent.com/nightflyer88/Jeti_VarioGPS-Sensor/master/Doc/img/EX_gps2.bmp)
![EX-VA](https://raw.githubusercontent.com/nightflyer88/Jeti_VarioGPS-Sensor/master/Doc/img/EX_volt_amp.bmp)

Nur mit Barometersensor werden folgende Werte angezeigt:
- Rel. und Abs. Höhe
- Vario
- Luftdruck
- Temperatur
- Luftfeuchte (nur mit BME280)
  
Im GPS Basic Mode sind folgende Werte verfügbar:
- Position
- GPS speed (kann auch zur Vario-TEK Kompensation verwendet werden) 
- Rel. und Abs. Höhe
- Vario
  
Im GPS Extended Mode werden zusätzlich die Werte angezeigt:
- Distanz vom Modell zum Startpunkt (2D oder 3D)
- zurückgelegte Strecke (Trip)
- Flugrichtung (Azimut)
- Kurs vom Modell zum Startpunkt
- Anzahl Satelliten
- HDOP (Horizontaler Faktor der Positionsgenauigkeit)
- Luftdruck
- Temperatur
- Luftfeuchtigkeit 

Durch einen Air Speed Sensor mit Staudruckrohr sind die Werte verfügbar:
- Air speed
- Vario-TEK Kompensation

Mit einem Strom/Spannungssensor können folgende Werte angezeigt werden:
- Strom
- Spannung
- Leistung
- Kapazität (wird gespeichert und automatisch bei vollem Akku zurückgesetzt -> es können dadurch mehrere Flüge mit dem gleichen Akku gemacht werden)

Für die Messung der Empfängerspannung stehen die Werte zur Verfügung:
- Rx1 voltage
- Rx2 voltage

Zur Messung von zB. der Motorentemperatur kann zusätzlich ein NTC-Temperaturwiederstand (-55 bis +155°C) angeschlossen werden.

### RXQ - Empfangsqualität
Für die Messung der Empfangsqualität (RXQ) muss ein Kabel vom Arduino Pin 2 mit einem 47kOhm Vorwiderstand zu einem freien Servoausgang hergestellt werden. Am Sender wird z.B. 100Hz Sendefrequenz eingestellt.

Beim Empfänger muss die Servo-Output-Period auf "AUTO" eingestellt sein, damit die PWM Signaldauer nicht vom Empfänger erzeugt wird. Zudem muss beim benutzten Empfängerausgang die Fail-Safe-Einstellung auf "AUS" eingestellt sein, damit im Problemfall, das Servosignal nicht vom RX erzeugt wird.

Der Arduino misst nun die Zeitabstände zwischen den am Servoausgang ausgegebenen Servosignale (PWM) (und zwar alle !!!), was bei 100Hz 10ms sein sollte.
Mehr Details und Erläuterungen sind unter: http://www.so-fa.de/nh/JetiSensorRXQ zu finden.


Für die Empfangsqualität durch Messung eines Servosignalausgangs stehen die Werte zur Verfügung:
- SigGap : aktuelle Zeitspanne zwischen den PWM Signalen in ms (10ms bei 100Hz)
- SigGapMax : maximale gemessene Zeitspanne zwischen den PWM Signalen (lückenlos überwacht !!!)
- SigLossCnt : werden Abstände > 100ms dedektiert, wird der SigLossCnt um 1 erhöht (sollte immer 0 sein !)
- #Sig/Sec : Anzahl von PWM Signalen pro Sekunde

Zum Aktivieren dieses Features Code sind folgende #defines in der Datei defaults.h gedacht:
- #define SUPPORT_RXQ : schaltet Feature im Code ein
- #define RXQ_SIGNAL_PIN : (default: 2) definiert den zu nutzenden Arduino Eingangs-Pin
- #define RXQ_SERVO_PIN_PULLUP : (default: "not defined") dies muss definiert sein, wenn der RX ohne Vorwiderstand angeschlossen wird

Die Erfahrung der letzten 2 Jahre, ca. 15 Modellen, Abstand zum Modell bis 1500m, bei gut verlegten Antennen, zeigt, dass selbst bei Sender-Signalverlustmeldungen, die SigDuraMax immer unter 100ms bleibt, was zeigt, dass die Jeti-Signalverlustmeldung eben meist am Verlust des Rückkanals liegt!

  
## JetiBox Einstellungen

![JetiBox](https://raw.githubusercontent.com/nightflyer88/Jeti_VarioGPS-Sensor/master/Doc/img/JetiBox_settings.png)

Folgende Einstellungen können per Jetibox vorgenommen werden:
- GPS: deaktiviert, Basic oder Extended
- GPS Distanz: 2D oder 3D
- Vario Filterparameter: Empfindlichkeit und Totzone
- Air Speed Sensor
- TEK Kompensation
- Stromsensor für Hauptantrieb 
- Einstellung Reset der Kapazität:
    - STARTUP(Wert ist nach jedem Einschalten auf 0)
    - AUTO(Wert wird gespeichert und erst zurückgesetzt wenn ein geladener Akku angeschlossen wird)
    - MANUAL(Wert muss manuell per Jetibox zurückgesetzt werden mit RESET OFFSET)
- Rx1, Rx2 Empfängerspannungsmessung aktivieren
- Temperaturmessung aktivieren

## Hardware

- Arduino Pro Mini 3.3V-8Mhz oder 5V-16Mhz
- GPS-Modul mit NMEA Protokoll und UART@9600baud
- Barometer Sensoren: BMP280, BME280, MS5611, LPS 
- Airspeed Sensoren: MPXV7002, MPXV5004
- Stromsensoren @3.3V/5V Betriebsspannung:
    - AttoPilot Module @3.3V: 45A/13.6V - 90A/50V - 180A/50V (@5V: 45A/20.6V - 90A/60V - 180A/60V)
    - APM2.5 PowerModul @5V: 90A/50V (@3.3V: 58A/33.4V)
    - ACS758_50B, ACS758_100B, ACS758_150B, ACS758_200B, ACS758_50U, ACS758_100U, ACS758_150U, ACS758_200U
- zusätzliche Stromsensoren @5V Betriebsspannung: ACS712_05, ACS712_20, ACS712_30

## Aufbau

Der VarioGPS Sensor kann individuell nach seinen eigenen Wünschen zusammengestellt werden. Es ist möglich den Sensor als reines Vario zu betreiben, nur zur Vermessung des Antriebs, oder als Überwachung der Empfängerstromversorgung. Die benötigten Sensoren werden einfach am Arduino angelötet, und per Jetibox aktiviert.

![schematic](https://raw.githubusercontent.com/nightflyer88/Jeti_VarioGPS-Sensor/master/Doc/img/VarioGPS_schematic.png)

Zu beachten gibt es folgendes: 
- Die Barometer Sensoren sind empfindlich gegen Licht und sollten deshalb in schwarzem Schrumpfschlauch eingepackt werden
- Die GPS-Module vertragen in der Regel nur eine Beschleunigung von 4G.

## Firmware laden

Je nach verwendetem Arduino Board wird die [Firmware](https://github.com/nightflyer88/Jeti_VarioGPS-Sensor/releases) für 3.3V oder 5V benötigt. Im Ordner Arduino/Firmware sind die entsprechenden hex-files die direkt auf das Arduino Board geladen werden können. Für fortgeschrittene Benutzer steht auch der komplette Code zur Verfügung, der mit der Arduino IDE auf seine eigene Bedürfnisse angepasst werden kann.

Für den Firmware upload wird ein USB<>serial Adapter benötigt, und ein entsprechendes Upload-Programm. Danach hat man's schon geschaft, und kann den Sensor im Modell einbauen.

Treten nach einem Firmwareupdate Probleme auf, sollten per JetiBox mit "Load defaults" die Einstellungen zurückgesetzt, und danach neu eingestellt werden.

### MAC

Für MAC User gibt es das Programm [HexUploader](https://github.com/nightflyer88/HexUploader/releases), dies ist ein simples Programm zum laden der Firmware. Einfach das Arduino Board per USB anschliessen, Arduino Type "Pro mini" und den USB-Serial Port auswählen. Nun das entsprechende hex-file auswählen und der upload startet automatisch.

![hexuploader2](https://raw.githubusercontent.com/nightflyer88/Jeti_VarioGPS-Sensor/master/Doc/img/HexUploader.png)

### Windows

Für Windows User kann der [XLoader](http://russemotto.com/xloader/XLoader.zip) verwendet werden. Ist ebenfalls ein simples Programm, einfach das hex-file, device und COM-port auswählen und auf "Upload" drücken.

![xloader](https://raw.githubusercontent.com/nightflyer88/Jeti_VarioGPS-Sensor/master/Doc/img/xloader.png)
