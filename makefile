CXX=clang++
LIBS=-lglfw -lGL -lGLU -lGLEW
# FLAGS=-DGLEW_STATIC
CXXFLAGS= -std=c++14 -Wall -g

default: testopengl

testopengl: main.cc window.o
	$(CXX) $(CXXFLAGS) $(FLAGS) $(LIBS) -o $@ $^

%.o: %.cc %.h
	$(CXX) $(CXXFLAGS) $(FLAGS) -c $<

.PHONY: clean

clean:
	rm -rf testopengl *.o *.a *.exe