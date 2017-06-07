default: all

.DEFAULT:
	cd src && $(MAKE) $@

.PHONY: tests
tests:
	@cd tests/unit          && \
	$(MAKE) clean           && \
	$(MAKE) compile         && \
	$(MAKE) run             && \
	$(MAKE) clean
	@cd tests/integration && $(MAKE) run

.PHONY: clean
clean:
	cd src && $(MAKE) clean
	cd tests/unit && $(MAKE) clean
