
DIRECTORY=Generated
BIN=$(DIRECTORY)\bin
IMAGES=$(DIRECTORY)\images

all: serial parallel parallelrace

serial: clean main.cpp
	g++ main.cpp -Wall -Wextra -Wpedantic -o $(BIN)\serial.exe

parallelrace: clean main.cpp
	g++ main.cpp -fopenmp -DRACE_COND -o $(BIN)\race.exe

parallel: clean main.cpp
	g++ main.cpp -fopenmp -o $(BIN)\parallel.exe

clean:
	-if exist $(DIRECTORY) rmdir /S /Q $(DIRECTORY)
	mkdir $(DIRECTORY)
	mkdir $(BIN)
	mkdir $(IMAGES)