GTEST_DIR :=./gtest
MYSQL_LIB_DIR :=/usr/lib64/mysql
MYSQL_HEARD_DIR :=/usr/include/mysql

INCLUDES_DIRS := -I$(GTEST_DIR)/include -I../utils -I../common -I../designMode -I../logger -I../songLib -I$(MYSQL_HEARD_DIR) -I../search -I../refresh
LIBS_DIRS := -L$(GTEST_DIR)/lib -L$(MYSQL_LIB_DIR)
LIBS :=-lpthread $(GTEST_DIR)/lib/gtest.a -lmysqlclient

BIN := test
SRCS := test_main.cpp Profile.cpp profile_test.cpp logger_test.cpp logger.cpp filelock.cpp webLib_test.cpp lib.cpp webLib.cpp MySqlHelper.cpp Utility.cpp utility_test.cpp localLib.cpp localLib_test.cpp Converter.cpp searchFileGenerator.cpp searchfile_test.cpp refresher.cpp refresher_test.cpp PacSocket.cpp
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

vpath %.cpp ../utils ../logger ../songLib ../search ../refresh
vpath %.h  ../common ../utils ../logger ../songLib  ../search ../refresh

all:$(OBJS)
	g++ -g -Wall -o $(BIN) $(OBJS) $(LIBS_DIRS) $(LIBS)
.cpp.o:$(SRCS)
	g++ -g -Wall -o $@ -c $< $(INCLUDES_DIRS)	

run_test:
	./$(BIN)

clean:
	rm -f *.o
	rm -f *.d
	rm -f $(BIN)
install:
	
.PHONY: all clean run_test install
