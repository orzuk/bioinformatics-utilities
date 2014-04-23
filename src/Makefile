PROJECT = libbioinfo.a
CC = g++
LIBS =  -lm
CFLAGS = -O3 
EXTRA_LIBS=
LFLAGS=
DEF = -DU_INT_DEF
DBFLAGS=

TEST_SRC = \
	dna_utils.cpp \
	hmm_chrom_funcs.cpp \
	markov.cpp \


TEST_OBJS = \
	dna_utils.o \
   	hmm_chrom_funcs.o \
    	markov.o \

all: $(PROJECT)

.cpp.o:
	g++ -c $(DBFLGS) $(LIBS) $<


$(PROJECT): $(TEST_OBJS)
	


	ar -cvq ../lib/libbioinfo.a *.o