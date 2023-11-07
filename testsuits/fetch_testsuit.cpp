#include "rubbler.h"
#include <Vfetch.h>
#include <memory>
#include <verilated.h>

using namespace std;

struct inst_fetch {
  CData opcode;
  CData rs1;
  CData rs2;
  CData rd;
  IData valC;
};

inst_fetch read_fetch(unique_ptr<Vfetch> &fetch,
                      unique_ptr<VerilatedContext> &context, int pc);
void write_fetch(unique_ptr<Vfetch> &fetch,
                 unique_ptr<VerilatedContext> &context, int pc, int wdata);
void reset_fetch(unique_ptr<Vfetch> &fetch,
                 unique_ptr<VerilatedContext> &context);
void clock_fetch(unique_ptr<Vfetch> &fetch,
                 unique_ptr<VerilatedContext> &context);

inst_fetch write_and_read_fetch(unique_ptr<Vfetch> &fetch,
                                unique_ptr<VerilatedContext> &context,
                                const char *asm_line) {
  IData inst = decode_asm_line_ffi(asm_line);
  write_fetch(fetch, context, 0x0, inst);
  return read_fetch(fetch, context, 0x0);
}

IData ui(int imm) {
  return imm & 0xFFFFF000;
}

int main(int argc, char *argv[]) {
  unique_ptr<VerilatedContext> context(new VerilatedContext());
  unique_ptr<Vfetch> fetch(new Vfetch(context.get(), "fetch"));

  reset_fetch(fetch, context);

  // Test OP-IMM instruction
  auto asm_line = "addi t2 t1 -3";
  auto res = write_and_read_fetch(fetch, context, asm_line);
  assert(res.rd == 7);
  assert(res.rs1 == 6);
  assert(res.rs2 == 0);
  assert(res.valC == -3);

  // Test U instruction
  asm_line = "lui t2 -3";
  res = write_and_read_fetch(fetch, context, asm_line);
  assert(res.rd == 7);
  assert(res.rs1 == 0);
  assert(res.rs2 == 0);
  assert(res.valC == ui(-3));

  // Test OP instruction
  asm_line = "add t2 t1 t0";
  res = write_and_read_fetch(fetch, context, asm_line);
  assert(res.rd == 7);
  assert(res.rs1 == 6);
  assert(res.rs2 == 5);
  assert(res.valC == 0);

  // Test JAL instruction
  asm_line = "jal t2 -3";
  res = write_and_read_fetch(fetch, context, asm_line);
  assert(res.rd == 7);
  assert(res.rs1 == 0);
  assert(res.rs2 == 0);
  assert(res.valC == (-3 & 0xFFFFFFFE));

  // Test JALR instruction
  asm_line = "jalr t2 t1 -3";
  res = write_and_read_fetch(fetch, context, asm_line);
  assert(res.rd == 7);
  assert(res.rs1 == 6);
  assert(res.rs2 == 0);
  assert(res.valC == -3);

  // Test B instruction
  asm_line = "beq t2 t1 -3";
  res = write_and_read_fetch(fetch, context, asm_line);
  assert(res.rd == 0);
  assert(res.rs1 == 7);
  assert(res.rs2 == 6);
  assert(res.valC == (-3 & 0xFFFFFFFE));

  return 0;
}

inst_fetch read_fetch(unique_ptr<Vfetch> &fetch,
                      unique_ptr<VerilatedContext> &context, int pc) {
  fetch->pc = pc;
  fetch->eval();
  return {fetch->opcode, fetch->rs1, fetch->rs2, fetch->rd, fetch->valC};
}

void write_fetch(unique_ptr<Vfetch> &fetch,
                 unique_ptr<VerilatedContext> &context, int pc, int wdata) {
  fetch->pc = pc;
  fetch->wdata = wdata;
  fetch->write_en = 1;
  clock_fetch(fetch, context);
  fetch->write_en = 0;
}

void reset_fetch(unique_ptr<Vfetch> &fetch,
                 unique_ptr<VerilatedContext> &context) {
  fetch->reset_n = 0;
  fetch->write_en = 1;
  clock_fetch(fetch, context);
  fetch->reset_n = 1;
  fetch->write_en = 0;
}

void clock_fetch(unique_ptr<Vfetch> &fetch,
                 unique_ptr<VerilatedContext> &context) {
  fetch->clock = 0;
  fetch->eval();
  context->timeInc(1);
  fetch->clock = 1;
  fetch->eval();
  context->timeInc(1);
}
