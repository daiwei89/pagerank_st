PR_ROOT := $(shell readlink $(dir $(lastword $(MAKEFILE_LIST))) -f)
SRC_DIR = $(PR_ROOT)/src

PR_THIRD_PARTY = $(PR_ROOT)/third_party
PR_THIRD_PARTY_SRC = $(PR_THIRD_PARTY)/src
PR_THIRD_PARTY_INCLUDE = $(PR_THIRD_PARTY)/include
PR_THIRD_PARTY_LIB = $(PR_THIRD_PARTY)/lib

PR_CXX = g++
PR_CXXFLAGS = -g -O3 \
           -std=c++0x \
           -Wall \
					 -Wno-sign-compare \
           -fno-builtin-malloc \
           -fno-builtin-calloc \
           -fno-builtin-realloc \
           -fno-builtin-free \
           -fno-omit-frame-pointer
PR_INCFLAGS = -I$(PR_THIRD_PARTY_INCLUDE)
PR_LDFLAGS = -Wl,-rpath,$(PR_THIRD_PARTY_LIB) \
          -L$(PR_THIRD_PARTY_LIB) \
          -pthread -lrt -lnsl -luuid \
          -lglog \
          -lgflags \
          -ltcmalloc

# defined in defns.mk
THIRD_PARTY = $(PR_THIRD_PARTY)
THIRD_PARTY_SRC = $(PR_THIRD_PARTY_SRC)
THIRD_PARTY_LIB = $(PR_THIRD_PARTY_LIB)
THIRD_PARTY_INCLUDE = $(PR_THIRD_PARTY_INCLUDE)

NEED_MKDIR = $(THIRD_PARTY_SRC) \
             $(THIRD_PARTY_LIB) \
             $(THIRD_PARTY_INCLUDE)

PR_SRC = $(wildcard $(SRC_DIR)/*.cpp)
PR_HDR = $(wildcard $(SRC_DIR)/*.hpp)
PR_BIN = $(PR_ROOT)/bin
PR = $(PR_BIN)/pagerank_main

pagerank: $(PR)

$(PR): $(PR_SRC)
	mkdir -p $(PR_BIN)
	$(PR_CXX) $(PR_CXXFLAGS) $(PR_INCFLAGS) $^ \
	$(PR_LDFLAGS) -o $@

$(PR_OBJ): %.o: %.cpp $(PR_HDR)
	$(PR_CXX) $(MSFT_CXXFLAGS) -I$(SRC_DIR) $(INCFLAGS) -c $< -o $@


all: path \
     third_party_core

path: $(NEED_MKDIR)

$(NEED_MKDIR):
	mkdir -p $@

clean:
	rm -rf $(PR_BIN)

.PHONY: all path pagerank clean

include $(THIRD_PARTY)/third_party.mk
