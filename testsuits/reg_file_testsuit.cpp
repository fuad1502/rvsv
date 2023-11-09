#include "Vreg_file.h"
#include "verilated.h"
#include <cassert>
#include <cstdio>
#include <memory>

using namespace std;

IData read_reg_file_rs1(unique_ptr<Vreg_file> &reg_file,
                    unique_ptr<VerilatedContext> &context, int rs1);
IData read_reg_file_rs2(unique_ptr<Vreg_file> &reg_file,
                    unique_ptr<VerilatedContext> &context, int rs1);
void write_reg_file(unique_ptr<Vreg_file> &reg_file,
                    unique_ptr<VerilatedContext> &context, int rd, int wdata);
void clock_reg_file(unique_ptr<Vreg_file> &reg_file,
                    unique_ptr<VerilatedContext> &context);

int main(int argc, char *argv[]) {
  unique_ptr<VerilatedContext> context(new VerilatedContext());
  unique_ptr<Vreg_file> reg_file(new Vreg_file(context.get(), "REG_FILE"));

  // Write to register 0, 1, 2
  write_reg_file(reg_file, context, 0, 0xAAAAAAAA);
  write_reg_file(reg_file, context, 1, 0xBBBBBBBB);
  write_reg_file(reg_file, context, 2, 0xCCCCCCCC);

  assert(read_reg_file_rs1(reg_file, context, 0) == 0);
  assert(read_reg_file_rs2(reg_file, context, 0) == 0);
  assert(read_reg_file_rs1(reg_file, context, 1) == 0x0BBBBBBBB);
  assert(read_reg_file_rs2(reg_file, context, 1) == 0x0BBBBBBBB);
  assert(read_reg_file_rs1(reg_file, context, 2) == 0x0CCCCCCCC);
  assert(read_reg_file_rs2(reg_file, context, 2) == 0x0CCCCCCCC);

  reg_file->final();

  return 0;
}

IData read_reg_file_rs1(unique_ptr<Vreg_file> &reg_file,
                    unique_ptr<VerilatedContext> &context, int rs1) {
  reg_file->rs1 = rs1;
  reg_file->eval();
  return reg_file->valA;
}

IData read_reg_file_rs2(unique_ptr<Vreg_file> &reg_file,
                    unique_ptr<VerilatedContext> &context, int rs2) {
  reg_file->rs2 = rs2;
  reg_file->eval();
  return reg_file->valB;
}

void write_reg_file(unique_ptr<Vreg_file> &reg_file,
                    unique_ptr<VerilatedContext> &context, int rd, int wdata) {
  reg_file->write_en = 1;
  reg_file->rd = rd;
  reg_file->wdata = wdata;
  clock_reg_file(reg_file, context);
  reg_file->write_en = 0;
}

void clock_reg_file(unique_ptr<Vreg_file> &reg_file,
                    unique_ptr<VerilatedContext> &context) {
  reg_file->clock = 0;
  reg_file->eval();
  context->timeInc(1);
  reg_file->clock = 1;
  reg_file->eval();
  context->timeInc(1);
}
