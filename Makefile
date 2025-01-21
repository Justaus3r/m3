all: compile


UNAME_S := $(shell uname -s)

prepare_build:
	echo "PREPARING BUILD DIRECTORY..."
	if [ ! -d "./build" ]; then mkdir build; fi

compile: prepare_build
	echo "Compiling the TU's and linking with raylib..."
	 g++ -c ./src/ui.cpp 
	 g++ -c ./src/m3.cpp
ifeq ($(UNAME_S), Linux)
	g++ -c ./src/file_dialog.cpp
else 
	g++ -c ./src/file_dialog.cpp -mwindows -DPLATFORM_WINDOWS
endif
	 g++ -c ./src/main.cpp



ifeq ($(UNAME_S),Linux)
	g++ -o ./build/m3 ./main.o ./m3.o ./ui.o ./file_dialog.o -g -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
else
	g++ -o ./build/m3 ./main.o ./m3.o ./ui.o ./file_dialog.o -g -I"C:\\raylib\raylib\src" -lraylib -lgdi32 -lwinmm -mwindows
endif
	 #g++ -o ./build/m3 ./main.o ./m3.o ./ui.o -I"C:\\raylib\raylib\src" -Wl,--subsystem,windows -lraylib -lgdi32 -lwinmm

clean:
	rm main.o m3.o ui.o file_dialog.o

rmbuild:
	rm -rf ./build
