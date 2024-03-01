`include "opcodes.sv"

module rv32i_pipe #(
    int XLEN = 32,
    int ILEN = 32
) (
    output logic fault,
    input  logic clock,
    input  logic reset_n
);
  import opcodes::*;

  /////////////////////////////////////////////////
  // WRITE BACK STAGE                            //
  /////////////////////////////////////////////////
  // W-pipeline registers
  logic [XLEN-1:0] W_pc;
  logic [6:0] W_opcode;
  logic [6:0] W_func7;
  logic [2:0] W_func3;
  logic [XLEN-1:0] W_valC;
  logic [4:0] W_rs1, W_rs2, W_rd;
  logic W_mem_write_en;
  logic W_mem_read_en;
  logic [1:0] W_mem_width;
  logic W_sign_extend;
  logic W_inst_fault_fetch;
  logic [XLEN-1:0] W_valA, W_valB;
  logic [XLEN-1:0] W_valE;
  logic W_cond;
  logic W_inst_fault_execute;
  logic [XLEN-1:0] W_valM;
  logic W_mem_fault;
  always_ff @(posedge clock) begin : W_stat
    W_pc <= M_pc;
    W_opcode <= M_opcode;
    W_func7 <= M_func7;
    W_func3 <= M_func3;
    W_valC <= M_valC;
    W_rs1 <= M_rs1;
    W_rs2 <= M_rs2;
    W_rd <= M_rd;
    W_mem_write_en <= M_mem_write_en;
    W_mem_read_en <= M_mem_read_en;
    W_mem_width <= M_mem_width;
    W_sign_extend <= M_sign_extend;
    W_inst_fault_fetch <= M_inst_fault_fetch;
    W_valA <= M_valA;
    W_valB <= M_valB;
    W_valE <= M_valE;
    W_cond <= M_cond;
    W_inst_fault_execute <= M_inst_fault_execute;
    W_valM <= m_valM;
    W_mem_fault <= m_mem_fault;
  end
  // Output of write back stage
  logic [XLEN-1:0] w_valR;
  assign fault = W_inst_fault_fetch | W_inst_fault_execute | W_mem_fault;

  always_comb begin : registerWriteBackInput
    case (W_opcode)
      LOAD: w_valR = W_valM;
      JAL, JALR: w_valR = W_valE + 4;
      default: w_valR = W_valE;
    endcase
  end

  /////////////////////////////////////////////////
  // FETCH STAGE                                 //
  /////////////////////////////////////////////////
  // F-pipeline registers
  // Output of fetch stage
  wire [XLEN-1:0] f_pc;
  wire [6:0] f_opcode;
  wire [6:0] f_func7;
  wire [2:0] f_func3;
  wire [XLEN-1:0] f_valC;
  wire [4:0] f_rs1, f_rs2, f_rd;
  wire f_mem_write_en;
  wire f_mem_read_en;
  wire [1:0] f_mem_width;
  wire f_sign_extend;
  wire f_inst_fault_fetch;

  program_counter #(
      .XLEN(XLEN)
  ) program_counter_inst (
      .pc  (f_pc),
      .addr(e_valE),
      .jump(e_cond),
      .clock,
      .reset_n
  );

  wire [ILEN-1:0] inst;
  inst_mem #(
      .XLEN(XLEN),
      .ILEN(ILEN)
  ) inst_mem_inst (
      .inst,
      .pc(f_pc),
      .wdata(),
      .write_en(0),
      .clock
  );

  fetch_stage #(
      .XLEN(XLEN),
      .ILEN(ILEN)
  ) fetch_stage_inst (
      .opcode(f_opcode),
      .func3(f_func3),
      .func7(f_func7),
      .valC(f_valC),
      .rs1(f_rs1),
      .rs2(f_rs2),
      .rd(f_rd),
      .mem_write_en(f_mem_write_en),
      .mem_read_en(f_mem_read_en),
      .mem_width(f_mem_width),
      .sign_extend(f_sign_extend),
      .inst_fault(f_inst_fault_fetch),
      .inst
  );

  /////////////////////////////////////////////////
  // DECODE STAGE                                //
  /////////////////////////////////////////////////
  // D-pipeline registers
  logic [XLEN-1:0] D_pc;
  logic [6:0] D_opcode;
  logic [6:0] D_func7;
  logic [2:0] D_func3;
  logic [XLEN-1:0] D_valC;
  logic [4:0] D_rs1, D_rs2, D_rd;
  logic D_mem_write_en;
  logic D_mem_read_en;
  logic [1:0] D_mem_width;
  logic D_sign_extend;
  logic D_inst_fault_fetch;
  always_ff @(posedge clock) begin : D_stat
    D_pc <= f_pc;
    D_opcode <= f_opcode;
    D_func7 <= f_func7;
    D_func3 <= f_func3;
    D_valC <= f_valC;
    D_rs1 <= f_rs1;
    D_rs2 <= f_rs2;
    D_rd <= f_rd;
    D_mem_write_en <= f_mem_write_en;
    D_mem_read_en <= f_mem_read_en;
    D_mem_width <= f_mem_width;
    D_sign_extend <= f_sign_extend;
    D_inst_fault_fetch <= f_inst_fault_fetch;
  end
  // Output of fetch stage
  wire [XLEN-1:0] d_valA, d_valB;

  reg_file #(
      .XLEN(XLEN)
  ) reg_file_inst (
      .valA(d_valA),
      .valB(d_valB),
      .rs1(D_rs1),
      .rs2(D_rs2),
      .rd(W_rd),
      .wdata(w_valR),
      .write_en(1),
      .clock
  );

  /////////////////////////////////////////////////
  // EXECUTE STAGE                               //
  /////////////////////////////////////////////////
  // E-pipeline registers
  logic [XLEN-1:0] E_pc;
  logic [6:0] E_opcode;
  logic [6:0] E_func7;
  logic [2:0] E_func3;
  logic [XLEN-1:0] E_valC;
  logic [4:0] E_rs1, E_rs2, E_rd;
  logic E_mem_write_en;
  logic E_mem_read_en;
  logic [1:0] E_mem_width;
  logic E_sign_extend;
  logic E_inst_fault_fetch;
  logic [XLEN-1:0] E_valA, E_valB;
  always_ff @(posedge clock) begin : E_stat
    E_pc <= D_pc;
    E_opcode <= D_opcode;
    E_func7 <= D_func7;
    E_func3 <= D_func3;
    E_valC <= D_valC;
    E_rs1 <= D_rs1;
    E_rs2 <= D_rs2;
    E_rd <= D_rd;
    E_mem_write_en <= D_mem_write_en;
    E_mem_read_en <= D_mem_read_en;
    E_mem_width <= D_mem_width;
    E_sign_extend <= D_sign_extend;
    E_inst_fault_fetch <= D_inst_fault_fetch;
    E_valA <= d_valA;
    E_valB <= d_valB;
  end
  // Output of execute stage
  wire [XLEN-1:0] e_valE;
  wire e_cond;
  wire e_inst_fault_execute;

  execute_stage #(
      .XLEN(XLEN),
      .ILEN(ILEN)
  ) execute_stage_inst (
      .valE(e_valE),
      .cond(e_cond),
      .inst_fault(e_inst_fault_execute),
      .opcode(E_opcode),
      .func3(E_func3),
      .func7(E_func7),
      .valA(E_valA),
      .valB(E_valB),
      .valC(E_valC),
      .pc(E_pc)
  );

  /////////////////////////////////////////////////
  // MEMORY STAGE                                //
  /////////////////////////////////////////////////
  // M-pipeline registers
  logic [XLEN-1:0] M_pc;
  logic [6:0] M_opcode;
  logic [6:0] M_func7;
  logic [2:0] M_func3;
  logic [XLEN-1:0] M_valC;
  logic [4:0] M_rs1, M_rs2, M_rd;
  logic M_mem_write_en;
  logic M_mem_read_en;
  logic [1:0] M_mem_width;
  logic M_sign_extend;
  logic M_inst_fault_fetch;
  logic [XLEN-1:0] M_valA, M_valB;
  logic [XLEN-1:0] M_valE;
  logic M_cond;
  logic M_inst_fault_execute;
  always_ff @(posedge clock) begin : M_stat
    M_pc <= E_pc;
    M_opcode <= E_opcode;
    M_func7 <= E_func7;
    M_func3 <= E_func3;
    M_valC <= E_valC;
    M_rs1 <= E_rs1;
    M_rs2 <= E_rs2;
    M_rd <= E_rd;
    M_mem_write_en <= E_mem_write_en;
    M_mem_read_en <= E_mem_read_en;
    M_mem_width <= E_mem_width;
    M_sign_extend <= E_sign_extend;
    M_inst_fault_fetch <= E_inst_fault_fetch;
    M_valA <= E_valA;
    M_valB <= E_valB;
    M_valE <= e_valE;
    M_cond <= e_cond;
    M_inst_fault_execute <= e_inst_fault_execute;
  end
  // Output of memory stage
  wire [XLEN-1:0] m_valM;
  wire m_mem_fault;

  memory #(
      .XLEN(XLEN)
  ) memory_inst (
      .valM(m_valM),
      .mem_fault(m_mem_fault),
      .addr(M_valE),
      .wdata(M_valB),
      .width(M_mem_width),
      .sign_extend(M_sign_extend),
      .read_en(M_mem_read_en),
      .write_en(M_mem_write_en),
      .clock
  );

endmodule
