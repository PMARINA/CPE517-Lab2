# CPE517-Lab2

[CPE 517-A: Spring 2021](https://personal.stevens.edu/~hliu77/teaching.html)

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

## Fully Tested Instructions

We only count instructions as fully tested when all variations of the instructions have been tested and their side-effects either match MARS or the expected spec (ie 32 bit numbers are not used in imm).

### I-Type

- `ADDI` = Add Immediate
  - [test](test/i-type/addi.asm)
- `ADDIU` = Add Immediate Unsigned (equivalent `ADDI`)
  - [test](test/i-type/addiu.asm)
- `BGTZ` = Branch if Greater than Zero
  - [test 1](test/i-type/bgtz.asm)
  - [test 2](test/i-type/bgtz2.asm)
- `LB` = Load Byte
  - See `SB`
- `SB` = Store Byte
  - [test](test/i-type/lbsb.asm)
- `LW` = Load Word
  - See `SW`
- `SW` = Store Word
  - [test](test/i-type/lwsw.asm)

### J-Type

- `J` = Jump
  - [test](test/j-type/j.asm)
- `JAL` = Jump and Link
  - [test](test/j-type/jal.asm)

### R-Type

- `ADD` = Add
  - [test](test/r-type/add.asm)
- `ADDU` = Add (equivalent `ADD`)
  - [test](test/r-type/addu.asm)
- `AND` = Bit-wise And Operation (`&`)
  - [test](test/r-type/and.asm)
- `SUB` = Subtract
  - [test](test/r-type/sub.asm)
- `SUBU` = Subtract Unsigned (equivalent `SUB`)
  - [test](test/r-type/subu.asm)
