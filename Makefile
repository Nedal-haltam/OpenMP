
DIRECTORY=Generated


all: serial parallel parallelrace

serial: clean main.cpp
	g++ main.cpp -Wall -Wextra -Wpedantic -o $(DIRECTORY)\serial.exe

parallelrace: clean main.cpp
	g++ main.cpp -fopenmp -DRACE_COND -o $(DIRECTORY)\race.exe

parallel: clean main.cpp
	g++ main.cpp -fopenmp -o $(DIRECTORY)\parallel.exe

clean:
	-if exist $(DIRECTORY) rmdir /S /Q $(DIRECTORY)
	mkdir $(DIRECTORY)