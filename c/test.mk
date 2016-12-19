#!/usr/bin/env make
#### EXAMPLE Makefile to unit test ./MakeTime.py ###
all:    START rule1 rule2 END
	@echo '>>>	rule: $@: rule END should output something like:'
	@echo '     8.128 seconds [START] Elapsed during the entire make process.'
	@echo '     3.029 seconds [rule1] Elapsed during rule1.'
	@echo '     5.025 seconds [rule2] Elapsed during rule2.'

.PHONY:
START:
	@echo '>>>	rule: $@'
	@./MakeTime.py --clear
	@./MakeTime.py $@ Elapsed during the entire make process.

.PHONY:
rule1:
	@echo '>>>	rule: $@'
	@./MakeTime.py $@ Elapsed during rule1.
	sleep 3
	@./MakeTime.py $@

.PHONY:
rule2:
	@echo '>>>	rule: $@'
	@./MakeTime.py $@ Elapsed during rule2.
	sleep 5
	@./MakeTime.py $@

.PHONY:
END:    # Note how the label 'START' is re-used from the START rule.
	@echo '>>>	rule: $@'
	@./MakeTime.py START
	@./MakeTime.py --report
###############################################################################

