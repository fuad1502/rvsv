#include "Vexecute_stage_tb.h"
#include "rubbler.h"
#include "verilated.h"
#include <cassert>
#include <cstdio>
#include <memory>

using namespace std;

void evaluate(unique_ptr<Vexecute_stage_tb> &execute_stage,
              const char *asm_line, int valA, int valB, int pc);

int main(int argc, char *argv[]) {
  unique_ptr<VerilatedContext> context(new VerilatedContext());
  unique_ptr<Vexecute_stage_tb> execute_stage(
      new Vexecute_stage_tb(context.get(), "EXECUTE_STAGE"));

  int t1 = 0x87654321;
  int t0 = -3;
  int pc = 0x12345678;

  ////////////////////////// TEST OP-IMM //////////////////////////
  // Test ADDI
  auto asm_line = "addi t2 t1 -3";
  evaluate(execute_stage, asm_line, t1, t0, pc);
  auto expected_valE = t1 - 3;
  assert(execute_stage->valE == expected_valE);
  // Test ANDI
  asm_line = "andi t2 t1 -3";
  evaluate(execute_stage, asm_line, t1, t0, pc);
  expected_valE = t1 & -3;
  assert(execute_stage->valE == expected_valE);
  // Test ORI
  asm_line = "ori t2 t1 -3";
  evaluate(execute_stage, asm_line, t1, t0, pc);
  expected_valE = t1 | -3;
  assert(execute_stage->valE == expected_valE);
  // Test XORI
  asm_line = "xori t2 t1 -3";
  evaluate(execute_stage, asm_line, t1, t0, pc);
  expected_valE = t1 ^ -3;
  assert(execute_stage->valE == expected_valE);
  // Test SLTI
  asm_line = "slti t2 t1 -3";
  evaluate(execute_stage, asm_line, t1, t0, pc);
  expected_valE = 1;
  assert(execute_stage->valE == expected_valE);
  // Test SLTIU
  asm_line = "sltiu t2 t1 -3";
  evaluate(execute_stage, asm_line, t1, t0, pc);
  expected_valE = 1;
  assert(execute_stage->valE == expected_valE);
  // Test SLLI
  asm_line = "slli t2 t1 4";
  evaluate(execute_stage, asm_line, t1, t0, pc);
  expected_valE = t1 << 4;
  assert(execute_stage->valE == expected_valE);
  // Test SRLI
  asm_line = "srli t2 t1 4";
  evaluate(execute_stage, asm_line, t1, t0, pc);
  expected_valE = (unsigned int)t1 >> 4;
  assert(execute_stage->valE == expected_valE);
  // Test SRAI
  asm_line = "srai t2 t1 4";
  evaluate(execute_stage, asm_line, t1, t0, pc);
  expected_valE = t1 >> 4;
  assert(execute_stage->valE == expected_valE);

  ////////////////////////// TEST OP //////////////////////////
  // Test ADD
  asm_line = "add t2 t1 t0";
  evaluate(execute_stage, asm_line, t1, t0, pc);
  expected_valE = t1 + t0;
  assert(execute_stage->valE == expected_valE);
  // Test AND
  asm_line = "and t2 t1 t0";
  evaluate(execute_stage, asm_line, t1, t0, pc);
  expected_valE = t1 & t0;
  assert(execute_stage->valE == expected_valE);
  // Test OR
  asm_line = "or t2 t1 t0";
  evaluate(execute_stage, asm_line, t1, t0, pc);
  expected_valE = t1 | t0;
  assert(execute_stage->valE == expected_valE);
  // Test XOR
  asm_line = "xor t2 t1 t0";
  evaluate(execute_stage, asm_line, t1, t0, pc);
  expected_valE = t1 ^ t0;
  assert(execute_stage->valE == expected_valE);
  // Test SLT
  asm_line = "slt t2 t1 t0";
  evaluate(execute_stage, asm_line, t1, t0, pc);
  expected_valE = 1;
  assert(execute_stage->valE == expected_valE);
  // Test SLTU
  asm_line = "sltu t2 t1 t0";
  evaluate(execute_stage, asm_line, t1, t0, pc);
  expected_valE = 1;
  assert(execute_stage->valE == expected_valE);
  // Test SLL
  t0 = 4;
  asm_line = "sll t2 t1 t0";
  evaluate(execute_stage, asm_line, t1, t0, pc);
  expected_valE = t1 << t0;
  assert(execute_stage->valE == expected_valE);
  // Test SRL
  asm_line = "srl t2 t1 t0";
  evaluate(execute_stage, asm_line, t1, t0, pc);
  expected_valE = (unsigned int)t1 >> t0;
  assert(execute_stage->valE == expected_valE);
  // Test SRA
  asm_line = "sra t2 t1 t0";
  evaluate(execute_stage, asm_line, t1, t0, pc);
  expected_valE = t1 >> t0;
  assert(execute_stage->valE == expected_valE);
  // Test SUB
  asm_line = "sub t2 t1 t0";
  evaluate(execute_stage, asm_line, t1, t0, pc);
  expected_valE = t1 - t0;
  assert(execute_stage->valE == expected_valE);

  ////////////////////////// TEST LUI & AUIPC //////////////////////////
  // Test LUI
  asm_line = "lui t2 0x12345678";
  evaluate(execute_stage, asm_line, t1, t0, pc);
  expected_valE = 0x12345678 & 0xFFFFF000;
  assert(execute_stage->valE == expected_valE);
  // Test AUIPC
  asm_line = "auipc t1 0x12345678";
  evaluate(execute_stage, asm_line, t1, t0, pc);
  expected_valE = 0x12345000 + pc;
  printf("result = %08x\n", execute_stage->valE);
  assert(execute_stage->valE == expected_valE);

  return 0;
}

void evaluate(unique_ptr<Vexecute_stage_tb> &execute_stage,
              const char *asm_line, int valA, int valB, int pc) {
  int inst = decode_asm_line_ffi(asm_line);
  printf("%032b\n", inst);
  execute_stage->inst = inst;
  execute_stage->valA = valA;
  execute_stage->valB = valB;
  execute_stage->pc = pc;
  execute_stage->eval();
}
