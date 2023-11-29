# wx-config to use, may be overridden on make command line.
.PHONY: clean

WX_CONFIG := wx-config
 
WX_CXXFLAGS := $(shell $(WX_CONFIG) --cxxflags)
WX_LIBS := $(shell $(WX_CONFIG) --libs)
 
OBJECTS := FlexArchGui.o
 
FlexArchGui: $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(LDFLAGS) $(WX_LIBS) $(LIBS)
 
$(OBJECTS): %.o: %.cpp
	$(CXX) -c -o $@ $(WX_CXXFLAGS) $(CXXFLAGS) $<

clean:
	rm -f *.o FlexArchGui