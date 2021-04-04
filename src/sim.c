// Group: Julia Chung, Pridhvi Myneni
// Lab Assignment #2

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shell.h"

uint8_t op;
uint32_t instruction;
uint8_t rs, rt, rd; // We only save the register number because it's confusing
                    // to have copies
uint8_t func;
uint16_t itemp; // for save immidiate number

// please take a loot at
// https://inst.eecs.berkeley.edu/~cs61c/resources/MIPS_Green_Sheet.pdf
// or
//  https://web.cse.ohio-state.edu/~crawfis.3/cse675-02/Slides/MIPS%20Instruction%20Set.pdf

void fetch() {
  // printf("fetch ");
  instruction = mem_read_32(CURRENT_STATE.PC); // the instruction
  if (instruction == 0) {
    // Either there is no instruction that is entirely 0 or we are not required
    // to implement it. In either case, we rely on the shell.c doing memset with
    // 0 to ensure we don't read in random garbage.
    RUN_BIT = 0;
  }
  NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}

void decode() {
  // printf("decode ");
  op = instruction >> 26; // find the 31-26 bit
  // printf("the op: %x ", op);
  if (op == 0) { // R type instructions
    func = instruction & 0x3f;
    rs = (instruction >> 21) & 0x1f;
    rt = (instruction >> 16) & 0x1f;
    rd = (instruction >> 11) & 0x1f;
  } else {                    // if i != 0, I-type, J-type
    if (op == 2 || op == 3) { // J-type
                              // The op is a Jump or JAL operation
                              // Nothing to do if it's a jump (we don't have an
                              // address global variable)
    } else {
      // Any jump instructions not supported (all besides J, JAL) will be
      // misclassified as I type
      rs = (instruction >> 21) & 0x1f; // 0-31
      rt = (instruction >> 16) & 0x1f;
      itemp = instruction & (uint32_t)(0xffff);
    }
    func = op; // in case we accidentally touch the op, which should be 0.
  }
}

void execute_addi() {
  // printf("addi\n");
  if (itemp > 32767) {               // 2^15-1=32767 is the largest number
    itemp = ((~itemp) & 0xffff) + 1; // 2's complement
    // Taking 2's complement allows you to "negate" a number, thus we take the
    // very large number (indicative of a negative) number, and negate it and
    // then subtract it to simulate adding a negative number.
    NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] - itemp;
  } else {
    NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] + itemp;
  }
}

void execute_addiu() {
  // Add immediate unsigned
  // Per discussion on slack, addi is equivalent to addiu, except the CPU state
  // flags might be changed, depending on the specific implementation of the
  // MIPS instruction set.
  // printf("addiu\n");
  if (itemp > 32767) {
    NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] - (((~itemp) & 0xffff) + 1);
  } else {
    NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] + itemp;
  }
}

void execute_bgtz() {
  // printf("bgtz\n");
  if (CURRENT_STATE.REGS[rs] > 0) {
    // 0x1d20fffd
    // i type, opcode = 7
    // jump back imm * 4
    // jump forward (imm + 1) * 4
    // and with 0xffff because itemp is only 16 bits
    uint32_t addr;
    if (itemp > 32767) {
      addr = CURRENT_STATE.PC - ((((~itemp) & 0xffff) + 1) & 0xffff - 1) * 4;
    } else {
      addr = CURRENT_STATE.PC + (itemp + 1) * 4;
    }
    NEXT_STATE.PC = addr;
  }
}

void execute_lb() {
  // printf("lb\n");
  uint32_t addr = CURRENT_STATE.REGS[rs];
  uint32_t val;
  if (itemp > 32767) {
    itemp = ((~itemp) & 0xffff) + 1;
    val = (mem_read_32(addr - (itemp + 3) / 4 * 4) >>
           ((4 - (itemp % 4)) % 4) * 8) &
          0xff;
  } else
    val = (mem_read_32(addr + (itemp / 4) * 4) >> ((itemp % 4) * 8) & 0xff);
  if (val > 127) {
    val = val | 0xffffff00;
  }
  NEXT_STATE.REGS[rt] = (CURRENT_STATE.REGS[rt] & (0xffffff00)) | val;
}

void execute_lw() {
  // printf("lw\n");
  uint32_t addr = CURRENT_STATE.REGS[rs];
  if (itemp > 32767) {
    itemp = ((~itemp) & 0xffff) + 1;
    NEXT_STATE.REGS[rt] = mem_read_32(addr - itemp);
  } else {
    NEXT_STATE.REGS[rt] = mem_read_32(addr + itemp);
  }
}

void execute_sb() {
  // printf("sb\n");
  // printf("itemp is %d \n", itemp);
  uint32_t addr = CURRENT_STATE.REGS[rs];
  // 32 bit registers
  // replace last 8 bits
  // read in first 24 bits from memory (chopping off the rest)
  uint32_t val;
  uint32_t temp_existing;
  if (itemp > 32767) {
    itemp = ((~itemp) & 0xffff) + 1;
    // printf("itemp is %d \n", itemp);
    temp_existing = mem_read_32(addr - ((itemp + 3) / 4) * 4) &
                    (~(0xff << (24 - ((itemp % 4 - 1) * 8))));
    mem_write_32(
        addr - ((itemp + 3) / 4) * 4,
        (((CURRENT_STATE.REGS[rt] & 0xff) << (24 - ((itemp % 4 - 1) * 8)))) |
            temp_existing);
  } else {
    temp_existing =
        mem_read_32(addr + (itemp / 4) * 4) & ~(0xff << ((itemp % 4) * 8));
    mem_write_32(addr + ((itemp / 4) * 4),
                 (((CURRENT_STATE.REGS[rt] & 0xff) << ((itemp % 4) * 8))) |
                     temp_existing);
  }
  // AND value in register with ff will chop off the most signficant 24 bits
  // (keeping only the last 8) ORRing the two will keep the first bits from
  // memory and drop in new bits from register
}

