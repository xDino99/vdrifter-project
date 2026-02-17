CXX = g++
CXXFLAGS = -O3 -std=c++17 -s -Isrc/header
TARGET = vdrifter

SRC_MAIN = src/main.cpp
SRC_CPP = src/cpp/Game.cpp src/cpp/Terminal.cpp
OBJ = main.o Game.o Terminal.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET) -lm

main.o: $(SRC_MAIN)
	$(CXX) $(CXXFLAGS) -c $(SRC_MAIN) -o main.o

Game.o: src/cpp/Game.cpp
	$(CXX) $(CXXFLAGS) -c src/cpp/Game.cpp -o Game.o

Terminal.o: src/cpp/Terminal.cpp
	$(CXX) $(CXXFLAGS) -c src/cpp/Terminal.cpp -o Terminal.o

clean:
	rm -f $(OBJ) $(TARGET)

install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/

.PHONY: all clean install