all: compile


prepare_build:
	echo "PREPARING BUILD DIRECTORY..."
	if [ ! -d "./build" ]; then mkdir build; fi

compile: prepare_build
	echo "Compiling the TU's and linking with raylib..."
	 g++ -c ./src/ui.cpp 
	 g++ -c ./src/m3.cpp
	 g++ -c ./src/file_dialog.cpp -mwindows
	 g++ -c ./src/main.cpp

	 g++ -o ./build/m3 ./main.o ./m3.o ./ui.o ./file_dialog.o -g -I"C:\\raylib\raylib\src" -lraylib -lgdi32 -lwinmm -mwindows

	 #g++ -o ./build/m3 ./main.o ./m3.o ./ui.o -I"C:\\raylib\raylib\src" -Wl,--subsystem,windows -lraylib -lgdi32 -lwinmm

clean:
	rm main.o m3.o ui.o file_dialog.o

rmbuild:
	rm -rf ./build
