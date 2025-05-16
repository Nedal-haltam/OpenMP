

all: clean serial parallel parallelrace

serial: main.cpp
	g++ main.cpp -Wall -Wextra -Wpedantic -o serial.exe

parallelrace: main.cpp
	g++ main.cpp -fopenmp -DRACE_COND -o race.exe

parallel: main.cpp
	g++ main.cpp -fopenmp -o parallel.exe

clean:
	del /Q *.exe