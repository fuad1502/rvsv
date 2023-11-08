`include "opcodes.sv"
`include "function_codes.sv"

module execute_stage #(
    int XLEN = 32,
    int ILEN = 32
) (
    output logic [XLEN-1:0] valE,
    output logic cond,
    input logic [6:0] opcode,
    input logic [9:0] func,
    input logic [XLEN-1:0] valA,
    input logic [XLEN-1:0] valB,
    input logic [XLEN-1:0] valC,
    input logic [ILEN-1:0] pc
);
  import opcodes::*;
  import function_codes::*;

  always_comb begin : calculateValE
    case (opcode)
      OP_IMM, JALR: valE = alu(.val1(valA), .val2(valC), .func(func));
      OP: valE = alu(.val1(valA), .val2(valB), .func(func));
      AUIPC, JAL, BRANCH: valE = alu(.val1(pc), .val2(valC), .func(ADD));
      LOAD, STORE: valE = alu(.val1(valA), .val2(valC), .func(ADD));
      LUI: valE = valC;
      default: valE = '0;
    endcase
  end

  always_comb begin : calculateCond
    case (opcode)
      BRANCH:  cond = comp(.val1(valA), .val2(valB), .func3(func[2:0]));
      default: cond = 1'b0;
    endcase
  end

  function automatic logic [XLEN-1:0] alu(input logic [XLEN-1:0] val1, input logic [XLEN-1:0] val2,
                                          input logic [9:0] func);
    case (func)
      ADD: return val1 + val2;
      AND: return val1 & val2;
      OR: return val1 | val2;
      XOR: return val1 ^ val2;
      SLT: return {{(XLEN - 1) {1'b0}}, signed'(val1) < signed'(val2)};
      SLTU: return {{(XLEN - 1) {1'b0}}, val1 < val2};
      SLL: return val1 << val2[4:0];
      SRL: return val1 >> val2[4:0];
      SRA: return signed'(val1) >>> signed'(val2[4:0]);
      SUB: return val1 - val2;
      default: return '0;
    endcase
  endfunction

  function automatic logic comp(input logic [XLEN-1] val1, input logic [XLEN-1] val2,
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
