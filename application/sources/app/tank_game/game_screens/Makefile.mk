CFLAGS		+= -I./sources/app/tank_game/game_screens
CPPFLAGS	+= -I./sources/app/tank_game/game_screens
VPATH += sources/app/tank_game/game_screens

# CPP source files
SOURCES_CPP += sources/app/tank_game/game_screens/scr_game.cpp
SOURCES_CPP += sources/app/tank_game/game_screens/scr_topscore_game.cpp
SOURCES_CPP += sources/app/tank_game/game_screens/scr_banner_game.cpp
SOURCES_CPP += sources/app/tank_game/game_screens/scr_game_over.cpp
SOURCES_CPP += sources/app/tank_game/game_screens/scr_victory.cpp
