CC			:= g++
LD			:= g++


include ../config.mk

CFLAGS		:= -Wall -lfann -lboost_regex -std=c++11
LFLAGS		:= -lfann -lboost_regex -lParrot
HEADERS 	:= src
INCLUDE 	:= -I${ANALOG_INC} -I${FANN_INC} -I${HEADERS}
LIB			:= -L${ANALOG_LIB} -L${FANN_LIB} -L$(PARROT_LIB)

MODULE		:= kmeans.out


CPP_FILES := $(wildcard src/*.c)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.c=.o)))

.PHONY: all clean
	
all: DIR $(MODULE)

DIR:
	@echo ${CPP_FILES}
	@echo ${OBJ_FILES}
	if [ ! -d "./bin" ];then 	\
		mkdir bin;				\
	fi
	if [ ! -d "./obj" ];then 	\
		mkdir obj;				\
	fi

$(MODULE): $(OBJ_FILES)
	$(LD) $^ $(LIB) $(LFLAGS) -o bin/$@

obj/%.o: src/%.c
	python $(PLANG) -c $(PARROT_JSON) -a observe -e "g++ $(CFLAGS) $(INCLUDE) -I$(PARROT_LIB) -c" -s "$<" -o "$@" -t
	#$(CC) -D$(NPU_MODE) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	@rm -rf *.o
	@rm -rf *.d
	@rm -rf *.out
	@rm -rf bin
	@rm -rf obj
