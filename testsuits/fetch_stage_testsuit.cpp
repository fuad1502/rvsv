#include "rubbler.h"
#include <Vfetch_stage.h>
#include <cstdint>
#include <memory>
#include <verilated.h>

using namespace std;

void write_asm_line_inst(unique_ptr<Vfetch_stage> &fetch,
                         unique_ptr<VerilatedContext> &context,
                         const char *asm_line) {
  uint32_t inst;
  auto ok = rubble_line(asm_line, &inst);
  assert(ok);
  fetch->inst = inst;
  fetch->eval();
}

IData ui(int imm) { return imm & 0xFFFFF000; }

int main(int argc, char *argv[]) {
  unique_ptr<VerilatedContext> context(new VerilatedContext());
  unique_ptr<Vfetch_stage> fetch(new Vfetch_stage(context.get(), "fetch"));

  // Test OP-IMM instruction
  auto asm_line = "addi t2, t1, -3";
  write_asm_line_inst(fetch, context, asm_line);
  assert(fetch->rd == 7);
  assert(fetch->rs1 == 6);
  assert(fetch->rs2 == 0);
  assert(fetch->valC == -3);
  assert(fetch->mem_write_en == 0);
  assert(fetch->mem_read_en == 0);

  // Test U instruction
  asm_line = "lui t2, -3";
  write_asm_line_inst(fetch, context, asm_line);
  assert(fetch->rd == 7);
  assert(fetch->rs1 == 0);
  assert(fetch->rs2 == 0);
  assert(fetch->valC == ui(-3));
  assert(fetch->mem_write_en == 0);
  assert(fetch->mem_read_en == 0);

  // Test OP instruction
  asm_line = "add t2, t1, t0";
  write_asm_line_inst(fetch, context, asm_line);
  assert(fetch->rd == 7);
  assert(fetch->rs1 == 6);
  assert(fetch->rs2 == 5);
  assert(fetch->valC == 0);
  assert(fetch->mem_write_en == 0);
  assert(fetch->mem_read_en == 0);

  // Test JAL instruction
  asm_line = "jal t2, -3";
  write_asm_line_inst(fetch, context, asm_line);
  assert(fetch->rd == 7);
  assert(fetch->rs1 == 0);
  assert(fetch->rs2 == 0);
  assert(fetch->valC == (-3 & 0xFFFFFFFE));
  assert(fetch->mem_write_en == 0);
  assert(fetch->mem_read_en == 0);

  // Test JALR instruction
  asm_line = "jalr t2, t1, -3";
  write_asm_line_inst(fetch, context, asm_line);
  assert(fetch->rd == 7);
  assert(fetch->rs1 == 6);
  assert(fetch->rs2 == 0);
  assert(fetch->valC == -3);
  assert(fetch->mem_write_en == 0);
  assert(fetch->mem_read_en == 0);

  // Test B instruction
  asm_line = "beq t2, t1, -3";
  write_asm_line_inst(fetch, context, asm_line);
  assert(fetch->rd == 0);
  assert(fetch->rs1 == 7);
  assert(fetch->rs2 == 6);
  assert(fetch->valC == (-3 & 0xFFFFFFFE));
  assert(fetch->mem_write_en == 0);
  assert(fetch->mem_read_en == 0);

  // Test load instruction
  asm_line = "lh t2, -3(t1)";
  write_asm_line_inst(fetch, context, asm_line);
  assert(fetch->rd == 7);
  assert(fetch->rs1 == 6);
  assert(fetch->rs2 == 0);
  assert(fetch->valC == -3);
  assert(fetch->mem_write_en == 0);
  assert(fetch->mem_read_en == 1);
  assert(fetch->mem_width == 1);
  assert(fetch->sign_extend == 1);
  asm_line = "lhu t2, -3(t1)";
  write_asm_line_inst(fetch, context, asm_line);
  assert(fetch->sign_extend == 0);

  // Test store instruction
  asm_line = "sh t2, -3(t1)";
  write_asm_line_inst(fetch, context, asm_line);
  assert(fetch->rd == 0);
  assert(fetch->rs1 == 6);
  assert(fetch->rs2 == 7);
  assert(fetch->valC == -3);
  assert(fetch->mem_write_en == 1);
  assert(fetch->mem_read_en == 0);
  assert(fetch->mem_width == 1);

  return 0;
}
