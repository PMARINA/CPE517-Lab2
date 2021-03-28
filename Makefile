build/sim: src/shell.c src/sim.c
	mkdir -p build
	gcc -g -O2 $^ -o $@

run: build/sim
	build/sim 'add(i).txt'

.PHONY: clean

clean:
	rm -rf *.o *~ sim
