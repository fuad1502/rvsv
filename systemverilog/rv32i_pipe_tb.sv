module rv32i_pipe_tb #(
    int XLEN = 32
) ();

  export "DPI-C" function read_mem;
  export "DPI-C" function write_mem;
  export "DPI-C" function read_inst_mem;
  export "DPI-C" function write_inst_mem;
  export "DPI-C" function read_reg_file;
  export "DPI-C" function write_reg_file;
  export "DPI-C" function read_pc;

  logic clock;
  logic reset_n;
  wire  fault  /* verilator public */;

  initial begin
    reset_n = 1;
  end

  always begin
    clock = 0;
    #1;
    clock = 1;
    #1;
  end

  rv32i_pipe inst (.*);

  function automatic int read_mem(int addr, int number_of_bytes);
    read_mem = 0;
    for (int i = 0; i < number_of_bytes; i++) begin
      read_mem += int'(inst.memory_inst.mem[addr+i][7:0]) << (8 * i);
    end
  endfunction

  function automatic int read_inst_mem(int addr, int number_of_bytes);
    read_inst_mem = 0;
    for (int i = 0; i < number_of_bytes; i++) begin
      read_inst_mem += int'(inst.inst_mem_inst.mem[addr+i][7:0]) << (8 * i);
    end
  endfunction

  function automatic int read_reg_file(int n);
    read_reg_file = inst.reg_file_inst.mem[n];
  endfunction

  function automatic int read_pc();
    read_pc = inst.f_pc;
  endfunction

  function automatic write_mem(string file_name);
    $readmemh(file_name, inst.memory_inst.mem);
  endfunction

  function automatic write_inst_mem(string file_name);
    $readmemh(file_name, inst.inst_mem_inst.mem);
  endfunction

  function automatic write_reg_file(int n, int value);
    inst.reg_file_inst.mem[n] = value;
  endfunction

endmodule
