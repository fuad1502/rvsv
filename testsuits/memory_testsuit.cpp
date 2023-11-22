#include "Vmemory_tb.h"
#include "rubbler.h"
#include "verilated.h"
#include <cassert>
#include <cstdio>
#include <memory>

using namespace std;

void clock_memory(unique_ptr<Vmemory_tb> &memory,
                  unique_ptr<VerilatedContext> &context);

int main(int argc, char *argv[]) {
  unique_ptr<VerilatedContext> context(new VerilatedContext());
  unique_ptr<Vmemory_tb> memory(new Vmemory_tb(context.get(), "TOP"));

  const svScope scope = svGetScopeFromName("TOP.memory_tb");
  assert(scope); // Check for nullptr if scope not found
  svSetScope(scope);

  ///////////////////// TEST MEMORY READ //////////////////////
  // Write test pattern
  memory->write_mem("memory_tb_pattern.txt");
  // Common input signals
  memory->addr = 0xFF;
  memory->read_en = 1;
  memory->write_en = 0;
  // Test single byte read (signed)
  memory->width = 0b000;
  memory->sign_extend = 1;
  memory->eval();
  assert(memory->mem_fault == 0);
  assert(memory->valM == 0xFFFFFF98);
  // Test single byte read (unsigned)
  memory->width = 0b000;
  memory->sign_extend = 0;
  memory->eval();
  assert(memory->mem_fault == 0);
  assert(memory->valM == 0x00000098);
  // Test two bytes read (signed)
  memory->width = 0b001;
  memory->sign_extend = 1;
  memory->eval();
  assert(memory->mem_fault == 0);
  assert(memory->valM == 0xFFFFBA98);
  // Test two bytes read (unsigned)
  memory->width = 0b001;
  memory->sign_extend = 0;
  memory->eval();
  assert(memory->mem_fault == 0);
  assert(memory->valM == 0x0000BA98);
  // Test four bytes read (signed)
  memory->width = 0b010;
  memory->sign_extend = 1;
  memory->eval();
  assert(memory->mem_fault == 0);
  assert(memory->valM == 0xFEDCBA98);

  ///////////////////// TEST MEMORY WRITE //////////////////////
  // Common input signals
  memory->read_en = 0;
  memory->write_en = 1;
  memory->wdata = 0x12345678;
  // Test single byte write
  memory->addr = 0xAA;
  memory->width = 0b000;
  clock_memory(memory, context);
  assert(memory->mem_fault == 0);
  assert(memory->read_mem(0xAA, 1) == 0x78);
  // Test two bytes write
  memory->addr = 0xBB;
  memory->width = 0b001;
  clock_memory(memory, context);
  assert(memory->mem_fault == 0);
  assert(memory->read_mem(0xBB, 2) == 0x5678);
  // Test four bytes write
  memory->addr = 0xCC;
  memory->width = 0b010;
  clock_memory(memory, context);
  assert(memory->mem_fault == 0);
  assert(memory->read_mem(0xCC, 4) == 0x12345678);

  ///////////////////// TEST MEMORY FAULT //////////////////////
  // First byte accesses address 0 
  memory->addr = 0x0;
  memory->width = 0b000;
  memory->read_en = 1;
  memory->write_en = 0;
  memory->eval();
  assert(memory->mem_fault == 1);
  // Second byte access address >= MEM_SIZE
  memory->addr = 1023;
  memory->width = 0b001;
  memory->read_en = 1;
  memory->write_en = 0;
  memory->eval();
  assert(memory->mem_fault == 1);
  // Third byte access address >= MEM_SIZE
  memory->addr = 1022;
  memory->width = 0b010;
  memory->read_en = 1;
  memory->write_en = 0;
  memory->eval();
  assert(memory->mem_fault == 1);
  // Third byte access address >= MEM_SIZE
  memory->addr = 1021;
  memory->width = 0b010;
  memory->read_en = 1;
  memory->write_en = 0;
  memory->eval();
  assert(memory->mem_fault == 1);
  // Invalid width 
  memory->addr = 1;
  memory->width = 0b011;
  memory->read_en = 1;
  memory->write_en = 0;
  memory->eval();
  assert(memory->mem_fault == 1);

  memory->final();
}

void clock_memory(unique_ptr<Vmemory_tb> &memory,
                  unique_ptr<VerilatedContext> &context) {
  memory->clock = 0;
  memory->eval();
  context->timeInc(1);
  memory->clock = 1;
  memory->eval();
  context->timeInc(1);
}
