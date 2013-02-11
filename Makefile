TARGET  := screenxtv-gcc-client
CC		= g++
LIBS    = -lpthread -lutil
LPATH	= lib
HEADERS	= $(wildcard $(LPATH)/*.h)
OBJS	:= $(patsubst %.h,%.out,$(HEADERS))

.PHONY: all clean distclean 
all:: ${TARGET} 

${TARGET}: main.out 
	${CC} -o ${TARGET} main.out ${LIBS}

main.out: main.cc $(OBJS) 
	${CC} -o $@ -c $<

$(OBJS): $(HEADERS)
	${CC} -o $*.out -c $*.h

run::
	./${TARGET} ${ARGS}

clean:: 
	echo $(OBJS)
	-rm -f *.o *.out ${TARGET} ${LPATH}/*.out

distclean:: clean
