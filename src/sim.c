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
    func = instruction & 0x3f;
    rs = (instruction >> 21) & 0x1f;
    rt = (instruction >> 16) & 0x1f;
    rd = (instruction >> 11) & 0x1f;
  } else {                     // if i != 0, I-type, J-type
    if (op == 2 || op == 3) {  // J-type
      // The op is a Jump or JAL operation
      // Nothing to do if it's a jump
    } else {
      // Any jump instructions not supported (all besides J, JAL) will be
      // misclassified as I type
      rs = (instruction >> 21) & 0x1f;  // 0-31
      rt = (instruction >> 16) & 0x1f;
      itemp = instruction & (uint32_t)(0xffff);
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
  NEXT_STATE.REGS[rt] =
      ((uint32_t)(CURRENT_STATE.REGS[rs])) + ((uint32_t)(itemp));
}

void execute_bgtz() {
  printf("bgtz\n");
  if (CURRENT_STATE.REGS[rs] > 0) {
    uint32_t addr = CURRENT_STATE.PC + (itemp + 1) * 4;
    NEXT_STATE.PC = addr;
  }
}

void execute_lb() {
  printf("lb\n");
  uint32_t addr = CURRENT_STATE.REGS[rs];
  NEXT_STATE.REGS[rt] = (CURRENT_STATE.REGS[rt] & (0xffffff00)) |
                        (mem_read_32(addr + itemp) & 0xff);
}

void execute_lw() {
  printf("lw\n");
  uint32_t addr = CURRENT_STATE.REGS[rs];
  NEXT_STATE.REGS[rt] = mem_read_32(addr + itemp);
}

void execute_sb() {
  printf("sb\n");
  uint32_t addr = CURRENT_STATE.REGS[rs];
  // 32 bit registers
  // replace last 8 bits
  // read in first 24 bits from memory (chopping off the rest)
  uint32_t temp_existing = mem_read_32(addr + itemp) & 0xffffff00;
  // AND value in register with ff will chop off the most signficant 24 bits
  // (keeping only the last 8) ORRing the two will keep the first bits from
  // memory and drop in new bits from register
  mem_write_32(addr + itemp, (CURRENT_STATE.REGS[rt] & 0xff) | temp_existing);
}

void execute_sw() {
  printf("sw\n");
  uint32_t addr = CURRENT_STATE.REGS[rs];
  mem_write_32(addr + itemp, CURRENT_STATE.REGS[rt]);
}

void execute_jump() {
  printf("jump\n");
  uint32_t addr = ((instruction & 0x3ffffff) << 2);
  addr = addr | (mem_read_32(CURRENT_STATE.PC) & (0xf << 28));
  NEXT_STATE.PC = addr;
}

void execute_jal() {
  printf("jal\n");
  uint32_t addr = ((instruction & 0x3ffffff) << 2);
  addr += CURRENT_STATE.PC & (0xf << 28);
  NEXT_STATE.PC = addr;
  NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
}

void execute_jr() {
  printf("jr\n");
  NEXT_STATE.PC = CURRENT_STATE.REGS[rs];
}

void execute_lui() {
  // 3c 01 10 01
  // 0011 1100 0000 0001 0001 0000 0000 0001
  // ____ __ = 1111 = 15                              OPCODE
  //        __ ___ = 0                                RS
  //              _ ____ = 1                          RT
  //                     ____ ____ ____ ____ = 4097   IMM (itemp)
  printf("lui\n");
  printf("rt is %d\n", rt);
  printf("itemp is %04x\n", itemp);
  NEXT_STATE.REGS[rt] = itemp << 16;
}

void execute() {
  printf("\nexecute\t");
  if (op == 0) {  // R type instruction
    printf("R-type\t");
    switch (func) {
      case 8:
        // JR = 8 == return
        execute_jr();
        break;
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
      switch (op) {
        case 2:
          // J = 2
          execute_jump();
          break;
        case 3:
          // JAL = 3 (BONUS)
          execute_jal();
          break;
      }
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
        case 15:
          // LUI = 15
          execute_lui();
          break;
        case 32:
          // LB = 32 (BONUS)
          execute_lb();
          break;
        case 35:
          // LW = 35
          execute_lw();
          break;
        case 40:
          // SB = 40 (BONUS)
          execute_sb();
          break;
        case 43:
          // SW = 43
          execute_sw();
          break;
      }
    }
  }
}
void process_instruction() {
  /*
   * execute one instruction here. You should use CURRENT_STATE and modify
   * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
   * access memory.
   * */
  fetch();
  decode();
  execute();
}
