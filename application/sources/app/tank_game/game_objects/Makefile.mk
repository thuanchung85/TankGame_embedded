CFLAGS		+= -I./sources/app/tank_game/game_objects
CPPFLAGS	+= -I./sources/app/tank_game/game_objects

VPATH += sources/app/tank_game/game_objects

# CPP source files
SOURCES_CPP += sources/app/tank_game/game_objects/ground_object.cpp
SOURCES_CPP += sources/app/tank_game/game_objects/tree_object.cpp
SOURCES_CPP += sources/app/tank_game/game_objects/building_object.cpp
