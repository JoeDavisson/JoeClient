# JoeClient Makefile

# The official FLTK source tree must be in this directory.
# run "make fltklib", then "make"

# libxft-dev should be installed before compiling FLTK on linux
# (otherwise you'll have ugly, non-resizable fonts)

FLTK_DIR=fltk-1.4.5
PLATFORM=linux
#PLATFORM=mingw64

VERSION=0.1.5
SRC_DIR=src
INCLUDE=-I$(SRC_DIR) -I$(FLTK_DIR)

ifeq ($(PLATFORM),linux)
  LIBS=$(shell ./$(FLTK_DIR)/fltk-config --use-cairo --ldstaticflags)
  HOST=
  CXX=g++
  CXXFLAGS=$(shell pkg-config --cflags cairo)
  CXXFLAGS+=-O3 -Wall -Wunused-parameter -DPACKAGE_STRING=\"$(VERSION)\" $(INCLUDE)
  LIBS+=-lssl -lcrypto
  EXE=joeclient
endif

ifeq ($(PLATFORM),mingw64)
  LIBS=$(shell ./$(FLTK_DIR)/fltk-config --ldstaticflags)
  HOST=x86_64-w64-mingw32
  CXX=$(HOST)-g++
  INCLUDE+=-I/usr/local/mingw64/openssl/include
  LIBS+=-L/usr/local/mingw64/openssl/lib64
  LIBS+=-lssl -lcrypto -lws2_32 -lgdi32 -lcomctl32 -lcrypt32 -static -lpthread
  CXXFLAGS= -O3 -Wall -Wunused-parameter -static-libgcc -static-libstdc++ -DPACKAGE_STRING=\"$(VERSION)\" $(INCLUDE)
  EXE=joeclient.exe
endif

OBJ= \
  $(SRC_DIR)/Chat.o \
  $(SRC_DIR)/CheckBox.o \
  $(SRC_DIR)/Dialog.o \
  $(SRC_DIR)/DialogWindow.o \
  $(SRC_DIR)/Language.o \
  $(SRC_DIR)/Gui.o \
  $(SRC_DIR)/Separator.o \
  $(SRC_DIR)/StyledText.o \
  $(SRC_DIR)/UrlBrowse.o \
  $(SRC_DIR)/UrlSelect.o

# build program
default: $(OBJ)
	$(CXX) -o ./$(EXE) $(SRC_DIR)/Main.cxx $(OBJ) $(CXXFLAGS) $(LIBS)

# build fltk
fltklib:
	cd ./$(FLTK_DIR); \
	make clean; \
	./configure --host=$(HOST) --enable-cairo --enable-cairoext --enable-usecairo --enable-pango --enable-localjpeg --enable-localzlib --enable-localpng --disable-xdbe; \
	make -j20; \
	cd ..; \
	echo "FLTK lib built.";

# remove object files
clean:
	@rm -f $(SRC_DIR)/*.o 
	@rm -f ./joeclient
	@echo "Clean."

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cxx $(SRC_DIR)/%.H
	$(CXX) $(CXXFLAGS) -c $< -o $@

