# CPE 517-A: Lab 2 (Spring 2021)

## What?

This is a simple and only partially complete implementation of the MIPS instruction set (I believe version MIPS32). It is intended to mimic [MARS](http://courses.missouristate.edu/kenvollmar/mars/) albeit with little more than the lowest functional layer, a limited interactive layer, and no graphical interface.

The lab was completed for [CPE 517 (Course Page)](https://personal.stevens.edu/~hliu77/teaching.html) at the [Stevens Institute of Technology](https://stevens.edu), Spring 2021. The lab material can be found in the [docs/](https://github.com/PMARINA/CPE517-Lab2/tree/main/docs) and [src/](https://github.com/PMARINA/CPE517-Lab2/tree/main/src) paths on the main branch. The objective was to complete the execute and decode methods (partially filled) in [src/sim.c](https://github.com/PMARINA/CPE517-Lab2/blob/main/src/sim.c).

## Credits

This lab was assigned by our course instructor, [Professor Hang Liu](mailto:Hang.Liu@stevens.edu) and our TA [Mingju He](mailto:mhe6@stevens.edu). All files in the [docs/](https://github.com/PMARINA/CPE517-Lab2/tree/main/docs) directory were provided, as well as [src/Lab2example.c](https://github.com/PMARINA/CPE517-Lab2/blob/main/src/Lab2example.c), [src/shell.c](https://github.com/PMARINA/CPE517-Lab2/blob/main/src/shell.c), [src/shell.h](https://github.com/PMARINA/CPE517-Lab2/blob/main/src/shell.h), and a fair portion of [src/sim.c](https://github.com/PMARINA/CPE517-Lab2/blob/main/src/sim.c). 

The lab was completed by [Julia Chung](https://github.com/chungiee) and [Pridhvi Myneni](https://github.com/PMARINA).

## Explanation of Task

The [reference material](https://inst.eecs.berkeley.edu/~cs61c/resources/MIPS_Green_Sheet.pdf) provides all details required for implementation of most instructions; however, it might be helpful to use a tool such as [utils/decode_instructions.py](https://github.com/PMARINA/CPE517-Lab2/blob/main/utils/decode_instruction.py) to help you understand the various arguments and values encoded into the instructions. I also heavily recommend compiling various versions of instructions in [MARS](http://courses.missouristate.edu/kenvollmar/mars/) and running them to identify three important things (as applicable) about each instruction:

1) How is the IMM value used (especially for branching and jumping instructions)
   a) Does it need to be sign extended? How does the sign extension happen with a < 32 bit number?
2) How is the destination register treated (especially when a < 32 bit value is being assigned to the register)?
3) Which memory address is being modified? Are the high or low bits being modified?

Make sure you test every instruction with every option available. This would include jumping, branching, and jumping and linking with offsets, and negative values for add, addu, etc.

## Important Note Regarding Variants of Instructions

Many architectures have different add or subtract instructions, which vary based on if the operands are signed or not. This is generally observed via the overflow and other flags. As the MIPS architecture (version that I am using) does not specify an accessible status register (unlike the ARM instruction set), the only way in which this is observable is if the emulator or chip implementation results in a [trap](https://en.wikipedia.org/wiki/Trap_(computing)). 

As the MARS emulator does not result in traps/exceptions for these situations, I have also omitted any distinction. Additionally, [src/shell.h](https://github.com/PMARINA/CPE517-Lab2/blob/main/src/shell.h) appears to include a way to set flags through the `CPU_State` struct; however, I have been assured this will not be relevant for the grading of this assignment, and have thus omitted it. 

Future versions of the MIPS instruction set seem to have noted this limited distinction, and seem to be omitting these semi-redundant instructions from their instruction sets as well, to reassign the op-codes/func values to more important operations. 
