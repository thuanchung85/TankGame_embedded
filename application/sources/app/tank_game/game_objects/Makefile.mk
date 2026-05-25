CFLAGS		+= -I./sources/app/tank_game/game_objects
CPPFLAGS	+= -I./sources/app/tank_game/game_objects

VPATH += sources/app/tank_game/game_objects

# CPP source files
SOURCES_CPP += sources/app/tank_game/game_objects/Gun_Bullet.cpp
SOURCES_CPP += sources/app/tank_game/game_objects/Cannon_Bullet.cpp
SOURCES_CPP += sources/app/tank_game/game_objects/Tank.cpp
SOURCES_CPP += sources/app/tank_game/game_objects/Ground.cpp
SOURCES_CPP += sources/app/tank_game/game_objects/Tree.cpp
SOURCES_CPP += sources/app/tank_game/game_objects/Mountain.cpp
SOURCES_CPP += sources/app/tank_game/game_objects/House.cpp
SOURCES_CPP += sources/app/tank_game/game_objects/HP_point.cpp
SOURCES_CPP += sources/app/tank_game/game_objects/Score.cpp
SOURCES_CPP += sources/app/tank_game/game_objects/Enemy.cpp
SOURCES_CPP += sources/app/tank_game/game_objects/Trap.cpp
SOURCES_CPP += sources/app/tank_game/game_objects/Boss.cpp