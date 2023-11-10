module execute_stage_tb #(
    int XLEN = 32,
    int ILEN = 32
) (
    output logic [XLEN-1:0] valE,
    output logic cond,
    input logic [ILEN-1:0] inst,
    input logic [XLEN-1:0] valA,
    valB,
    pc
);

  wire [6:0] opcode;
  wire [2:0] func3;
  wire [6:0] func7;
  wire [XLEN-1:0] valC;

  fetch_stage fetch_stage_inst (
      .rs1(),
      .rs2(),
      .rd(),
      .mem_write_en(),
      .mem_read_en(),
      .mem_width(),
      .*
  );
  execute_stage execute_stage_inst (.*);

endmodule
