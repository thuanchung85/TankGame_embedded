CFLAGS		+= -I./sources/app/screens/game_objects
CPPFLAGS	+= -I./sources/app/screens/game_objects

VPATH += sources/app/screens/game_objects

# CPP source files
SOURCES_CPP += sources/app/screens/game_objects/Gun_Bullet.cpp
SOURCES_CPP += sources/app/screens/game_objects/Canon_Bullet.cpp
SOURCES_CPP += sources/app/screens/game_objects/Tank.cpp
SOURCES_CPP += sources/app/screens/game_objects/Ground.cpp
SOURCES_CPP += sources/app/screens/game_objects/Tree.cpp
SOURCES_CPP += sources/app/screens/game_objects/Mountain.cpp
SOURCES_CPP += sources/app/screens/game_objects/House.cpp
SOURCES_CPP += sources/app/screens/game_objects/HP_point.cpp