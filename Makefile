all:
	$(MAKE) -C demo/c

clean:
	@-$(MAKE) -C demo/c $@
