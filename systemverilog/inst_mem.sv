module inst_mem #(
    int XLEN = 32,
    int ILEN = 32,
    logic [XLEN-1:0] MEM_SIZE = 1024
) (
    output wire [ILEN-1:0] inst,
    input logic [XLEN-1:0] pc,
    input logic [ILEN-1:0] wdata,
    input logic write_en,
    input logic clock
);

  logic [7:0] mem[MEM_SIZE];

  always_ff @(posedge clock) begin
    if (write_en) begin
      {mem[pc+3], mem[pc+2], mem[pc+1], mem[pc+0]} <= wdata;
    end
  end

  assign inst = {mem[pc+3], mem[pc+2], mem[pc+1], mem[pc+0]};

endmodule
