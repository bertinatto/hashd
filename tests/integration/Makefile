.PHONY: run
run:
	@echo python3 main.py
	@cd ../../src/ && $(MAKE) hashd && ./hashd &
	@python3 main.py -afd
