RCS = $(wildcard *.cpp)
PROGS = $(patsubst %.cpp,%,$(SRCS))
OBJS = $(SRCS:.cpp=.o)
TEMPS = $(SRCS:.cpp=.txt)
SRC = src/main.cpp src/computeDescriptor.cpp src/createDoG.cpp src/detectKeypoints.cpp src/drawKeyPoints.cpp src/filterKeyPoints.cpp \
		src/match.cpp src/debug.cpp src/computeSift.cpp \
		src/dumpKeyPoints.cpp src/dumpImage.cpp
OUT = sift
CFLAGS = `pkg-config --cflags --libs opencv` -lpthread -O3

:q
LDFLAGS = `pkg-config --libs opencv`

$(OUT): $(SRC)
	g++ $(SRC) $(CFLAGS)  -o $@

clean:
	@rm -f $(PROGS) $(OBJS) $(TEMPS) $(OUT)
	@echo "Limpo!"
