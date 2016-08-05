RCS = $(wildcard *.cpp)
PROGS = $(patsubst %.cpp,%,$(SRCS))
OBJS = $(SRCS:.cpp=.o)
TEMPS = $(SRCS:.cpp=.txt)
SRC = tmp.cpp
OUT = sift
CFLAGS = `pkg-config --cflags --libs opencv` -lpthread -O3

LDFLAGS = `pkg-config --libs opencv`

$(OUT): $(SRC)
	g++ $(SRC) $(CFLAGS)  -o $@

clean:
	@rm -f $(PROGS) $(OBJS) $(TEMPS) $(OUT)
	@echo "Limpo!"
