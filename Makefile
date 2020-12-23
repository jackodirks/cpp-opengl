SRCDIR=src/
CFILES=$(wildcard $(SRCDIR)*.c)
CXXFILES=$(wildcard $(SRCDIR)*.cpp)
INC= -Iinc/ -I/usr/include/freetype2
LIBDIR=
CFLAGS:=-std=gnu18 -Wall -Wfatal-errors
CXXFLAGS:=-std=gnu++17 -Wshadow=local -Wall -Wfatal-errors
CPPFLAGS:=$(INC) -MMD -MP
LDFLAGS:=-lGL -lGLEW -lglfw -ldl -lm -lfreeimage -lfreetype
ODIR=obj/
DEBUGODIR=$(ODIR)debug/
RELEASEODIR=$(ODIR)release/
DEBUG_OFILES = $(patsubst $(SRCDIR)%,$(DEBUGODIR)%,$(patsubst %.c,%.c.o,$(CFILES)))
DEBUG_OFILES += $(patsubst $(SRCDIR)%,$(DEBUGODIR)%,$(patsubst %.cpp,%.cpp.o,$(CXXFILES)))
RELEASE_OFILES = $(patsubst $(SRCDIR)%,$(RELEASEODIR)%,$(patsubst %.c,%.c.o,$(CFILES)))
RELEASE_OFILES += $(patsubst $(SRCDIR)%,$(RELEASEODIR)%,$(patsubst %.cpp,%.cpp.o,$(CXXFILES)))
ALL_OFILES = $(DEBUG_OFILES) $(RELEASE_OFILES)
RELEASE_TARGET := final
DEBUG_TARGET := final_debug

.DEFAULT_GOAL := release

.PHONY: all clean debug release docs

all: release debug docs

release: CFLAGS += -O2 -march=native
release: CXXFLAGS += -O2 -march=native
release: $(RELEASE_TARGET)

debug: CFLAGS += -Og -ggdb
debug: CXXFLAGS += -Og -ggdb
debug: CPPFLAGS += -DDEBUG
debug: $(DEBUG_TARGET)

-include $(DEBUG_OFILES:%.o=%.d)
-include $(RELEASE_OFILES:%.o=%.d)

$(ALL_OFILES) : Makefile

$(RELEASEODIR) $(DEBUGODIR) :
	mkdir -p $@

$(DEBUGODIR)%.c.o: $(SRCDIR)%.c | $(DEBUGODIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(RELEASEODIR)%.c.o: $(SRCDIR)%.c | $(RELEASEODIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(DEBUGODIR)%.cpp.o: $(SRCDIR)%.cpp | $(DEBUGODIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(RELEASEODIR)%.cpp.o: $(SRCDIR)%.cpp | $(RELEASEODIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(DEBUG_TARGET): $(DEBUG_OFILES)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(RELEASE_TARGET): $(RELEASE_OFILES)
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf $(ODIR)
	rm -f $(RELEASE_TARGET)
	rm -f $(DEBUG_TARGET)

docs:
	doxygen Doxyfile
