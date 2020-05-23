CXX?=g++
SDL2FLAGS=$(shell pkg-config sdl2 --cflags --libs)
CXXFLAGS?=-std=c++11 -Wall -pedantic -Werror -Wshadow -Wstrict-aliasing -Wstrict-overflow -lSDL2_image
FILENAME=*.cpp
OUTNAME=outfile

all:
	${CXX} ${FILENAME} ${CXXFLAGS} ${SDL2FLAGS} -o ${OUTNAME}

debug:
	${CXX} ${FILENAME} ${CXXFLAGS} ${SDL2FLAGS} -g -o ${OUTNAME}_debug
