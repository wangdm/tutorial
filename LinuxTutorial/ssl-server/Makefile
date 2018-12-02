
CROSS_COMPILE :=
RM  := rm -f
AR  := $(CROSS_COMPILE)ar
CC  := $(CROSS_COMPILE)gcc
CXX := $(CROSS_COMPILE)g++
SRTIP := $(CROSS_COMPILE)strip

DEFINES  :=
INCLUDES :=
LIBRARYS :=
LDLIBS   := ssl crypto pthread

CFLAGS	 += $(strip $(addprefix -D,$(DEFINES)) $(addprefix -I,$(INCLUDES)) $(addprefix -L,$(LIBRARYS)) $(addprefix -l,$(LDLIBS)))
CXXFLAGS += $(strip $(addprefix -D,$(DEFINES)) $(addprefix -I,$(INCLUDES)) $(addprefix -L,$(LIBRARYS)) $(addprefix -l,$(LDLIBS)))

CFLAGS   += -std=c99 -fPIC -g -Wall -pedantic -Werror
CXXFLAGS += -std=c++0x -fPIC -g -Wall

PREFIX :=

SRCS :=  client.cpp alexa.cpp device.cpp socket.cpp
		 
OBJS := $(SRCS:%.cpp=%.o)


STATIC_TARGET :=
SHARED_TARGET :=
CLIENT_TARGET := client


all: $(STATIC_TARGET) $(SHARED_TARGET) $(CLIENT_TARGET)
	
$(STATIC_TARGET) : $(OBJS)
	$(AR) -rcs $(STATIC_TARGET) $(OBJS)
	
$(SHARED_TARGET) : $(OBJS)
	$(CXX) -shared -fPIC -o $(SHARED_TARGET) $(OBJS)
	
$(CLIENT_TARGET):  $(OBJS)
	$(CXX) $(OBJS) -o $(CLIENT_TARGET) $(CXXFLAGS)

strip:
	$(SRTIP) -s $(STATIC_TARGET) $(SHARED_TARGET)
	
install: all
	
clean :
	$(RM) $(OBJS)
	$(RM) $(STATIC_TARGET) $(SHARED_TARGET) $(CLIENT_TARGET)