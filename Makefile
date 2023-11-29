.PHONY: clean all

all: FlexArchGui/FlexArchGui

FlexArchGui/FlexArchGui:
	make -C FlexArchGui

clean:
	make -C FlexArchGui clean