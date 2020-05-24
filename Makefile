CXX?=g++
SDL2FLAGS= $(shell pkg-config sdl2 --cflags --libs)
CXXFLAGS?=-std=c++11 -Wall -pedantic -Werror -Wshadow -Wstrict-aliasing -Wstrict-overflow
FILENAME=*.cpp
OUTNAME=outfile

SDLWIN=$(shell ../SDL2-2.0.12/i686-w64-mingw32/bin/sdl2-config  --prefix=../SDL2-2.0.12/i686-w64-mingw32 --cflags --libs --static-libs)

all:
	${CXX} ${FILENAME} ${CXXFLAGS} ${SDL2FLAGS} -o ${OUTNAME}
debug:
	${CXX} ${FILENAME} ${CXXFLAGS} ${SDL2FLAGS} -D DEBUG -g -o ${OUTNAME}_debug
slow_debug:
	${CXX} ${FILENAME} ${CXXFLAGS} ${SDL2FLAGS} -D DEBUG -D SLOW -g -o ${OUTNAME}_debug
win:
	i686-w64-mingw32-gcc ${FILENAME} ${CXXFLAGS} -lmingw32 ${SDLWIN} -lstdc++ -static -o ${OUTNAME}.exe
