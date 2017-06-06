default: all

.DEFAULT:
	cd src && $(MAKE) $@

.PHONY: tests
tests:
	cd tests        && \
	$(MAKE) clean   && \
	$(MAKE) compile && \
	$(MAKE) run     && \
	$(MAKE) clean

.PHONY: clean
clean:
	cd src && $(MAKE) clean
	cd tests && $(MAKE) clean
