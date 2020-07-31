# makefile to build all linux related plugins in this plugin project.

all:
	$(MAKE) -C leechp_helloworld_linux

clean:
	$(MAKE) -C leechp_helloworld_linux clean
