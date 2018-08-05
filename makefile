CXX=clang++
LIBS=-lglfw -lGL -lGLU -lGLEW -lpng
# FLAGS=-DGLEW_STATIC
CXXFLAGS= -std=c++14 -Wall -g

OFILES=window.o shadersproc.o color.o glpng.o

default: testopengl

testopengl: main.cc $(OFILES)
	$(CXX) $(CXXFLAGS) $(FLAGS) $(LIBS) -o $@ $^

%.o: %.cc %.h
	$(CXX) $(CXXFLAGS) $(FLAGS) -c $<

.PHONY: clean

clean:
	rm -rf testopengl *.o *.a *.exe