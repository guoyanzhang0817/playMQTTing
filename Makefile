CC = gcc
CFLAGS_FORMAT              = -O2 -Wall -Wcomment -Wformat -Wmissing-braces -Wswitch -Wuninitialized \
                             -Wbad-function-cast -Waggregate-return -Wmultichar -Wsign-compare -Wshadow
CFLAGS_STANDARD            = -lpthread 
CFLAGS_DEBUG               = -gdwarf-2 -g
CFLAGS_OTHERS_LIB          = -lpaho-mqtt3a -ljson-c -lsqlite3 -ldl -lm

SRC_DIR              = ./src
OUTPUT_DIR           = ./output

OBJS_MAIN                  = ./main.o
OBJS_PLAY_MQTT             = ./play_mqtt.o

TARGET   = playMqtt
RM       = rm -rf 
MV       = mv 
CP       = cp -r


OBJS = $(OBJS_MAIN) $(OBJS_PLAY_MQTT)

$(TARGET):$(OBJS)
	$(RM) $(OUTPUT_DIR)/*
	$(MV) $(OBJS) $(OUTPUT_DIR)/
	$(CC) -o $(OUTPUT_DIR)/$(TARGET) $(OUTPUT_DIR)/*  $(CFLAGS_FORMAT) $(CFLAGS_STANDARD) $(CFLAGS_DEBUG) $(CFLAGS_OTHERS_LIB)
$(OBJS_MAIN):
	$(CC) -c $(SRC_DIR)/main.c
$(OBJS_PLAY_MQTT):
	$(CC) -c $(SRC_DIR)/play_mqtt.c

.PHONY:clean
clean:
	-$(RM) $(OUTPUT_DIR)/*
