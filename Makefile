
TARGET=exe
OBJS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))
OBJS += $(patsubst %.cpp,%.o,$(wildcard *.cc))

CXX=g++
CXXFLAGS= -g -O0 -std=c++11

all: proto_buf $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o exe -lprotobuf 

.PHONY: proto_buf
proto_buf: def.proto
	protoc $^ --cpp_out=.

clean:
	rm *.o
	rm $(TARGET)
