all: dio.o call31.o call31a.o s99.o s99a.o

clean:
	rm -f *.o *.lst

IINCDIR=../intinclude
INCDIR=../include
MACDIR=../maclib

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

