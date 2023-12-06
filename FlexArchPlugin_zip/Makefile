.PHONY: clean all
CFLAGS := -I..
LIBS := -lzip
LDFLAGS := -shared

OBJECTS := FlexArchPlugin_zip.o

FlexArchPlugin_zip.so: $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS) $(LIBS)

$(OBJECTS): %.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $<

clean:
	rm -f *.o *.so
