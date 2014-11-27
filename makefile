IDIR =../
CC=arm-unknown-linux-gnueabi-g++
CFLAGS=-I$(IDIR) -g --std=c++11



DEPS=uart.hpp utils.hpp

uart: uart.cpp $(DEPS)
	$(CC) -c -o uart.o $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 