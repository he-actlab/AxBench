CC=g++
CFLAGS=-O3 -MD -mavx

INCLUDES=-I../ParrotLib -I../FannLib -I../MeasureLib

LFLAGS=#-L../ParrotLib/

LIBS=../ParrotLib/libParrot.a ../FannLib/libFann.a ../MeasureLib/libMeasure.a #-lm

SRCS=$(wildcard *.cpp)

# $(name:string1=string2)
# For each word in 'name' replace 'string1' with 'string2'
OBJS=$(SRCS:.cpp=.o)

OUT_DIR=out
OUT=ParrotEvaluator.out

#OUT_OBJS=$(addprefix $(OUT_DIR)/, $(TEMP_OBJS))

.PHONY: depend clean

all: $(OUT)

$(OUT): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LFLAGS) $(LIBS) -o $(OUT_DIR)/$(OUT)

# This is a suffix replacement rule for building .o's from .c's.
# It uses 'automatic variables' $<: the name of the prerequisite of
# the rule (a .c file) and $@: the name of the target of the rule
# (a .o file) 
%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) *.o *.d *.tmp

dist: clean
	$(RM) *.data
	$(RM) $(OUT_DIR)/*.out

test:
#	time $(OUT_DIR)/$(OUT) 1 ./parrot.nn ./parrot.data print
	time $(OUT_DIR)/$(OUT) 1 ./parrot.nn ./parrot.data parrot 100000 | tee parrot.txt
#	time $(OUT_DIR)/$(OUT) 1 ./parrot.nn ./parrot.data fann   100000 | tee fann.txt
#	time $(OUT_DIR)/$(OUT) 1 ./parrot.nn ./parrot.data verify
	
