TARGET  := screenxtv-gcc-client
CC		= g++
LIBS    = -lpthread -lutil
LPATH	= lib
HEADERS	= $(wildcard $(LPATH)/*.h)
OBJS	:= $(patsubst %.h,%.out,$(HEADERS))

.PHONY: all clean distclean 
all:: ${TARGET} 

${TARGET}: main.out $(OBJS)
	${CC}  -o ${TARGET} main.out ${LIBS}

main.out: main.cc
	${CC} -c -o $@ $^ 

$(OBJS): $(HEADERS)
	${CC} -c -o $@ $< 

clean:: 
	-rm -f *.o *.out ${TARGET} ${LPATH}/*.out

distclean:: clean
