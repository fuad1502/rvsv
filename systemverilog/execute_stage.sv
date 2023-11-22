`include "opcodes.sv"
`include "function_codes.sv"
`include "operations.sv"

module execute_stage #(
    int XLEN = 32,
    int ILEN = 32
) (
    output logic [XLEN-1:0] valE,
    output logic cond,
    output logic inst_fault,
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

  always_comb begin : calculateValEAndCond
    logic inst_fault_alu = 0;
    logic inst_fault_cond = 0;
    valE = '0;
    cond = 0;
    case (opcode)
      OP:
      valE = alu_op(
        .val1(valA),
        .val2(valB),
        .func3(func3),
        .func7(func7),
        .inst_fault(inst_fault_alu)
      );
      OP_IMM, JALR:
      valE = alu_op_imm(.val1(valA), .val2(valC), .func3(func3), .inst_fault(inst_fault_alu));
      AUIPC, JAL:
      valE = alu_op_imm(.val1(pc), .val2(valC), .func3(ADDI), .inst_fault(inst_fault_alu));
      LOAD, STORE:
      valE = alu_op_imm(.val1(valA), .val2(valC), .func3(ADDI), .inst_fault(inst_fault_alu));
      LUI: valE = valC;
      BRANCH: begin
        valE = alu_op_imm(.val1(pc), .val2(valC), .func3(ADDI), .inst_fault(inst_fault_alu));
        cond = comp(.val1(valA), .val2(valB), .func3(func3), .inst_fault(inst_fault_cond));
      end
      default: begin
        inst_fault_alu  = 1;
        inst_fault_cond = 1;
      end
    endcase
    inst_fault = inst_fault_alu | inst_fault_cond;
  end

  function automatic logic [XLEN-1:0] alu_op(input logic [XLEN-1:0] val1,
                                             input logic [XLEN-1:0] val2, input logic [2:0] func3,
                                             input logic [6:0] func7, output logic inst_fault);
    inst_fault = 0;
    case ({
      func7, func3
    })
      ADD:  return Operations#(.XLEN(XLEN))::add_f(val1, val2);
      AND:  return Operations#(.XLEN(XLEN))::and_f(val1, val2);
      OR:   return Operations#(.XLEN(XLEN))::or_f(val1, val2);
      XOR:  return Operations#(.XLEN(XLEN))::xor_f(val1, val2);
      SLT:  return Operations#(.XLEN(XLEN))::slt_f(val1, val2);
      SLTU: return Operations#(.XLEN(XLEN))::sltu_f(val1, val2);
      SLL:  return Operations#(.XLEN(XLEN))::sll_f(val1, val2);
      SRL:  return Operations#(.XLEN(XLEN))::srl_f(val1, val2);
      SRA:  return Operations#(.XLEN(XLEN))::sra_f(val1, val2);
      SUB:  return Operations#(.XLEN(XLEN))::sub_f(val1, val2);
      default: begin
        inst_fault = 1;
        return '0;
      end
    endcase
  endfunction

  function automatic logic [XLEN-1:0] alu_op_imm(input logic [XLEN-1:0] val1,
                                                 input logic [XLEN-1:0] val2,
                                                 input logic [2:0] func3, output logic inst_fault);
    inst_fault = 0;
    case (func3)
      ADDI:  return Operations#(.XLEN(XLEN))::add_f(val1, val2);
      ANDI:  return Operations#(.XLEN(XLEN))::and_f(val1, val2);
      ORI:   return Operations#(.XLEN(XLEN))::or_f(val1, val2);
      XORI:  return Operations#(.XLEN(XLEN))::xor_f(val1, val2);
      SLTI:  return Operations#(.XLEN(XLEN))::slt_f(val1, val2);
      SLTIU: return Operations#(.XLEN(XLEN))::sltu_f(val1, val2);
      SLLI:
      case (valC[11:5])
        7'b0000000: return Operations#(.XLEN(XLEN))::sll_f(val1, val2);
        default: begin
          inst_fault = 1;
          return '0;
        end
      endcase
      SRLI_SRAI: begin
        case (valC[11:5])
          7'b0000000: return Operations#(.XLEN(XLEN))::srl_f(val1, val2);
          7'b0100000: return Operations#(.XLEN(XLEN))::sra_f(val1, val2);
          default: begin
            inst_fault = 1;
            return '0;
          end
        endcase
      end
      default: begin
        inst_fault = 1;
        return '0;
      end
    endcase
  endfunction

  function automatic logic comp(input logic [XLEN-1:0] val1, input logic [XLEN-1:0] val2,
                                input logic [2:0] func3, output logic inst_fault);
    inst_fault = 0;
    case (func3)
      BEQ:  return val1 == val2;
      BNE:  return val1 != val2;
      BLT:  return signed'(val1) < signed'(val2);
      BLTU: return val1 < val2;
      BGE:  return signed'(val1) >= signed'(val2);
      BGEU: return val1 >= val2;
      default: begin
        inst_fault = 1;
        return 0;
      end
    endcase
  endfunction

endmodule
