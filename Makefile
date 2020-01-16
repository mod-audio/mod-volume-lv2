all:
	$(MAKE) -C volume/source
	$(MAKE) -C volume-2x2/source

install: all
	mkdir bundles
	cp -r volume/source/mod-volume.lv2 bundles/
	cp -r volume-2x2/source/mod-volume-2x2.lv2 bundles/

clean:
	$(MAKE) clean -C volume/source
	$(MAKE) clean -C volume-2x2/source
