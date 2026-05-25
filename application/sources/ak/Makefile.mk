include sources/ak/ak.cfg.mk

CFLAGS += -I./sources/ak/inc
CPPFLAGS += -I./sources/ak/inc

VPATH += sources/ak/src

SOURCES_CPP += sources/ak/src/fsm.cpp
SOURCES_CPP += sources/ak/src/tsm.cpp
SOURCES_CPP += sources/ak/src/task.cpp
SOURCES_CPP += sources/ak/src/timer.cpp
SOURCES_CPP += sources/ak/src/message.cpp
