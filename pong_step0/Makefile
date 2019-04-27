TARGET			=	pong_step0
CXXFLAGS		=	`pkg-config --cflags glfw3` -std=c++17 -Wall -pedantic -Werror -O3
INCLUDE_PATH	=	-I../sgfx/include
LIB_PATH		=	-L../sgfx/lib
LIBS			=	-lsgfx `pkg-config --static --libs glfw3` -lGL -lGLEW

SRCS			=	*.cpp
OBJS			=	$(patsubst %.cpp,%.o,$(wildcard $(SRCS)))


all: $(TARGET)

$(TARGET): $(OBJS) 
	$(CXX) $(OBJS) -o $(TARGET) $(LIB_PATH) $(LIBS)
	
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_PATH) -c $< -o $@

clean:
	rm $(TARGET) $(OBJS)

.PHONY: shared clean
