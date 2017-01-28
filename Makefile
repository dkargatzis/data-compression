DEBUG=1
#PROFILE=1

DEBUGFLAGS=  -g 
OPTFLAGS= -finline -march=native -O3 -DNDEBUG 

ifeq ($(PROFILE),1)
PROFFLAGS= -g -pg 
PLFLAGS= -g -pg
else
PROFFLAGS= 
PLFLAGS=
endif

INCLUDE_PATH=-I. 

CFLAGS= -Wall -D_GNU_SOURCE 
ifeq ($(DEBUG),1)
CFLAGS+=  $(DEBUGFLAGS) $(PROFFLAGS) $(INCLUDE_PATH)
else
CFLAGS+=  $(OPTFLAGS) $(PROFFLAGS) $(INCLUDE_PATH)
endif

LDFLAGS= $(PLFLAGS)  
LIBS= -lm


C_PROG= huffman_encoding.c
PROG=$(C_PROC:.c=)

C_SRC= encoding.c residual_code.c dec_to_bin.c golomb_rice.c
C_OBJ=$(C_SRC:.c=.o)  

C_SRC_1= laplace.c
C_OBJ_1=$(C_SRC_1:.c=.o)

C_SOURCES= $(C_PROG) $(C_SRC) $(C_SRC_1)
C_OBJECTS=$(C_SOURCES:.c=.o)


.PHONY: all

all: huffman_encoding 

huffman_encoding: huffman_encoding.o $(C_OBJ) $(C_OBJ_1)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)


clean:
	-rm huffman_encoding $(C_OBJECTS)

valid_clean: 
	-rm valid.txt
