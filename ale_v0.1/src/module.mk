MODULE := src/

MODULE_OBJS := \
	src/main.o 

MODULE_SOURCES := \
	src/main.cpp 

MODULE_HEADERS := \
	src/ale_interface.hpp

MODULE_DIRS += \
	src/

# Include common rules 
include $(srcdir)/common.rules