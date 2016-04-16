.PHONY : all clean

all: 
	@gcc source/*.c source/polarssl/*.c -o ../pack_tool -I source -I include -std=c99 -Wall -g 

clean:
	@rm -f ../pack_tool