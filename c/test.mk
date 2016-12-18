all:    START rule1 rule2 END

.PHONY:
START:
	@./MakeTime.py --clear
	@./MakeTime.py $@ Elapsed during the entire make process.

.PHONY:
rule1:
	@./MakeTime.py $@ Elapsed during rule1.
	sleep 3
	@./MakeTime.py $@

.PHONY:
rule2:
	@./MakeTime.py $@ Elapsed during rule2.
	sleep 5
	@./MakeTime.py $@

.PHONY:
END:
	@./MakeTime.py START
	@./MakeTime.py --summary
