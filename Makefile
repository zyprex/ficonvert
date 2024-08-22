CC = gcc 

CFLAGS = -Wall -Wextra -std=c99 -c 
INC_PATH = -I.\\
LD_PATH = -L.\\ -lfreeimage

RM = del

OBJS = ficonvert.o ficonvert_excmd.o
TARGET = ficonvert.exe

OBJS2 = fijpeg.o
TARGET2 = fijpeg.exe

all: $(TARGET2) $(TARGET)

$(TARGET2):  $(OBJS2)
	$(CC) $(LD_PATH) $^ -o $(TARGET2)

$(TARGET):  $(OBJS)
	$(CC) $(LD_PATH) $^ -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $^

clean:
	$(RM) $(OBJS) $(OBJS2)

