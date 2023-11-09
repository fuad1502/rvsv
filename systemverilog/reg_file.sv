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
    input logic clock
);
  logic [XLEN-1:0] mem[1:32];

  assign valA = (rs1 == 5'b00000)? 0 : mem[rs1];
  assign valB = (rs2 == 5'b00000)? 0 : mem[rs2];

  always_ff @(posedge clock) begin : writeRegister
    if (write_en && rd != 5'b00000) begin
      mem[rd] <= wdata;
    end
  end

endmodule
