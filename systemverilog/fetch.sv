`include "opcodes.sv"

module fetch #(
    int XLEN = 32,
    int ILEN = 32
) (
    output wire [6:0] opcode,
    output wire [9:0] func,
    output logic [XLEN-1:0] valC,
    output logic [4:0] rs1,
    output logic [4:0] rs2,
    output logic [4:0] rd,
    input logic [XLEN-1:0] pc,
    input logic [ILEN-1:0] wdata,
    input logic write_en,
    input logic clock,
    input logic reset_n
);

  import opcodes::*;

  logic [ILEN-1:0] inst;

  inst_mem inst_mem (.*);

  assign opcode = inst[6:0];
  assign func   = {inst[31:25], inst[14:12]};

  always_comb begin : setSourceRegister
    case (opcode)
      OP, BRANCH, STORE: begin
        rs1 = inst[19:15];
        rs2 = inst[24:20];
      end
      OP_IMM, JALR, LOAD: begin
        rs1 = inst[19:15];
        rs2 = '0;
      end
      default: begin
        rs1 = '0;
        rs2 = '0;
      end
    endcase
  end

  always_comb begin : setDestinationRegister
    case (opcode)
      OP, OP_IMM, LUI, AUIPC, JAL, JALR, LOAD: rd = inst[11:7];
      default: rd = 0;
    endcase
  end

  always_comb begin : setImmediateValue
    case (opcode)
      OP_IMM, JALR: begin
        valC = {{20{inst[31]}}, inst[31:20]};
      end
      LUI, AUIPC: begin
        valC = {inst[31:12], {12{1'b0}}};
      end
      JAL: begin
        valC = {{12{inst[31]}}, inst[19:12], inst[20], inst[30:21], 1'b0};
      end
      BRANCH: begin
        valC = {{20{inst[31]}}, inst[7], inst[30:25], inst[11:8], 1'b0};
      end
      default: valC = '0;
    endcase
  end

endmodule
