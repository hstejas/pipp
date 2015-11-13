IDIR =../
CC=g++
CPPFLAGS += -I$(IDIR) --std=c++11



DEPS=uart.hpp utils.hpp

uart: uart.cpp $(DEPS)
	$(CC) -c -o uart.o $< $(CPPFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
