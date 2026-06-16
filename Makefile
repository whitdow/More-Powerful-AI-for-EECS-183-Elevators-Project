CXX ?= g++
CXXFLAGS ?= -Wall -Werror -pedantic -g --std=c++17 -Wno-sign-compare -Wno-comment

# Compile the game exe
elevators.exe: AI.cpp Building.cpp Elevator.cpp Floor.cpp Game.cpp main.cpp Move.cpp Person.cpp SatisfactionIndex.cpp test.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@


 # Remove automatically generated files
clean :
	rm -rvf *.exe *~ *.out *.dSYM *.stackdump