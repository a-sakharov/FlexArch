.PHONY: clean all install

PLUGINS := $(wildcard FlexArchPlugin_*)
PLUGIN_OUT := $(join $(addsuffix /,$(PLUGINS)),$(PLUGINS).so)

INSTALL_PATH := "install"

clean_plugins=$(addprefix clean_,$(PLUGINS))
make_plugins=$(addprefix make_,$(PLUGINS))
install_plugins=$(addprefix install_,$(PLUGINS))


all: FlexArchCl/FlexArchCl FlexArchGui/FlexArchGui plugins

plugins: $(make_plugins)

FlexArchBase/libFlexArchBase.a:
	make -C FlexArchBase

FlexArchCl/FlexArchCl: FlexArchBase/libFlexArchBase.a
	make -C FlexArchCl

FlexArchGui/FlexArchGui: FlexArchBase/libFlexArchBase.a
	make -C FlexArchGui

#FlexArchPlugin_%:
#	make -C $@

install: all $(install_plugins)
	-mkdir $(INSTALL_PATH) 2> /dev/null
	cp FlexArchCl/FlexArchCl $(INSTALL_PATH)
	cp FlexArchGui/FlexArchGui $(INSTALL_PATH)

$(PLUGIN_OUT): $(make_plugins)

$(install_plugins): $(PLUGIN_OUT)
	-mkdir $(INSTALL_PATH) 2> /dev/null
	cp $(patsubst install_%,%,$@)/$(patsubst install_%,%,$@).so $(INSTALL_PATH)
    
$(make_plugins):
	make -C $(patsubst make_%,%,$@)
    
$(clean_plugins):
	make -C $(patsubst clean_%,%,$@) clean
  
clean: $(clean_plugins)
	make -C FlexArchGui clean
	make -C FlexArchCl clean
	make -C FlexArchBase clean

dbg:
	echo $(PLUGIN_OUT)
	echo $(PLUGINS)
	echo $(clean_plugins)
	echo $(make_plugins)
	echo $(install_plugins)