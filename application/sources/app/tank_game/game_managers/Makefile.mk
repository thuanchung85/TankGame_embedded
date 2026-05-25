CFLAGS		+= -I./sources/app/tank_game/game_managers
CPPFLAGS	+= -I./sources/app/tank_game/game_managers

VPATH += sources/app/tank_game/game_managers

# CPP source files
SOURCES_CPP += sources/app/tank_game/game_managers/CollisionManager.cpp
SOURCES_CPP += sources/app/tank_game/game_managers/GameEventsManager.cpp