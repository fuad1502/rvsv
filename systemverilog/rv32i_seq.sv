`include "opcodes.sv"

module rv32i_seq #(
    int XLEN = 32,
    int ILEN = 32
) (
    output logic inst_fault,
    input  logic clock,
    input  logic reset_n
);
  import opcodes::*;
  // Output of program counter
  wire [XLEN-1:0] pc;
  // Output of instruction memory
  wire [ILEN-1:0] inst;
  // Output of fetch stage
  wire [6:0] opcode;
  wire [6:0] func7;
  wire [2:0] func3;
  wire [XLEN-1:0] valC;
  wire [4:0] rs1, rs2, rd;
  wire mem_write_en;
  wire mem_read_en;
  wire [1:0] mem_width;
  wire sign_extend;
  wire inst_fault_inst;
  // Output of register file
  wire [XLEN-1:0] valA, valB;
  // Output of execute stage
  wire [XLEN-1:0] valE;
  wire cond;
  wire inst_fault_execute;
  // Output of memory stage
  wire [XLEN-1:0] valM;
  wire mem_fault;
  // Input to register write back
  logic [XLEN-1:0] valR;

  assign inst_fault = inst_fault_inst | inst_fault_execute;

  always_comb begin : registerWriteBackInput
    case (opcode)
      LOAD: valR = valM;
      JAL, JALR: valR = valE + 4;
      default: valR = valE;
    endcase
  end

  program_counter #(
      .XLEN(XLEN)
  ) program_counter_inst (
      .addr(valE),
      .jump(cond),
      .*
  );

  inst_mem #(
      .XLEN(XLEN),
      .ILEN(ILEN)
  ) inst_mem_inst (
      .wdata(),
      .write_en(0),
      .*
  );

  fetch_stage #(
      .XLEN(XLEN),
      .ILEN(ILEN)
  ) fetch_stage_inst (
      .*
  );

  reg_file #(
      .XLEN(XLEN)
  ) reg_file_inst (
      .wdata(valR),
      .write_en(1),
      .*
  );

  execute_stage #(
      .XLEN(XLEN),
      .ILEN(ILEN)
  ) execute_stage_inst (
      .*
  );

  memory #(
      .XLEN(XLEN)
  ) memory_inst (
      .addr(valE),
      .width(mem_width),
      .wdata(valB),
      .read_en(mem_read_en),
      .write_en(mem_write_en),
      .*
  );

endmodule
