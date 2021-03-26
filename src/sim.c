// Julia Chung, Pridhvi Myneni
#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shell.h"

uint8_t op;
uint32_t instruction;
uint32_t rs, rt, rd;  // to save the register value
uint8_t func;
uint16_t itemp;  // for save immidiate number

// please take a loot at
// https://inst.eecs.berkeley.edu/~cs61c/resources/MIPS_Green_Sheet.pdf
// or
//  https://web.cse.ohio-state.edu/~crawfis.3/cse675-02/Slides/MIPS%20Instruction%20Set.pdf

void fetch() {
  printf("fetch ");
  instruction = mem_read_32(CURRENT_STATE.PC);  // the instruction
  NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}

void decode() {
  // printf("decode ");
  op = instruction >> 26;  // find the 31-26 bit
  //  printf("the op: %x ",op);
  // if op==0, it is R type
  // if I type, there is no funct, so set func based off of opcode
  if (op == 0) {  // R type instructions
    rs = (instruction >> 21) & 0x1f;
    rt = (instruction >> 16) & 0x1f;
    rd = (instruction >> 11) & 0x1f;
    func = instruction & 0x3f;
  } else {                     // if i != 0, I-type, J-type
    if (op == 2 || op == 3) {  // J-type
      // The op is a Jump or JAL operation
      rs = instruction & 0x3ffffff;
    } else {
      // Any jump instructions not supported (all besides J, JAL) will be
      // misclassified as I type
      rs = (instruction >> 21) & 0x1f;  // 0-31
      rt = (instruction >> 16) & 0x1f;
      itemp = instruction & 0xffff;
    }
    func = op;
  }
}

void execute_addi() {
  printf("addi\n");
  if (itemp > 32767) {   // 2^15-1=32767 is the largest number
    itemp = ~itemp + 1;  // 2's complement
    NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] - itemp;
  } else {
    NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] + itemp;
  }
  // NEXT_STATE.REGS[addr_rt] = rt;  // It is fine if you just execute here
  // instead of executing in execute() function.
}

void execute_addiu() {
  printf("addiu\n");
  if (itemp > 32767) {   // 2^15-1=32767 is the largest number
    itemp = ~itemp + 1;  // 2's complement
    NEXT_STATE.REGS[rt] = (uint32_t)(CURRENT_STATE.REGS[rs]) - itemp;
  } else {
    NEXT_STATE.REGS[rt] = (uint32_t)(CURRENT_STATE.REGS[rs]) + itemp;
  }
}

void execute_bgtz() {
  printf("bgtz\n");
  // NEXT_STATE.REGS[PC] =
  //     0;
  // TODO: Figure out where the operand containing the address and
  // containing the conditional variable is stored.
}

void execute_lw() {
  printf("lw\n");
  uint32_t addr = CURRENT_STATE.REGS[rs];
  NEXT_STATE.REGS[rt] = mem_read_32(addr + itemp);
}

void execute_sw() {
  printf("sw\n");
  uint32_t addr = CURRENT_STATE.REGS[rs];
  mem_write_32(addr + itemp, CURRENT_STATE.REGS[rt]);
}

void execute() {
  printf("\nexecute\t");
  if (op == 0) {  // R type instruction
    printf("R-type\t");
    switch (func) {
      case 32:  // add:100000
        // ADD = 32
        printf("add\n");
        //      if(rt>2147483647)
        NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] + CURRENT_STATE.REGS[rt];
        break;
      case 33:
        // ADDU = 33
        printf("addu\n");
        NEXT_STATE.REGS[rd] = (uint32_t)(CURRENT_STATE.REGS[rs]) +
                              (uint32_t)(CURRENT_STATE.REGS[rt]);
        break;
      case 34:
        // SUB = 34
        printf("sub\n");
        NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] - CURRENT_STATE.REGS[rt];
        break;
      case 35:
        // SUBU = 35
        printf("subu\n");
        NEXT_STATE.REGS[rd] = (uint32_t)(CURRENT_STATE.REGS[rs]) -
                              (uint32_t)(CURRENT_STATE.REGS[rt]);
        break;
      case 36:
        // AND = 36
        printf("and\n");
        NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] & CURRENT_STATE.REGS[rt];
        break;

      case 12:                              // system call:001100
        if (CURRENT_STATE.REGS[2] == 10) {  // v0==10 then exit
          printf("systemcall: exit\n");
          RUN_BIT = FALSE;
        }
        if (CURRENT_STATE.REGS[2] == 1) {       // v0==1: print int
          int32_t val = CURRENT_STATE.REGS[4];  // a0 is 4th register
          printf("\n print the number:   %d \n ", val);
        }
        break;
      default:
        break;
    }
  } else {
    if (op == 2 || op == 3) {
      printf("J-type\t");
      // Execute Jumps
      // J = 2
      // JAL = 3
    } else {
      printf("I-type\t");
      // Execute I-type
      switch (op) {
        case 7:
          // BGTZ = 7
          execute_bgtz();
          break;
        case 8:
          // ADDI = 8
          execute_addi();
          break;
        case 9:
          // ADDIU = 9
          execute_addiu();
          break;
        case 35:
          // LW = 35
          execute_lw();
          break;
        case 43:
          execute_sw();
          break;
          // SW = 43
      }
    }
  }
}
void process_instruction() {
  /* execute one instruction here. You should use CURRENT_STATE and modify
   * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
   * access memory. */
  fetch();
  decode();
  execute();
}
