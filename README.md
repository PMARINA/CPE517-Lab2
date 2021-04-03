# CPE517-Lab2

[CPE 517-A: Spring 2020](https://personal.stevens.edu/~hliu77/teaching.html)

## Contributors

### Lab Materials

- **Professor** [Hang Liu](mailto:Hang.Liu@stevens.edu)
- **TA** [Mingju He](mailto:mhe6@stevens.edu)

### Students completing this lab

- [Julia Chung](https://github.com/chungiee)

- [Pridhvi Myneni](https://github.com/PMARINA)

## Instructions to Run

### Install Pre-Requisite Libraries

`pip3 install --upgrade python-magic loguru`

Change to `pip` if `pip3` not installed and `pip` corresponds to `python3`.

### Compile & Run

1. Run `make`
2. Run `build/sim build/test/*/___.txt`
   - `build/test/*/___.txt` is the test file you wish to run.
3. Check the `test/*/___.asm` file for comments regarding expected output and evaluation.

Note that `build/test/*/___.txt` is generated based on `test/*/___.asm` when `make` is run.

See [this repository's github.io page](https://pmarina.github.io/CPE517-Lab2/) for more info about how the instructions were implemented.

## Instructions to Contribute

1. `git clone ...`
2. `pip install --upgrade pre-commit`
3. `pre-commit install`
