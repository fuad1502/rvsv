`include "opcodes.sv"
`include "function_codes.sv"
`include "operations.sv"

module execute_stage #(
    int XLEN = 32,
    int ILEN = 32
) (
    output logic [XLEN-1:0] valE,
    output logic cond,
    input logic [6:0] opcode,
    input logic [2:0] func3,
    input logic [6:0] func7,
    input logic [XLEN-1:0] valA,
    input logic [XLEN-1:0] valB,
    input logic [XLEN-1:0] valC,
    input logic [ILEN-1:0] pc
);
  import opcodes::*;
  import function_codes::*;
  import operations::*;

  always_comb begin : calculateValE
    case (opcode)
      OP: valE = alu_op(.val1(valA), .val2(valB), .func3(func3), .func7(func7));
      OP_IMM, JALR: valE = alu_op_imm(.val1(valA), .val2(valC), .func3(func3));
      AUIPC, JAL, BRANCH: valE = alu_op_imm(.val1(pc), .val2(valC), .func3(ADDI));
      LOAD, STORE: valE = alu_op_imm(.val1(valA), .val2(valC), .func3(ADDI));
      LUI: valE = valC;
      default: valE = '0;
    endcase
  end

  always_comb begin : calculateCond
    case (opcode)
      BRANCH:  cond = comp(.val1(valA), .val2(valB), .func3(func3));
      default: cond = 1'b0;
    endcase
  end

  function automatic logic [XLEN-1:0] alu_op(input logic [XLEN-1:0] val1,
                                             input logic [XLEN-1:0] val2, input logic [2:0] func3,
                                             input logic [6:0] func7);
    case ({
      func7, func3
    })
      ADD: return Operations#(.XLEN(XLEN))::add_f(val1, val2);
      AND: return Operations#(.XLEN(XLEN))::and_f(val1, val2);
      OR: return Operations#(.XLEN(XLEN))::or_f(val1, val2);
      XOR: return Operations#(.XLEN(XLEN))::xor_f(val1, val2);
      SLT: return Operations#(.XLEN(XLEN))::slt_f(val1, val2);
      SLTU: return Operations#(.XLEN(XLEN))::sltu_f(val1, val2);
      SLL: return Operations#(.XLEN(XLEN))::sll_f(val1, val2);
      SRL: return Operations#(.XLEN(XLEN))::srl_f(val1, val2);
      SRA: return Operations#(.XLEN(XLEN))::sra_f(val1, val2);
      SUB: return Operations#(.XLEN(XLEN))::sub_f(val1, val2);
      default: return '0;
    endcase
  endfunction

  function automatic logic [XLEN-1:0] alu_op_imm(
      input logic [XLEN-1:0] val1, input logic [XLEN-1:0] val2, input logic [2:0] func3);
    case (func3)
      ADDI: return Operations#(.XLEN(XLEN))::add_f(val1, val2);
      ANDI: return Operations#(.XLEN(XLEN))::and_f(val1, val2);
      ORI: return Operations#(.XLEN(XLEN))::or_f(val1, val2);
      XORI: return Operations#(.XLEN(XLEN))::xor_f(val1, val2);
      SLTI: return Operations#(.XLEN(XLEN))::slt_f(val1, val2);
      SLTIU: return Operations#(.XLEN(XLEN))::sltu_f(val1, val2);
      SLLI:
      case (valC[31:25])
        7'b0000000: return Operations#(.XLEN(XLEN))::sll_f(val1, val2);
        default: return '0;
      endcase
      SRLI_SRAI:
      case (valC[31:25])
        7'b0000000: return Operations#(.XLEN(XLEN))::srl_f(val1, val2);
        7'b0100000: return Operations#(.XLEN(XLEN))::sra_f(val1, val2);
        default: return '0;
      endcase
      default: return '0;
    endcase
  endfunction

  function automatic logic comp(input logic [XLEN-1:0] val1, input logic [XLEN-1:0] val2,
                                input logic [2:0] func3);
    case (func3)
      BEQ: return val1 == val2;
      BNE: return val1 != val2;
      BLT: return signed'(val1) < signed'(val2);
      BLTU: return val1 < val2;
      BGE: return signed'(val1) >= signed'(val2);
      BGEU: return val1 >= val2;
      default: return 1'b0;
    endcase
  endfunction

endmodule
