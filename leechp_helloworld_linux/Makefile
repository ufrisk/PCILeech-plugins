CC=gcc
CFLAGS  += -I. -I../includes -D LINUX -shared -fPIC -fvisibility=hidden
LDFLAGS += -g -shared
DEPS = 
OBJ = leechp_helloworld_linux.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

leechp_helloworld_linux: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) -o leechp_helloworld_linux.so $(LDFLAGS)
	rm *.o
	mkdir -p ../files
	mv leechp_helloworld_linux.so ../files/
	true

clean:
	rm -f *.o
