include sources/app/screens/bitmap_data/Makefile.mk
include sources/app/screens/game_objects/Makefile.mk

CFLAGS		+= -I./sources/app/screens
CPPFLAGS	+= -I./sources/app/screens

VPATH += sources/app/screens

# CPP source files
SOURCES_CPP += sources/app/screens/scr_es35sw_th_sensor.cpp
SOURCES_CPP += sources/app/screens/scr_idle.cpp
SOURCES_CPP += sources/app/screens/scr_info.cpp
SOURCES_CPP += sources/app/screens/scr_lhio404_io_device.cpp
SOURCES_CPP += sources/app/screens/scr_noen.cpp
SOURCES_CPP += sources/app/screens/scr_startup.cpp
SOURCES_CPP += sources/app/screens/scr_game.cpp
SOURCES_CPP += sources/app/screens/scr_banner_game.cpp