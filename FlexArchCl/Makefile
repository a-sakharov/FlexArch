.PHONY: clean

CFLAGS := -I.. -DFLEXARCH_INTERNAL -g -O0
LIBS := -ldl
OBJECTS := FlexArchCl.o
LDFLAGS := -L../FlexArchBase -lFlexArchBase

FlexArchCl: $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS) $(LIBS)

$(OBJECTS): %.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $<

clean:
	rm -f *.o FlexArchCl $(OBJECTS)
