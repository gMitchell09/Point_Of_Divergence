# targets are
#   box2d      - just create the box2d library
#   examples   - just create the examples
#   all      - create everything. or simply call make/mingw32-make by itself
#   clean      - remove all .o files
#   veryclean   - remove all .o and .a files

LIBDIR = ./Build
LIBDIRMINGW = .\Build
INCLUDEDIR = ../

CC = gcc
CXX = g++
LD = g++
CFLAGS = -O2 -s -fexceptions -I$(INCLUDEDIR)

SOURCE = ./

TESTLIBS = -lBox2d -lfreeglut -lglui -lGLU32 -lOpenGL32 -lWinMM -lGDI32
OBJDIR = ./obj

OBJ_CPP_CORE += $(addprefix $(OBJDIR)/core/,     $(subst $(SOURCE),,$(patsubst %.cpp,%.o,$(wildcard $(SOURCE)*/*.cpp))))
OBJ_CPP_CORE += $(addprefix $(OBJDIR)/core/,     $(subst $(SOURCE),,$(patsubst %.cpp,%.o,$(wildcard $(SOURCE)*/*/*.cpp))))
OBJ_CPP_GLUI += $(addprefix $(OBJDIR)/glui/,     $(subst $(CONTRIB)glui/,,$(patsubst %.cpp,%.o,$(wildcard $(CONTRIB)glui/*.cpp))))
OBJ_C_FREE   += $(addprefix $(OBJDIR)/freeglut/, $(subst $(CONTRIB)freeglut/,,$(patsubst %.c,%.o,$(wildcard $(CONTRIB)freeglut/*.c))))

ifeq ($(findstring /sh.exe, $(SHELL)), /sh.exe)
	UNIX_TOOLS=1
endif

UNIX_TOOLS=1

all: createdir box2d

box2d:  createdir $(OBJ_CPP_CORE)
	ar rc $(LIBDIR)/libbox2d.a $(OBJDIR)/core/*.o
	@echo   BOX2D LIBRARY MADE 

$(OBJDIR)/core/%.o: $(SOURCE)%.cpp
	$(CXX) $(CFLAGS) -o $(OBJDIR)/core/$(notdir $@) -c $<

createdir:
ifdef UNIX_TOOLS
	-mkdir -p $(OBJDIR)
	-mkdir -p $(OBJDIR)/core
	-mkdir -p $(OBJDIR)/glui
	-mkdir -p $(OBJDIR)/freeglut
	-mkdir -p $(LIBDIR)
else
	-mkdir $(OBJDIR)
	-mkdir $(OBJDIR)\core
	-mkdir $(OBJDIR)\glui
	-mkdir $(OBJDIR)\freeglut
endif
	@echo   TEMP AREA CREATED 

clean:
ifdef UNIX_TOOLS
	-rm $(OBJDIR)/core/*.o
	-rm $(OBJDIR)/glui/*.o
	-rm $(OBJDIR)/freeglut/*.o
else
	-del $(OBJDIR)\core\*.o
	-del $(OBJDIR)\glui\*.o
	-del $(OBJDIR)\freeglut\*.o
endif
	@echo  DONE CLEAN 

veryclean: clean
ifdef UNIX_TOOLS
	-rm $(LIBDIR)/*.a
else
	-del $(LIBDIRMINGW)\*.a
endif
	@echo  DONE VERYCLEAN 

