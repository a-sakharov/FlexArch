.PHONY: clean

CFLAGS := -I.. -DFLEXARCH_INTERNAL

OBJECTS := FlexArchCl.o

FlexArchCl: $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS) $(LIBS)

$(OBJECTS): %.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $<

clean:
	rm -f *.o FlexArchCl $(OBJECTS)
