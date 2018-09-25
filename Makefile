# JoeClient Makefile
#
# you MUST have libxft-dev installed before compiling FLTK on linux
# (otherwise you'll have ugly, non-resizable fonts)
#PLATFORM=linux
#PLATFORM=mingw32
PLATFORM=mingw64

NAME="JoeClient "
#VERSION=$(shell git describe --always --dirty)
VERSION=$(shell git describe --always)

SRC_DIR=src
INCLUDE=-I$(SRC_DIR) $(shell fltk-config --cxxflags)
LIBS=$(shell fltk-config --use-images --ldstaticflags)

ifeq ($(PLATFORM),linux)
  HOST=
  CXX=g++
  CXXFLAGS=-O3 -DPACKAGE_STRING=\"$(NAME)$(VERSION)\" $(INCLUDE)
  EXE=joeclient
endif

ifeq ($(PLATFORM),mingw32)
  HOST=i686-w64-mingw32
  CXX=$(HOST)-g++
  CXXFLAGS=-O3 -static-libgcc -static-libstdc++ -DPACKAGE_STRING=\"$(NAME)$(VERSION)\" $(INCLUDE)
  LIBS+=-lgdi32 -lcomctl32 -lws2_32 -static -lpthread
  EXE=joeclient.exe
endif

ifeq ($(PLATFORM),mingw64)
  HOST=x86_64-w64-mingw32
  CXX=$(HOST)-g++
  CXXFLAGS=-O3 -static-libgcc -static-libstdc++ -DPACKAGE_STRING=\"$(NAME)$(VERSION)\" $(INCLUDE)
  LIBS+=-lgdi32 -lcomctl32 -lws2_32 -static -lpthread
  EXE=joeclient.exe
endif

OBJ= \
  $(SRC_DIR)/Chat.o \
  $(SRC_DIR)/Dialog.o \
  $(SRC_DIR)/DialogWindow.o \
  $(SRC_DIR)/Gui.o \
  $(SRC_DIR)/Separator.o

default: $(OBJ)
	$(CXX) -o ./$(EXE) $(SRC_DIR)/Main.cxx $(OBJ) $(CXXFLAGS) $(LIBS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cxx $(SRC_DIR)/%.H
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -f $(SRC_DIR)/*.o 
	@rm -f ./$(EXE)
	@echo "Clean!"

