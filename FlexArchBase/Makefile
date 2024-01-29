.PHONY: clean all

CFLAGS := -I.. -DFLEXARCH_INTERNAL -g

all: FlexArchBase.a

FlexArchBase.a: FlexArchInternals.o
	ar rcs libFlexArchBase.a FlexArchInternals.o

FlexArchInternals.o: FlexArchInternals.c FlexArchInternals.h
	cc -o FlexArchInternals.o -c FlexArchInternals.c $(CFLAGS)

clean: $(clean_plugins)
	rm -f libFlexArchBase.a FlexArchInternals.o
