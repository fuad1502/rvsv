module memory #(
    logic [31:0] MEM_SIZE = 1024
) (
    output logic [XLEN-1:0] valM,
    output logic mem_fault,
    input logic [XLEN-1:0] addr,
    input logic [XLEN-1:0] wdata,
    input logic [2:0] width,
    input logic write_en,
    input logic clock
);

  logic [7:0] mem[MEM_SIZE];

  always_comb begin : setMemFault
    if (addr >= MEM_SIZE || width > 3'b010) begin
      mem_fault = 1;
    end
  end

  always_comb begin : readMem
    if (addr >= MEM_SIZE) begin
      valM = '0;
    end else begin
      valM = read_mem(.mem(mem), .addr(addr), .width(width));
    end
  end

  always_ff @(posedge clk) begin : writeMem
    if (write_en && addr < MEM_SIZE) begin
      write_mem(.mem(mem), .addr(addr), .width(width));
    end
  end

  function automatic logic [XLEN-1:0] read_mem(
      input logic [7:0] mem[MEM_SIZE], input logic [XLEN-1:0] addr, input logic [2:0] width);
    case (width)
      3'b000:  return {8'b0, 8'b0, 8'b0, mem[addr]};
      3'b001:  return {8'b0, 8'b0, mem[addr+1], mem[addr]};
      3'b010:  return {mem[addr+3], mem[addr+2], mem[addr+1], mem[addr]};
      default: return '0;
    endcase
  endfunction

  function automatic void write_mem(output logic [7:0] mem[MEM_SIZE], input logic [XLEN-1:0] addr,
                                    input logic [2:0] width);
    case (width)
      3'b000: begin
        mem[addr] <= wdata[7:0];
      end
      3'b001: begin
        mem[addr]   <= wdata[7:0];
        mem[addr+1] <= wdata[15:8];
      end
      3'b001: begin
        mem[addr]   <= wdata[7:0];
        mem[addr+1] <= wdata[15:8];
        mem[addr+2] <= wdata[23:16];
        mem[addr+3] <= wdata[31:24];
      end
      default: ;
    endcase
  endfunction


endmodule
