
#実行ファイル名
APP = bfvm

#それぞれのソースを列挙
SRCS = src/main.c src/vm_create.c src/vm_exec.c

OBJS = $(SRCS:.c=.o)

### command and flags ###
# uncomment when debugging
#DEBUG	= -ggdb -pg # -lefence

# common (*.o)
LD	= gcc
LDFLAGS	+= -g $(DEBUG)
LDLIBS	+= -lm

# C (*.c)
CC	= gcc
CFLAGS	+= -g -O3 -Wall $(DEBUG)
CPPFLAGS += -I.

# C++ (*.cc)
CXX	= g++
CXXFLAGS += -g -O3 -Wall $(DEBUG)

# etc
RM	= rm -f

### rules ###

.SUFFIXES:
.SUFFIXES: .o .c .cc

all: $(APP)

$(APP): $(OBJS)
	$(LD) $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS)

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $(@D)/$(<F) -o $(@D)/$(@F)
.cc.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $(@D)/$(<F) -o $(@D)/$(@F)

### useful commands ###

clean:
	$(RM) $(APP) $(OBJS)
	$(RM) core gmon.out *~ #*#
