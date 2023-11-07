#include <Vinst_mem.h>
#include <iostream>
#include <memory>
#include <verilated.h>

using namespace std;

IData read_inst_mem(unique_ptr<Vinst_mem> &inst_mem,
                    unique_ptr<VerilatedContext> &context, int pc);
void write_inst_mem(unique_ptr<Vinst_mem> &inst_mem,
                    unique_ptr<VerilatedContext> &context, int pc, int wdata);
void reset_inst_mem(unique_ptr<Vinst_mem> &inst_mem,
                    unique_ptr<VerilatedContext> &context);
void clock_inst_mem(unique_ptr<Vinst_mem> &inst_mem,
                    unique_ptr<VerilatedContext> &context);

int main(int argc, char *argv[]) {
  unique_ptr<VerilatedContext> context(new VerilatedContext());
  unique_ptr<Vinst_mem> inst_mem(new Vinst_mem(context.get(), "INST_MEM"));

  IData inst;

  reset_inst_mem(inst_mem, context);
  inst = read_inst_mem(inst_mem, context, 0xFF); 
  assert(inst == 0x0);

  write_inst_mem(inst_mem, context, 0xFF, 0x12345678);
  inst = read_inst_mem(inst_mem, context, 0xFF); 
  assert(inst == 0x12345678);
  inst = read_inst_mem(inst_mem, context, 0xFE); 
  assert(inst == 0x34567800);

  return 0;
}

IData read_inst_mem(unique_ptr<Vinst_mem> &inst_mem,
                    unique_ptr<VerilatedContext> &context, int pc) {
  inst_mem->pc = pc;
  inst_mem->eval();
  return inst_mem->inst;
}

void write_inst_mem(unique_ptr<Vinst_mem> &inst_mem,
                    unique_ptr<VerilatedContext> &context, int pc, int wdata) {
  inst_mem->pc = pc;
  inst_mem->wdata = wdata;
  inst_mem->write_en = 1;
  clock_inst_mem(inst_mem, context);
}

void reset_inst_mem(unique_ptr<Vinst_mem> &inst_mem,
                    unique_ptr<VerilatedContext> &context) {
  inst_mem->reset_n = 0;
  inst_mem->write_en = 1;
  clock_inst_mem(inst_mem, context);
  inst_mem->reset_n = 1;
  inst_mem->write_en = 0;
}

void clock_inst_mem(unique_ptr<Vinst_mem> &inst_mem,
                    unique_ptr<VerilatedContext> &context) {
  inst_mem->clock = 0;
  inst_mem->eval();
  context->timeInc(1);
  inst_mem->clock = 1;
  inst_mem->eval();
  context->timeInc(1);
}
