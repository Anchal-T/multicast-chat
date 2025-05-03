# Makefile for multicast_chat client

CC = g++
CFLAGS = -std=c++14 -Iinclude -pthread -Wall -O2

SRCS = client.cpp network.cpp message.cpp 
OBJS = $(SRCS:.cpp=.o)
TARGET = client

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.cpps
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
