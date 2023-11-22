module memory_tb #(
    int XLEN = 32
) (
    output logic [XLEN-1:0] valM,
    output logic mem_fault,
    input logic [XLEN-1:0] wdata,
    input logic [XLEN-1:0] addr,
    input logic [2:0] width,
    input logic read_en,
    input logic write_en,
    input logic clock
);

  export "DPI-C" function read_mem;
  export "DPI-C" function write_mem;

  memory mem_inst0 (.*);

  function automatic int read_mem(int addr, int number_of_bytes);
    read_mem = 0;
    for (int i = 0; i < number_of_bytes; i++) begin
      read_mem += int'(mem_inst0.mem[addr+i][7:0]) << (8 * i);
    end
  endfunction

  function automatic write_mem(string file_name);
    $readmemh(file_name, mem_inst0.mem);
  endfunction

endmodule
