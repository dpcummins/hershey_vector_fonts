IDIR =../include
ODIR=obj

CC=gcc
CFLAGS=-I$(IDIR)

_INCS = hershey_font.h
INCS = $(patsubst %,$(IDIR)/%,$(_INCS))

LIBS=-lX11

_OBJS = showhfont.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))


$(ODIR)/%.o: %.c $(INCS)
	$(CC) -c -o $@ $< $(CFLAGS)

showhfont: $(ODIR)/showhfont.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

showhtext: $(ODIR)/showhtext.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ $(IDIR)/*~ showhfont showhtext
