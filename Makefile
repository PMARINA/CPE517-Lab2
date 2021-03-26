build/sim: src/shell.c src/sim.c
	clang -g -O2 $^ -o $@
run: build/sim
	build/sim
.PHONY: clean
clean:
	rm -rf *.o *~ sim
