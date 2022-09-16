EXEFILE = ray
CXXFLAGS = -c -Wall 
LINKER_FLAGS = -std=c++14 -Ofast -pthread
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
INCLUDES = -I headers

$(EXEFILE): $(OBJECTS)
	g++ $(LINKER_FLAGS) $^ -o $@ $(INCLUDES)

%.o: %.cpp
	g++ $(CXXFLAGS) $(LINKER_FLAGS) $^ -o $@ $(INCLUDES)

clean:
	rm *.o $(EXEFILE)

.PHONY: clean
