#!/bin/bash

# supported boards are
#  BOARD="PRO_MINI_16MHZ"
#  BOARD="PRO_MINI_8MHZ"
#  BOARD="NODE_MCU_1.0"

MYDIR=$(dirname $0)

if [ -f $MYDIR/prj.ini ] ; then
  source $MYDIR/prj.ini 
fi

USBPort=$( ls /dev/ttyUSB[0123456789] )

if [ "${USBPort:5:3}" != "tty" ] ; then 
  USBPort=""
fi

echo "USBPort: $USBPort"

TEMP_FOLDER=/tmp/arduino_build_make

if [ ! -d $TEMP_FOLDER ] ; then
  mkdir $TEMP_FOLDER
fi

function connectBoard {
  minicom -b 115200 -D $USBPort
}

function usage {
  echo "usage: make [options] "
  echo "       options:"
  echo "         build : only build process is triggered"
  echo "         upload: build and upload "
  echo "         connect: build and upload and start minicom"
  echo "         all: build and upload and start minicom"
  echo "         clean: cleanup all temp files"
  echo "         html: create patched html files in html/ folder "
}

function compile {
  PM_GCC_PATH=$( find $HOME/.arduino* -type f -name "avr-g++" )
  PM_GCC_PATH=${PM_GCC_PATH%/bin/avr-g++}
  PM_DUDE_PATH=$( find $HOME/.arduino* -type f -name "avrdude" )
  PM_DUDE_PATH=${PM_DUDE_PATH%/bin/avrdude}
  PM_OTA_PATH=$( find $HOME/.arduino* -type f -name "arduinoOTA" )
  PM_OTA_PATH=${PM_OTA_PATH%/bin/arduinoOTA}

  ESP_PYTHON_PATH=$( find $HOME/.arduino* -type f -name "placeholder_for_arduino" )
  ESP_PYTHON_PATH=${ESP_PYTHON_PATH%/placeholder_for_arduino}
  ESP_TOOLS_PATH=$( find $HOME/.arduino*/packages/esp8266/hardware/esp8266/ -type f -name "upload.py" )
  ESP_TOOLS_PATH=${ESP_TOOLS_PATH%/upload.py}

  echo "compiling for board : $BOARD"
  if [ $BOARD = "NODE_MCU_1.0" ] ; then
    # /usr/share/arduino/arduino-builder -dump-prefs -logger=machine -hardware /usr/share/arduino/hardware -hardware $HOME/.arduino15/packages -tools /usr/share/arduino/tools-builder -tools $HOME/.arduino15/packages -built-in-libraries /usr/share/arduino/libraries -libraries $HOME/Links/stransky/dev/arduino/libraries -fqbn=esp8266:esp8266:nodemcuv2:xtal=80,vt=flash,exception=disabled,ssl=all,eesz=4M,ip=lm2f,dbg=Disabled,lvl=None____,wipe=none,baud=115200 -ide-version=10805 -build-path /tmp/arduino_build_422359 -warnings=default -prefs=build.warn_data_percentage=75 -prefs=runtime.tools.python.path=$HOME/.arduino15/packages/esp8266/tools/python/3.7.2-post1 -prefs=runtime.tools.xtensa-lx106-elf-gcc.path=$HOME/.arduino15/packages/esp8266/tools/xtensa-lx106-elf-gcc/2.5.0-3-20ed2b9 -prefs=runtime.tools.mkspiffs.path=$HOME/.arduino15/packages/esp8266/tools/mkspiffs/2.5.0-3-20ed2b9 -verbose $HOME/Links/stransky/dev/arduino/esp8266/KlebeTester/KlebeTester.ino
    /usr/share/arduino/arduino-builder -compile -hardware /usr/share/arduino/hardware -hardware $HOME/.arduino15/packages -tools /usr/share/arduino/tools-builder -tools $HOME/.arduino15/packages -built-in-libraries /usr/share/arduino/libraries -libraries $HOME/Links/stransky/dev/arduino/libraries -fqbn=esp8266:esp8266:nodemcuv2:xtal=80,vt=flash,exception=disabled,ssl=all,eesz=4M,ip=lm2f,dbg=Disabled,lvl=None____,wipe=none,baud=115200 -build-path $TEMP_FOLDER -warnings=default -prefs=build.warn_data_percentage=75 -prefs=runtime.tools.python.path=$ESP_PYTHON_PATH -prefs=runtime.tools.xtensa-lx106-elf-gcc.path=$HOME/.arduino15/packages/esp8266/tools/xtensa-lx106-elf-gcc/2.5.0-3-20ed2b9 -prefs=runtime.tools.mkspiffs.path=$HOME/.arduino15/packages/esp8266/tools/mkspiffs/2.5.0-3-20ed2b9 ./*.ino
  elif [ $BOARD =  "PRO_MINI_16MHZ" ] ; then
    /usr/share/arduino/arduino-builder -compile -hardware /usr/share/arduino/hardware -hardware $HOME/.arduino15/packages -tools /usr/share/arduino/tools-builder -tools $HOME/.arduino15/packages -built-in-libraries /usr/share/arduino/libraries -libraries $HOME/Links/stransky/dev/arduino/libraries -fqbn=arduino:avr:pro:cpu=16MHzatmega328 -ide-version=10805 -build-path $TEMP_FOLDER -warnings=default -prefs=build.warn_data_percentage=75 -prefs=runtime.tools.avr-gcc.path=$PM_GCC_PATH -prefs=runtime.tools.avrdude.path=$PM_DUDE_PATH -prefs=runtime.tools.arduinoOTA.path=$PM_OTA_PATH -verbose $MYDIR/*.ino

  elif [ $BOARD =  "PRO_MINI_8MHZ" ] ; then
    /usr/share/arduino/arduino-builder -compile -hardware /usr/share/arduino/hardware -hardware $HOME/.arduino15/packages -tools /usr/share/arduino/tools-builder -tools $HOME/.arduino15/packages -built-in-libraries /usr/share/arduino/libraries -libraries $HOME/Links/stransky/dev/arduino/libraries -fqbn=arduino:avr:pro:cpu=8MHzatmega328 -ide-version=10805 -build-path $TEMP_FOLDER -warnings=default -prefs=build.warn_data_percentage=75 -prefs=runtime.tools.avr-gcc.path=$PM_GCC_PATH -prefs=runtime.tools.avrdude.path=$PM_DUDE_PATH -prefs=runtime.tools.arduinoOTA.path=$PM_OTA_PATH -verbose $MYDIR/*.ino
  fi
}

function upload {
  INOFile=$( ls $TEMP_FOLDER/*.ino.hex )
  echo "INO: $INOFile"
  if [ $BOARD =  "NODE_MCU_1.0" ] ; then
    $ESP_PYTHON_PATH/python $ESP_TOOLS_PATH/upload.py --chip esp8266 --port $USBPort --baud 115200 --trace version --end --chip esp8266 --port $USBPort --baud 115200  write_flash 0x0 $TEMP_FOLDER/*.ino.bin --end
  elif [ $BOARD =  "PRO_MINI_16MHZ" ] ; then
    $PM_DUDE_PATH/bin/avrdude -C$PM_DUDE_PATH/etc/avrdude.conf -v -V -patmega328p -carduino -P$USBPort -b57600 -D -Uflash:w:${INOFile}:i
  elif [ $BOARD =  "PRO_MINI_8MHZ" ] ; then
    $PM_DUDE_PATH/bin/avrdude -C$PM_DUDE_PATH/etc/avrdude.conf -v -V -patmega328p -carduino -P$USBPort -b57600 -D -Uflash:w:${INOFile}:i
  fi
}

function createHTML {
  for file in $htmlPages ; do
    echo $file
  done

}

DO_BUILD=1
DO_UPLOAD=2
DO_CONNECT=3
DO_CLEAN=10

DOIT=0

if [ $# -eq 0 ] ; then
  echo "build and upload sketch"
  DOIT=3
elif [ $# -eq 1 ] ; then
  if [ "$1" = "all" ] ; then	
    echo "build and upload sketch and connect console"
    DOIT=3
  fi
  if [ "$1" = "connect" ] ; then	
    echo "connect to arduino"
    DOIT=9
  fi
  if [ "$1" = "upload" ] ; then	
    echo "build and upload sketch"
    DOIT=2
  fi
  if [ "$1" = "build" ] ; then	
    echo "build sketch"
    DOIT=1
  fi
  if [ "$1" = "clean" ] ; then	
    echo "cleanup temporary files"
    DOIT=10
  fi
  if [ "$1" = "html" ] ; then	
    echo "creating html files ..."
    createHTML
    exit 0
  fi
fi


if [ $DOIT -eq 0 ] ; then
    echo "do nothing, not valid option given"
    exit -1
fi

if [ $DOIT -eq 9 ] ; then
    echo "connect to USB port: $USBPort"
    connectBoard
    exit 0
fi

if [ $DOIT -eq 10 ] ; then
    echo "cleanup of temporary folders"
    rm -rf $TEMP_FOLDER
    rm -f *.ino.elf *.ino.bin *.ino.eep *.ino.hex
    exit 0
fi

if [ $DOIT -gt 1 -a  -z "$USBPort"  ] ; then
    echo "USB Port not connected"
    exit -1
else 
    echo "using USB Port: $USBPort"
fi


if [ $DOIT -gt 0 ] ; then
  compile 
  if [ $? -ne 0 ] ; then
    echo "result : $?"
    exit -1;
  fi
fi

if [ $DOIT -gt 1 ] ; then
  upload 
  if [ $? -ne 0 ] ; then
    echo "result : $?"
    exit -1;
  fi
sleep 2

fi
if [ $DOIT -gt 2 ] ; then
  connectBoard
fi
