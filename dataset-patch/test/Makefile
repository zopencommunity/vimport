all: basicwrite basicread

clean:
	rm -f basicwrite basicread *.o *.lst 

INCDIR=../include

basicread: basicread.o ../src/*.o
	$(LD) $(LDFLAGS) -o $@ $< ../src/*.o

basicwrite: basicwrite.o ../src/*.o
	$(LD) $(LDFLAGS) -o $@ $< ../src/*.o

basicread.o: basicread.c $(INCDIR)/*.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -I$(INCDIR) -c $<

basicwrite.o: basicwrite.c $(INCDIR)/*.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -I$(INCDIR) -c $<

