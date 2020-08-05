OBJS	= main.o Chirper.o
SOURCE	= main.cpp Chirper.cpp
HEADER	= Chirper.h
OUT	= chirper
CC	= g++ -std=c++11
FLAGS	= -g -c 

all: $(OBJS)
		$(CC) -o $(OUT) $(OBJS)

main.o: main.cpp
		$(CC) $(FLAGS) main.cpp
Chirper.o: Chirper.cpp
		$(CC) $(FLAGS) Chirper.cpp

clean:
		rm -f $(OBJS) $(OUT)
