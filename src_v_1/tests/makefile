
DIR_TEST = ./EventLoop_test

tests: 
	for dir in $(DIR_TEST); do\
		(cd $$dir; ${MAKE});\
	done

.PHONY: clean

clean:
	rm -f $(SRC_OBJ)
	for dir in $(DIR_TEST); do\
		(cd $$dir; ${MAKE} clean);\
	done