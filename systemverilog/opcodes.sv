`ifndef OPCODES_PKG
`define OPCODES_PKG
`include "riscv_instr.sv"
package opcodes;
  typedef enum logic [6:0] {
    OP = riscv_instr::ADD[6:0],
    OP_IMM = riscv_instr::ADDI[6:0],
    LUI = riscv_instr::LUI[6:0],
    AUIPC = riscv_instr::AUIPC[6:0],
    JAL = riscv_instr::JAL[6:0],
    JALR = riscv_instr::JALR[6:0],
    BRANCH = riscv_instr::BEQ[6:0],
    STORE = riscv_instr::SB[6:0],
    LOAD = riscv_instr::LB[6:0]
  } opcode_t;
endpackage
`endif