void execute_sw() {
  // printf("sw\n");
  uint32_t addr = CURRENT_STATE.REGS[rs];
  if (itemp > 32767) {
    itemp = ((~itemp) & 0xffff) + 1;
    mem_write_32(addr - itemp, CURRENT_STATE.REGS[rt]);
  } else {
    mem_write_32(addr + itemp, CURRENT_STATE.REGS[rt]);
  }
}

void execute_jump() {
  // printf("jump\n");
  // Per the manual, we use the high 4 bits of the pc
  // and the low 26 bits from the instruction, but shift that left 2 so that the
  // lsb won't result in a badly aligned address.
  // Decode the address
  uint32_t addr = ((instruction & 0x3ffffff) << 2);
  addr = addr | (mem_read_32(CURRENT_STATE.PC) & (0xf << 28));
  // Set the PC to the jump address
  NEXT_STATE.PC = addr;
}

void execute_jal() {
  // printf("jal\n");
  // Jump and Link
  uint32_t addr = ((instruction & 0x3ffffff) << 2);
  // We do decoding here to get the address to jump to
  addr += CURRENT_STATE.PC & (0xf << 28);
  // Set the PC to the jump address
  NEXT_STATE.PC = addr;
  // Per this extensive documentation, the register 31 is the hardcoded location
  // of the return address.
  // https://www.cs.cmu.edu/afs/cs/academic/class/15740-f97/public/doc/mips-isa.pdf
  NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
}

void execute_jr() {
  // printf("jr\n");
  // Set the PC to the value provided.
  NEXT_STATE.PC = CURRENT_STATE.REGS[rs];
  // No other action taken.
}

void execute_lui() {
  // Load upper immediate
  // Makes for easier testing since the MIPS compiler uses it a lot
  // 3c 01 10 01
  // 0011 1100 0000 0001 0001 0000 0000 0001
  // ____ __ = 1111 = 15                              OPCODE
  //        __ ___ = 0                                RS
  //              _ ____ = 1                          RT
  //                     ____ ____ ____ ____ = 4097   IMM (itemp)
  // printf("lui\n");
  // Set the upper bits to whatever is specified. Lower bits are always cleared
  // out.
  NEXT_STATE.REGS[rt] = itemp << 16;
}

void execute_ori() {
  // printf("ori\n");
  NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] | itemp;
}

void execute() {
  // printf("\nexecute\t");
  if (op == 0) { // R type instruction
    // printf("R-type\t");
    switch (func) {
    case 8:
      // JR = 8 == return
      execute_jr();
      break;
    case 32: // add:100000
      // ADD = 32
      // printf("add\n");
      //      if(rt>2147483647)
      NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] + CURRENT_STATE.REGS[rt];
      break;
    case 33:
      // ADDU = 33
      // printf("addu\n");
      NEXT_STATE.REGS[rd] = (uint32_t)(CURRENT_STATE.REGS[rs]) +
                            (uint32_t)(CURRENT_STATE.REGS[rt]);
      break;
    case 34:
      // SUB = 34
      // printf("sub\n");
      NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] - CURRENT_STATE.REGS[rt];
      break;
    case 35:
      // SUBU = 35
      // printf("subu\n");
      NEXT_STATE.REGS[rd] = (uint32_t)(CURRENT_STATE.REGS[rs]) -
                            (uint32_t)(CURRENT_STATE.REGS[rt]);
      break;
    case 36:
      // AND = 36
      // printf("and\n");
      NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] & CURRENT_STATE.REGS[rt];
      break;
    case 12:                             // system call:001100
      if (CURRENT_STATE.REGS[2] == 10) { // v0==10 then exit
        printf("systemcall: exit\n");
        RUN_BIT = FALSE;
      } else if (CURRENT_STATE.REGS[2] == 1) { // v0==1: print int
        int32_t val = CURRENT_STATE.REGS[4];   // a0 is 4th register
        printf("\n print the number:   %d \n ", val);
      } else if (CURRENT_STATE.REGS[2] == 34) {
        printf("\n print the number: %x \n", CURRENT_STATE.REGS[4]);
      } else {
        printf("\n syscall with unsupported parameter %d \n",
               CURRENT_STATE.REGS[2]);
      }
      break;
    default:
      break;
    }
  } else {
    if (op == 2 || op == 3) {
      // printf("J-type\t");
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
      // printf("I-type\t");
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
      case 13:
        // ORI = 13
        execute_ori();
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
      default:
        printf(
            "UNSUPPORTED INSTRUCTION ISSUED. PLEASE VERIFY OUTPUT OF MARS.\n");
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
  if (RUN_BIT == 0)
    return;
  decode();
  execute();
}
