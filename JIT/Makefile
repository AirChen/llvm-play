LLVM_CONFIG ?= ../bin_3.4/bin/llvm-config

ifndef VERBOSE
QUIET := @
endif

SRC_DIR ?= $(PWD)

LLVM_LDFLAGS := $(shell $(LLVM_CONFIG) --ldflags)
COMMON_FLAGS = -Wall -Wextra
LLVM_CXXFLAGS += $(COMMON_FLAGS) $(shell $(LLVM_CONFIG) --cxxflags)
LLVM_CPPFLAGS += $(shell $(LLVM_CONFIG) --cppflags) -I$(SRC_DIR)
LLVM_LIBS = $(shell $(LLVM_CONFIG) --libs jit interpreter nativecodegen)

objects = Driver.o Expr.o Lexer.o Parser.o
name = driver

default: $(name)

$(name) : $(objects)
		@echo Linking $@
		$(QUIET)$(CXX) -o $@ $(LLVM_CXXFLAGS) $(LLVM_LDFLAGS) $^ $(LLVM_LIBS)

%.o : %.cpp
		@echo Compiling $*.cpp
		$(QUIET)$(CXX) -c $< $(LLVM_CPPFLAGS) -o $@

#--- expand ---#
# Driver.o : Driver.cpp
# 		@echo Compiling $@
# 		$(QUIET)$(CXX) -c $< $(LLVM_CPPFLAGS) -o $@

# Parser.o : Parser.cpp
# 		@echo Compiling $@
# 		$(QUIET)$(CXX) -c $< $(LLVM_CPPFLAGS) -o $@

# Expr.o : Expr.cpp
# 		@echo Compiling $@
# 		$(QUIET)$(CXX) -c $< $(LLVM_CPPFLAGS) -o $@

# Lexer.o : Lexer.cpp
# 		@echo Compiling $@
# 		$(QUIET)$(CXX) -c $< $(LLVM_CPPFLAGS) -o $@

clean::
		$(QUIET)rm -f $(name) $(objects)
