all:
	$(MAKE) -C volume/source
	$(MAKE) -C volume-2x2/source

install: all
	$(MAKE) install -C volume/source
	$(MAKE) install -C volume-2x2/source

clean:
	$(MAKE) clean -C volume/source
	$(MAKE) clean -C volume-2x2/source
