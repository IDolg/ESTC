BUILD_ROOT = ../../..

OBJS = SetColor.o
DEPS = SetColor.d

include $(BUILD_ROOT)/common_defs.mk

LIB = libSetColor.a

all: $(LIB)
	@echo "Done"

$(LIB): $(OBJS)
	@echo "Linking library $@"
	$(MAKE_LIBRARY)

clean:
	@echo "Cleaning directory"
	@rm -f $(OBJS) $(DEPS) $(LIB)
