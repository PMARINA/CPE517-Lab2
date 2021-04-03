all: build/sim buildtest

build/sim: src/shell.c src/sim.c
	mkdir -p build
	gcc -g -O2 $^ -o $@

run: build/sim
	build/sim 'add(i).txt'

buildtest: utils/Mars4_5.jar utils/build_test.py
	python3 utils/build_test.py

.PHONY: clean

clean:
	rm -rf *.o *~ sim
