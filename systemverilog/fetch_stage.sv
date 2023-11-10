`include "opcodes.sv"

module fetch_stage #(
    int XLEN = 32,
    int ILEN = 32
) (
    output wire [6:0] opcode,
    output wire [2:0] func3,
    output wire [6:0] func7,
    output logic [XLEN-1:0] valC,
    output logic [4:0] rs1,
    output logic [4:0] rs2,
    output logic [4:0] rd,
    output logic mem_write_en,
    output logic mem_read_en,
    output logic [2:0] mem_width,
    input logic [XLEN-1:0] inst
);

  import opcodes::*;

  assign opcode = inst[6:0];
  assign func3  = inst[14:12];
  assign func7  = inst[31:25];

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
      OP_IMM, JALR, LOAD: begin
        valC = {{21{inst[31]}}, inst[30:20]};
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
      STORE: begin
        valC = {{21{inst[31]}}, inst[30:25], inst[11:8], inst[7]};
      end
      default: valC = '0;
    endcase
  end

  assign mem_write_en = opcode == STORE;
  assign mem_read_en = opcode == LOAD;
  assign mem_width = inst[14:12];

endmodule
