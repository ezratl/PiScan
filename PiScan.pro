# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

#TARGET = PiScan

HEADERS = \
   $$PWD/src/interface/display/graphic_lcd/display.h \
   $$PWD/src/interface/display/graphic_lcd/drv_pipe.h \
   $$PWD/src/interface/display/graphic_lcd/font.h \
   $$PWD/src/interface/display/graphic_lcd/font_6x8.h \
   $$PWD/src/interface/display/graphic_lcd/font_6x8_bold.h \
   $$PWD/src/interface/display/graphic_lcd/font_lucida16pt.h \
   $$PWD/src/interface/display/graphic_lcd/font_lucida7pt.h \
   $$PWD/src/interface/display/graphic_lcd/font_lucida8pt.h \
   $$PWD/src/interface/display/graphic_lcd.h \
   $$PWD/src/debug.h \
   $$PWD/src/event.h \
   $$PWD/src/gpio.h \
   $$PWD/src/interface.h \
   $$PWD/src/scan_tree.h \
   $$PWD/src/scanner.h \
   $$PWD/src/tuner.h

SOURCES = \
   $$PWD/profiles/Test profile 1/Analog system 1/.scn \
   $$PWD/profiles/Test profile 1/Analog system 1.scn \
   $$PWD/profiles/Test profile 1.sp \
   $$PWD/res/sounds/beep.wav \
   $$PWD/res/sounds/critical_error.wav \
   $$PWD/res/sounds/error.wav \
   $$PWD/res/sounds/info_1.wav \
   $$PWD/res/sounds/info_2.wav \
   $$PWD/res/sounds/start.wav \
   $$PWD/res/sounds/stop.wav \
   $$PWD/src/interface/display/driver/drv_MatrixOrbitalGX.c \
   $$PWD/src/interface/display/graphic_lcd/drv_pipe.c \
   $$PWD/src/interface/display/graphic_lcd/font_lucida16pt.c \
   $$PWD/src/interface/display/graphic_lcd/font_lucida7pt.c \
   $$PWD/src/interface/display/graphic_lcd/font_lucida8pt.c \
   $$PWD/src/interface/display/graphic_lcd/graphic_lcd.c \
   $$PWD/src/interface/int_MatrixOrbitalGX_GPIO.c \
   $$PWD/src/debug.c \
   $$PWD/src/event.c \
   $$PWD/src/interface.c \
   $$PWD/src/piscan.c \
   $$PWD/src/rtl_fm.c \
   $$PWD/src/scan_tree.c \
   $$PWD/src/scanner.c \
   $$PWD/src/tuner.c \
   $$PWD/tests/test_event_service.c \
   $$PWD/tests/test_graphic_lcd.c \
   $$PWD/tests/test_scan_tree.c \
   $$PWD/tests/test_tuner_thread.c

INCLUDEPATH = \
    $$PWD/src \
    $$PWD/src/interface/display \
    $$PWD/src/interface/display/graphic_lcd

#DEFINES = 

