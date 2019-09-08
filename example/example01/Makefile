OBJS += capture.o
OBJS += display.o
OBJS += main.o

TARGET = capture

LDFALG += -lSDL2 \
		  -Wall

$(TARGET):$(OBJS)
	g++ $^ -o $@ $(LDFALG)


%.o:%.cpp
	g++ -c $< -o $@


.PHONY: clean

clean:
	rm *.o $(TARGET)