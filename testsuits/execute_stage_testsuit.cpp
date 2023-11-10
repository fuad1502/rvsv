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
  int t0 = 0x87654321;
  int pc = 0x87654321;

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
