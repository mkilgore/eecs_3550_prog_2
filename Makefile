
PROG := main

CPPFLAGS := -I./include

SRCDIR := ./src

SRCS := $(wildcard $(SRCDIR)/*.cpp)
SRCS_OBJS := $(patsubst %.cpp,%.o,$(SRCS))
SRCS_DEPS := $(patsubst %.cpp,%.d,$(SRCS))
SRCS_DEPS := $(foreach file,$(SRCS_DEPS),$(dir $(file)).$(notdir $(file)))

all: $(PROG)

.%.d: %.cpp
	$(CXX) -MM -MP -MF $@ $(CPPFLAGS) $< -MT ./$*.o -MT $@

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(PROG): $(SRCS_OBJS)
	$(CXX) $(CPPFLAGS) -o $@ $(SRCS_OBJS)

clean:
	rm -fr $(SRCS_OBJS) ./$(PROG) $(SRCS_DEPS)

ifneq ($(MAKECMDGOALS),clean)
-include $(SRCS_DEPS)
endif


