#include <Vfetch.h>
#include <memory>
#include <verilated.h>

using namespace std;

struct inst_fetch {
  CData opcode;
  CData rs1;
  CData rs2;
};

inst_fetch read_fetch(unique_ptr<Vfetch> &fetch,
                      unique_ptr<VerilatedContext> &context, int pc);
void write_fetch(unique_ptr<Vfetch> &fetch,
                 unique_ptr<VerilatedContext> &context, int pc, int wdata);
void reset_fetch(unique_ptr<Vfetch> &fetch,
                 unique_ptr<VerilatedContext> &context);
void clock_fetch(unique_ptr<Vfetch> &fetch,
                 unique_ptr<VerilatedContext> &context);

int main(int argc, char *argv[]) {
  unique_ptr<VerilatedContext> context(new VerilatedContext());
  unique_ptr<Vfetch> fetch(new Vfetch(context.get(), "fetch"));

  IData inst = 0;
  CData rs1 = 0b10101;
  CData rs2 = 0b10001;
  inst |= rs1 << 15;
  inst |= rs2 << 20;

  reset_fetch(fetch, context);

  inst_fetch res;

  // Set opcode to OP
  inst &= ~0b01111111;
  inst |= 0b0110011;
  write_fetch(fetch, context, 0x0, inst);
  res = read_fetch(fetch, context, 0x0);
  VL_PRINTF("inst: %032b\n"
            "opcode : %07b\n"
            "rs1: %05b\n"
            "rs2: %05b\n",
            inst, res.opcode, res.rs1, res.rs2);
  assert(res.rs1 == rs1);
  assert(res.rs2 == rs2);

  // Set opcode to OP-IMM
  inst &= ~0b01111111;
  inst |= 0b0010011;
  write_fetch(fetch, context, 0x0, inst);
  res = read_fetch(fetch, context, 0x0);
  VL_PRINTF("inst: %032b\n"
            "opcode : %07b\n"
            "rs1: %05b\n"
            "rs2: %05b\n",
            inst, res.opcode, res.rs1, res.rs2);
  assert(res.rs1 == rs1);
  assert(res.rs2 == 0);

  // Set opcode to INVALID
  inst &= ~0b01111111;
  inst |= 0b0000000;
  write_fetch(fetch, context, 0x0, inst);
  res = read_fetch(fetch, context, 0x0);
  VL_PRINTF("inst: %032b\n"
            "opcode : %07b\n"
            "rs1: %05b\n"
            "rs2: %05b\n",
            inst, res.opcode, res.rs1, res.rs2);
  assert(res.rs1 == 0);
  assert(res.rs2 == 0);

  return 0;
}

inst_fetch read_fetch(unique_ptr<Vfetch> &fetch,
                      unique_ptr<VerilatedContext> &context, int pc) {
  fetch->pc = pc;
  fetch->eval();
  return {fetch->opcode, fetch->rs1, fetch->rs2};
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
