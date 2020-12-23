CC=cc
CPP=c++
SRCDIR=src/
CFILES=$(wildcard $(SRCDIR)*.c)
CPPFILES=$(wildcard $(SRCDIR)*.cpp)
INC= -Iinc/ -isystem/usr/include/freetype2
LIBDIR= 
COMMONFLAGS:=-Wall -Wfatal-errors $(INC) -MMD -MP
CFLAGS:=-std=gnu18
CPPFLAGS:=-std=gnu++17 -Wshadow=local
RELEASE_FLAGS:=-O2 -march=native
DEBUG_FLAGS:=-DDEBUG -Og -ggdb
LDFLAGS:=-lGL -lGLEW -lglfw -ldl -lm -lfreeimage -lfreetype
RELEASE_LDFLAGS:=
DEBUG_LDFLAGS:=
ODIR=obj/
DEBUGODIR=$(ODIR)debug/
RELEASEODIR=$(ODIR)release/
DEBUG_OFILES = $(patsubst $(SRCDIR)%,$(DEBUGODIR)%,$(patsubst %.c,%.c.o,$(CFILES)))
DEBUG_OFILES += $(patsubst $(SRCDIR)%,$(DEBUGODIR)%,$(patsubst %.cpp,%.cpp.o,$(CPPFILES)))
RELEASE_OFILES = $(patsubst $(SRCDIR)%,$(RELEASEODIR)%,$(patsubst %.c,%.c.o,$(CFILES)))
RELEASE_OFILES += $(patsubst $(SRCDIR)%,$(RELEASEODIR)%,$(patsubst %.cpp,%.cpp.o,$(CPPFILES)))
ALL_OFILES = $(DEBUG_OFILES) $(RELEASE_OFILES)
RELEASE_TARGET := final
DEBUG_TARGET := final_debug

.DEFAULT_GOAL := release

.PHONY: all clean debug release docs

all: release debug docs

release: $(RELEASE_TARGET)

debug: $(DEBUG_TARGET)

-include $(DEBUG_OFILES:%.o=%.d)
-include $(RELEASE_OFILES:%.o=%.d)

$(ALL_OFILES) : Makefile

$(RELEASEODIR) $(DEBUGODIR) :
	mkdir -p $@

$(DEBUGODIR)%.c.o: $(SRCDIR)%.c | $(DEBUGODIR)
	$(CC) $(COMMONFLAGS) $(CFLAGS) $(DEBUG_FLAGS) -c $< -o $@

$(RELEASEODIR)%.c.o: $(SRCDIR)%.c | $(RELEASEODIR)
	$(CC) $(COMMONFLAGS) $(CFLAGS) $(RELEASE_FLAGS) -c $< -o $@

$(DEBUGODIR)%.cpp.o: $(SRCDIR)%.cpp | $(DEBUGODIR)
	$(CPP) $(COMMONFLAGS) $(CPPFLAGS) $(DEBUG_FLAGS) -c $< -o $@

$(RELEASEODIR)%.cpp.o: $(SRCDIR)%.cpp | $(RELEASEODIR)
	$(CPP) $(COMMONFLAGS) $(CPPFLAGS) $(RELEASE_FLAGS) -c $< -o $@

$(DEBUG_TARGET): $(DEBUG_OFILES)
	$(CPP) -o $@ $^ $(LDFLAGS) $(DEBUG_LDFLAGS)

$(RELEASE_TARGET): $(RELEASE_OFILES)
	$(CPP) -o $@ $^ $(LDFLAGS) $(RELEASE_LDFLAGS)

clean:
	rm -rf $(ODIR)
	rm -f $(RELEASE_TARGET)
	rm -f $(DEBUG_TARGET)

docs:
	doxygen Doxyfile
