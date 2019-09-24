OBJS := v4l2grab_Anysize.o

CC = gcc
RM = rm -rf

LDFLAGS += -ljpeg

Anys:$(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o:%.c
	$(CC) -c $< -o $@

.PHONY : clean

clean:
	$(RM) *.o Anys
