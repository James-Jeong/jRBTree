include makefile.conf

all: $(TARGET)

$(TARGET): $(JRBTREE_OBJS)
	$(AR) $@ $^

clean:
	$(RM) $(JRBTREE_OBJS)
	$(RM) $(TARGET)

