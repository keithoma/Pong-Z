TARGET			=	lib/libsgfx.a
CXXFLAGS		=	`pkg-config --cflags glfw3` -std=c++17 -Wall -pedantic -Werror -O3
INCLUDE_PATH	=	-Iinclude

SRCS			=	src/*.cpp
OBJS			=	$(patsubst src/%.cpp,src/%.o,$(wildcard $(SRCS)))
	
all: $(TARGET)

$(TARGET):$(OBJS) 
	ar rcs $(TARGET) $(OBJS)
	ranlib $(TARGET)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_PATH) -c $< -o $@

clean:
	rm $(TARGET) $(OBJS)

.PHONY: clean
