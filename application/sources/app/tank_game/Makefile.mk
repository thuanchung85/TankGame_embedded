include sources/app/tank_game/bitmap_data/Makefile.mk
include sources/app/tank_game/game_objects/Makefile.mk
include sources/app/tank_game/game_screens/Makefile.mk
include sources/app/tank_game/other_screens/Makefile.mk

CFLAGS		+= -I./sources/app/tank_game
CPPFLAGS	+= -I./sources/app/tank_game

VPATH += sources/app/tank_game

# CPP source files
