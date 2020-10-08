include makefile.conf

all: $(TARGET)

$(TARGET): $(JRBTREE_OBJS)
	$(AR) $@ $^

clean:
	$(RM) $(jRBTREE_OBJS)
	$(RM) $(TARGET)

