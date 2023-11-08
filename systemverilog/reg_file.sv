module reg_file #(
    int XLEN = 32
) (
    output logic [XLEN-1:0] val1,
    output logic [XLEN-1:0] val2,
    input logic [4:0] rs1,
    input logic [4:0] rs2,
    input logic [4:0] rd,
    input logic [XLEN-1:0] valR,
    input logic write_en,
    input logic clock,
    input logic reset_n
);
  logic [XLEN-1:0] mem[32];

  assign val1 = mem[rs1];
  assign val2 = mem[rs2];

  always_ff @(posedge clock) begin : writeRegister
    if (!reset_n) begin
      mem <= '{default: 0};
    end else if (write_en) begin
      mem[rd] <= valR;
    end
  end

endmodule
