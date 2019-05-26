CC = arm-linux-gnueabihf-gcc
CXX = arm-linux-gnueabihf-g++

CPPFLAGS = -I .
CFLAGS =-g -std=gnu99 -O1 -Wall
CXXFLAGS = -g -std=gnu++11 -O1 -Wall
LDFLAGS = -lrt -lpthread

SOURCES = main.c mzapo_phys.c mzapo_parlcd.c
#SOURCES += lcdframe.c
SOURCES += lcd_menu_lib.c convert_lib.c write2lcd_lib.c
SOURCES += font_prop14x16.c font_rom8x16.c
TARGET_EXE = main
TARGET_IP ?= 192.168.202.203
ifeq ($(TARGET_IP)$(filter run,$(MAKECMDGOALS)),run)
$(warning The target IP address is not set)
$(warning Run as "TARGET_IP=192.168.202.xxx make run" or modify Makefile)
TARGET_IP ?= 192.168.202.203
endif
TARGET_DIR ?= /tmp/$(shell whoami)
TARGET_USER ?= root

OBJECTS += $(filter %.o,$(SOURCES:%.c=%.o))
OBJECTS += $(filter %.o,$(SOURCES:%.cpp=%.o))

#$(warning OBJECTS=$(OBJECTS))

ifeq ($(filter %.cpp,$(SOURCES)),)
LINKER = $(CC)
LDFLAGS += $(CFLAGS) $(CPPFLAGS)
else
LINKER = $(CXX)
LDFLAGS += $(CXXFLAGS) $(CPPFLAGS)
endif

%.o:%.c
	$(CC) $(CFLAGS) -c $<

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c $<

all: $(TARGET_EXE)

$(TARGET_EXE): $(OBJECTS)
	$(LINKER) $(LDFLAGS) -L. $^ -o $@

.PHONY : dep all run copy-executable debug

dep: depend

depend: $(SOURCES) *.h
	echo '# autogenerated dependencies' > depend
ifneq ($(filter %.c,$(SOURCES)),)
	$(CC) $(CFLAGS) $(CPPFLAGS) -w -E -M $(filter %.c,$(SOURCES)) \
	  >> depend
endif
ifneq ($(filter %.cpp,$(SOURCES)),)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -w -E -M $(filter %.cpp,$(SOURCES)) \
	  >> depend
endif

clean:
	rm -f *.o *.a $(OBJECTS) $(TARGET_EXE) connect.gdb depend

copy-executable: $(TARGET_EXE)
	ssh -t $(TARGET_USER)@$(TARGET_IP) killall gdbserver 1>/dev/null 2>/dev/null || true
	ssh $(TARGET_USER)@$(TARGET_IP) mkdir -p $(TARGET_DIR)
	scp $(TARGET_EXE) $(TARGET_USER)@$(TARGET_IP):$(TARGET_DIR)/$(TARGET_EXE)

run: copy-executable $(TARGET_EXE)
	ssh -t $(TARGET_USER)@$(TARGET_IP) $(TARGET_DIR)/$(TARGET_EXE)

debug: copy-executable $(TARGET_EXE)
	xterm -e ssh -t $(TARGET_USER)@$(TARGET_IP) gdbserver :12345 $(TARGET_DIR)/$(TARGET_EXE) &
	sleep 2
	echo >connect.gdb "target extended-remote $(TARGET_IP):12345"
	echo >>connect.gdb "b main"
	echo >>connect.gdb "c"
	ddd --debugger gdb-multiarch -x connect.gdb $(TARGET_EXE)

-include depend
