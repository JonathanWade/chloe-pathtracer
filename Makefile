#Preprocessor
CPPFLAGS += -I.

#C++ compiler flags
CXXFLAGS += -g -Wall -Wextra -std=c++11

#All the things our executable depends on
OBJS = main.o

pathtracer: $(OBJS) 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ 

clean:
	rm -f $(OBJS) pathtracer

main.o: main.cc
