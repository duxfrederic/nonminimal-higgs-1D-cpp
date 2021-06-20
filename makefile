CC = g++
CFLAGS = -std=c++11 -Wall -pedantic -ansi -g -O3 -funroll-loops -mavx -mavx2 -msse -msse2 -msse4
EXEC_NAME = finite_differences
INCLUDES =
LIBS =
OBJ_FILES = finite_differences.o

all : $(EXEC_NAME)

clean :
	rm $(EXEC_NAME) $(OBJ_FILES) *.out*

$(EXEC_NAME) : $(OBJ_FILES)
	$(CC) -o $(EXEC_NAME) $(OBJ_FILES) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<




