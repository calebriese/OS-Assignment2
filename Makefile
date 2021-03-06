CC 	 = gcc
CFLAGS   = -std=c11 -w -lm
TARGET   = master
TARGETb = bin_adder
OBJFILES =
SOURCE = master.c
SOURCEb = bin_adder.c
OUTPUT = $(TARGET) $(TARGETb)

all: $(OUTPUT)
 
$(TARGET): $(OBJFILES)  
	$(CC) -o $(TARGET) $(SOURCE) $(CFLAGS) 

$(TARGETb): $(OBJFILES)
	$(CC) -o $(TARGETb) $(SOURCEb) $(CFLAGS) 

clean: 
	rm -f $(TARGET) $(TARGETb) *.o adder_log output.log
