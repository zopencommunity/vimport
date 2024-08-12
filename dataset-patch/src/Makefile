IINCDIR=../intinclude
INCDIR=../include
MACDIR=../maclib

AR=ar
ARFLAGS=rcs
ARCHIVE=libdio.a

OBJ_FILES=dio.o call31.o call31a.o s99.o s99a.o

all: $(ARCHIVE)

$(ARCHIVE): $(OBJ_FILES)
	$(AR) $(ARFLAGS) $@ $(OBJ_FILES)

clean:
	rm -f *.o *.lst $(ARCHIVE)

call31.o: call31.c $(IINCDIR)/wrappers.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -I$(IINCDIR) -c $<

call31a.o: call31a.s
	$(AS) $(ASFLAGS) -I$(MACDIR) $< >$*.lst

s99a.o: s99a.s
	$(AS) $(ASFLAGS) -I$(MACDIR) $< >$*.lst

dio.o: dio.c $(IINCDIR)/wrappers.h $(INCDIR)/*.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -I$(INCDIR) -I$(IINCDIR) -c $<

s99.o: s99.c $(IINCDIR)/wrappers.h $(IINCDIR)/s99.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -I$(INCDIR) -I$(IINCDIR) -c $<

