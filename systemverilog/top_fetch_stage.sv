module top_fetch_stage #(
    int XLEN = 32,
    int ILEN = 32
) (
    output logic [6:0] opcode,
    output logic [9:0] func,
    output logic [XLEN-1:0] valC,
    output logic [4:0] rs1,
    output logic [4:0] rs2,
    output logic [4:0] rd,
    input logic [ILEN-1:0] pc,
    input logic [ILEN-1:0] wdata,
    input logic write_en,
    input logic clock,
    input logic reset_n
);

  logic [ILEN-1:0] inst;

  inst_mem inst_mem (.*);
  fetch_stage fetch_stage (.*);

endmodule
