module reg_file #(
    int XLEN = 32
) (
    output logic [XLEN-1:0] valA,
    output logic [XLEN-1:0] valB,
    input logic [4:0] rs1,
    input logic [4:0] rs2,
    input logic [4:0] rd,
    input logic [XLEN-1:0] wdata,
    input logic write_en,
    input logic clock,
    input logic reset_n
);
  logic [XLEN-1:0] mem[32];

  assign valA = mem[rs1];
  assign valB = mem[rs2];

  always_ff @(posedge clock) begin : writeRegister
    if (!reset_n) begin
      mem <= '{default: 0};
    end else if (write_en) begin
      mem[rd] <= wdata;
    end
  end

endmodule
