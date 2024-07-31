CC = gcc 

CFLAGS = -Wall -Wextra -std=c99 -c 
INC_PATH = -I.\\
LD_PATH = -L.\\ -lfreeimage

RM = del


OBJS = ficonvert.o ficonvert_excmd.o
TARGET = ficonvert.exe

$(TARGET):  $(OBJS)
	$(CC) $(LD_PATH) $^ -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $^

clean:
	$(RM) $(OBJS)

