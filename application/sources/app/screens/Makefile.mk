include sources/app/screens/bitmap_data/Makefile.mk
include sources/app/screens/game_objects/Makefile.mk
include sources/app/screens/game_screens/Makefile.mk
include sources/app/screens/game_managers/Makefile.mk

CFLAGS		+= -I./sources/app/screens
CPPFLAGS	+= -I./sources/app/screens

VPATH += sources/app/screens

# CPP source files
SOURCES_CPP += sources/app/screens/scr_info.cpp
SOURCES_CPP += sources/app/screens/scr_startup.cpp
