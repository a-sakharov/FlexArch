.PHONY: clean all

PLUGINS := $(wildcard FlexArchPlugin_*)
PLUGIN_OUT := $(join $(addsuffix /,$(PLUGINS)),$(PLUGINS))

all: FlexArchGui/FlexArchGui FlexArchCl/FlexArchCl

FlexArchCl/FlexArchCl:
	make -C FlexArchCl

FlexArchGui/FlexArchGui:
	make -C FlexArchGui

FlexArchPlugin_%:
	make -C $@

clean_plugins=$(addprefix clean_,$(PLUGINS))

$(clean_plugins):
	make -C $(patsubst clean_%,%,$@) clean
  
clean: $(clean_plugins)
	make -C FlexArchGui clean
	make -C FlexArchCl clean
