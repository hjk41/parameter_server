CC = g++
# OPT = -O0 -ggdb
OPT = -O3 -ggdb

BIN = ../bin
LIB = ../lib

ifndef THIRD
THIRD=$(shell cd ..;pwd)/third_party
endif
THIRD_BIN=$(THIRD)/bin
THIRD_INC=$(THIRD)/include
THIRD_LIB=$(THIRD)/lib

#-Wconversion -fPIC #-fno-omit-frame-pointer
WARN = -Wall -Wno-unused-function -finline-functions -Wno-sign-compare
CFLAGS = -std=c++0x $(WARN) $(OPT) -I. -I${THIRD_INC} #-msse4.2 #-fopenmp #-fPIC #

ifeq (${STATIC}, 1)
CFLAGS += -static -static-libgcc -static-libstdc++
endif

LDFLAGS = -L$(THIRD_LIB) -L$(LIB) -lgflags -lzmq -lprotobuf -lglog -lz \
-lsnappy -lm -lpthread  -lstdc++ # -ltcmalloc_and_profiler # -lrt
GTEST = -lgtest_main -lgtest

PROTOS= $(wildcard */*.proto)
PROTOS_CC = $(PROTOS:.proto=.pb.cc)

all: dir $(BIN)/ps
clean:
	rm -rf */*.o */*.d */*/*.o */*/*.d proto/*.cc $(BIN) $(LIB)

-include */BUILD
# system
SYS_SRCS = $(PROTOS_CC) \
	$(addprefix system/, $(system_srcs)) \
	$(addprefix util/, $(util_srcs)) \
	$(addprefix data/, $(data_srcs)) \
	$(addprefix filter/, $(filter_srcs))
SYS_OBJS = $(SYS_SRCS:.cc=.o)
$(LIB)/libps.a: $(SYS_OBJS)
	ar crv $@ $(SYS_OBJS)

# linear methods
LM_SRCS = $(addprefix linear_method/, $(linear_method_srcs))
LM_OBJS = $(LM_SRCS:.cc=.o)
$(LIB)/liblm.a: $(LM_OBJS)
	ar crv $@ $(LM_OBJS)

# graph partitioning
GP_SRCS = $(addprefix graph_partition/, $(graph_partition_srcs))
GP_OBJS = $(GP_SRCS:.cc=.o)
$(LIB)/libgp.a: $(GP_OBJS)
	ar crv $@ $(GP_OBJS)

# cxxnet

# dependencies
# $(SRCS) $(addsuffix .cc, $(addprefix test/, $(TESTS)))
ALL_SRCS = $(wildcard */*.cc)
DEPS = $(ALL_SRCS:.cc=.d)
-include $(DEPS)

LIBS = $(LIB)/liblm.a $(LIB)/libgp.a $(LIB)/libps.a

test: $(addprefix $(BIN)/, $(TESTS))
%.o: %.cc
	$(CC) $(CFLAGS) -c $< -o $@
%.d: %.cc
	$(CC) $(CFLAGS) -MM -MT $*.o $< > $@

$(BIN)/%: test/%.cc $(LIBS)
	$(CC) $(CFLAGS) $< $(GTEST) $(LIBS) $(LDFLAGS) -o $@

%.pb.cc %.pb.h : %.proto
	${THIRD_BIN}/protoc --cpp_out=. $<

$(BIN)/ps: main.cc $(LIBS)
	$(CC) $(CFLAGS) main.cc $(LIBS) $(LDFLAGS) -o $@

# make directories
dir: $(LIB) $(BIN)
$(LIB):
	mkdir -p $(LIB)
$(BIN):
	mkdir -p $(BIN)
