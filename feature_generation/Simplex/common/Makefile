.PHONY : clean check-dir

CC=g++
CFLAG=-Wall -Wextra -Wshadow -Wconversion -Werror -I./include -c
LDFLAG=-lm
OBJDIR=$(shell pwd)/obj
LIBDIR=$(shell pwd)/lib

ifeq ($(DEBUG), 1)
	CFLAG +=-DDEBUG -g
else
	CFLAG +=-O2
endif

all : $(LIBDIR)/util.a

SRCS=util

clean : 
	rm -f $(OBJDIR)/* $(LIBDIR)/*

check-dir :
	mkdir -p $(OBJDIR)
	mkdir -p $(LIBDIR)


$(LIBDIR)/util.a : $(addsuffix .o, $(addprefix $(OBJDIR)/, $(SRCS))) 
	ar rvs $@ $^

$(OBJDIR)/%.o : src/%.cpp
	$(CC) $(CFLAG) $^ -o $@
