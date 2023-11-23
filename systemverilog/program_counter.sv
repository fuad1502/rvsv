module program_counter #(
    int XLEN = 32
) (
    output logic [XLEN-1:0] pc,
    input logic [XLEN-1:0] addr,
    input logic jump,
    input logic clock,
    input logic reset_n
);

  logic [XLEN-1:0] next_pc;

  always_comb begin : calculateNextPC
    if (jump) begin
      next_pc = addr;
    end else begin
      next_pc = pc + 4;
    end
  end

  always_ff @(posedge clock) begin : pcUpdate
    if (!reset_n) begin
      pc <= '0;
    end else begin
      pc <= {next_pc[XLEN-1:1], 1'b0};
    end
  end

endmodule
