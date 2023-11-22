module execute_stage_tb #(
    int XLEN = 32,
    int ILEN = 32
) (
    output logic [XLEN-1:0] valE,
    output logic cond,
    output logic inst_fault,
    input logic [ILEN-1:0] inst,
    input logic [XLEN-1:0] valA,
    valB,
    pc
);

  wire [6:0] opcode;
  wire [2:0] func3;
  wire [6:0] func7;
  wire [XLEN-1:0] valC;
  wire inst_fault_fetch;
  wire inst_fault_execute;

  assign inst_fault = inst_fault_fetch | inst_fault_execute;

  fetch_stage fetch_stage_inst (
      .rs1(),
      .rs2(),
      .rd(),
      .mem_write_en(),
      .mem_read_en(),
      .mem_width(),
      .sign_extend(),
      .inst_fault(inst_fault_fetch),
      .*
  );

  execute_stage execute_stage_inst (
      .inst_fault(inst_fault_execute),
      .*
  );

endmodule
