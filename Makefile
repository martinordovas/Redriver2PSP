TARGET = redriver2_psp

GAME_OBJS = $(patsubst %.c,%.o,$(wildcard src/Game/*.c src/Game/*/*.c src/Game/*/*/*.c)) \
            $(patsubst %.C,%.o,$(wildcard src/Game/*.C src/Game/*/*.C src/Game/*/*/*.C)) \
            $(patsubst %.cpp,%.o,$(wildcard src/Game/*.cpp src/Game/*/*.cpp src/Game/*/*/*.cpp)) \
            $(patsubst %.CPP,%.o,$(wildcard src/Game/*.CPP src/Game/*/*.CPP src/Game/*/*/*.CPP))

PSYCROSS_OBJS = $(patsubst %.c,%.o,$(wildcard src/PsyCross/*.c src/PsyCross/*/*.c src/PsyCross/*/*/*.c src/PsyCross/*/*/*/*.c)) \
                $(patsubst %.C,%.o,$(wildcard src/PsyCross/*.C src/PsyCross/*/*.C src/PsyCross/*/*/*.C src/PsyCross/*/*/*/*.C)) \
                $(patsubst %.cpp,%.o,$(wildcard src/PsyCross/*.cpp src/PsyCross/*/*.cpp src/PsyCross/*/*/*.cpp src/PsyCross/*/*/*/*.cpp)) \
                $(patsubst %.CPP,%.o,$(wildcard src/PsyCross/*.CPP src/PsyCross/*/*.CPP src/PsyCross/*/*/*.CPP src/PsyCross/*/*/*/*.CPP))

UTILS_OBJS = $(patsubst %.c,%.o,$(wildcard src/utils/*.c src/utils/*/*.c)) \
             $(patsubst %.C,%.o,$(wildcard src/utils/*.C src/utils/*/*.C)) \
             $(patsubst %.cpp,%.o,$(wildcard src/utils/*.cpp src/utils/*/*.cpp)) \
             $(patsubst %.CPP,%.o,$(wildcard src/utils/*.CPP src/utils/*/*.CPP))

PLATFORM_OBJS = src/platform/psp_main.o \
                src/platform/psp_video.o \
                src/platform/psp_input.o \
                src/platform/psp_audio.o

OBJS = $(PLATFORM_OBJS) $(GAME_OBJS) $(PSYCROSS_OBJS) $(UTILS_OBJS)

INCDIR = src src/Game src/PsyCross/include src/PsyCross/include/psx src/PsyCross/src src/utils /opt/pspdev/psp/include/SDL2
CFLAGS = -O3 -G0 -Wall -Wno-narrowing -DPSP -DNG_PSP -DNTSC_VERSION
CXXFLAGS = $(CFLAGS) -std=c++11 -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LIBS = -lGL -lSDL2 -lopenal -ljpeg -lpspvfpu -lpspvram -lpsphprm -lpsprtc -lpsppower -lpspgu -lpspgum -lpspaudiolib -lpspaudio -lm -lc -lstdc++

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Redriver2 PSP

PSPSDK ?= $(shell psp-config --pspsdk-path)

# Compile .c and .C files as C++ (matching REDRIVER2 build requirements)
%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.C
	$(CXX) $(CXXFLAGS) -c $< -o $@

include $(PSPSDK)/lib/build.mak
