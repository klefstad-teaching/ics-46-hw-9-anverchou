CXXFLAGS = -g -ggdb -std=c++20 -Wall -Wextra -Werror -pedantic

dijkstras: src/dijkstras_main.cpp src/dijkstras.cpp src/dijkstras.h
	$(CXX) $(CXXFLAGS) src/dijkstras_main.cpp src/dijkstras.cpp -o dijkstras

ladder: src/ladder_main.cpp src/ladder.cpp src/ladder.h
	$(CXX) $(CXXFLAGS) src/ladder_main.cpp src/ladder.cpp -o ladder

test: gtest/student_gtests.cpp src/dijkstras_main.cpp src/ladder_main.cpp 
	g++ $(CXXFLAGS) gtest/student_gtests.cpp src/dijkstras_main.cpp src/ladder_main.cpp \
	    -o test -lgtest -lgtest_main -lpthread
clean:
	rm -f *.o dijkstras ladder test
