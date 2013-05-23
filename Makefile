MYSQL_LIB_DIR :=/usr/lib64/mysql
MYSQL_HEARD_DIR :=/usr/include/mysql

INCLUDES_DIRS := -Iutils -Icommon -IdesignMode -Ilogger -IsongLib -I$(MYSQL_HEARD_DIR) -Isearch -Irefresh
LIBS_DIRS :=  -L$(MYSQL_LIB_DIR)
LIBS :=-lpthread  -lmysqlclient

BIN := webTagEtl
SRCS := Profile.cpp logger.cpp filelock.cpp lib.cpp webLib.cpp MySqlHelper.cpp Utility.cpp localLib.cpp  Converter.cpp searchFileGenerator.cpp  refresher.cpp  PacSocket.cpp tagEtl.cpp
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

vpath %.cpp ./utils ./logger ./songLib ./search ./refresh
vpath %.h  ./common ./utils ./logger ./songLib  ./search ./refresh

all:$(OBJS)
	g++ -g -Wall -o $(BIN) $(OBJS) $(LIBS_DIRS) $(LIBS)
.cpp.o:$(SRCS)
	g++ -g -Wall -o $@ -c $< $(INCLUDES_DIRS)	

run:
	./$(BIN)

clean:
	rm -f *.o
	rm -f *.d
	rm -f $(BIN)
install:
	
.PHONY: all clean run install
