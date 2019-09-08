OBJS := v4l2grab_Anysize.o

CC = gcc
RM = rm -rf


Anys:$(OBJS)
	$(CC) $< -o $@

%.o:%.c
	$(CC) -c $< -o $@

.PHONY : clean

clean:
	$(RM) *.o Anys
