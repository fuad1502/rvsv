#include "Vrv32i_seq_tb.h"
#include "Vrv32i_seq_tb_rv32i_seq_tb.h"
#include "rubbler.h"
#include "verilated.h"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <memory>

using namespace std;

#define MAX_BYTES 1024

void bytes_to_pattern_file(uint8_t *bytes, size_t size, const char *file_name) {
  auto f = fopen(file_name, "w");
  for (int i = 0; i < size; i++) {
    fprintf(f, "%02x\n", bytes[i]);
  }
  fclose(f);
}

void test_fibonacci(int n, int expected_result,
                    unique_ptr<VerilatedContext> &context,
                    unique_ptr<Vrv32i_seq_tb> &rv32i_tb) {
  auto bytes = (uint8_t *)malloc(sizeof(uint8_t) * MAX_BYTES);
  auto ok = rubble_file("asm_tests/fibonacci.asm", bytes, MAX_BYTES);
  assert(ok);
  bytes_to_pattern_file(bytes, 128, "code_hex.txt");
  rv32i_tb->write_inst_mem("code_hex.txt");
  rv32i_tb->write_reg_file(10, 12);
  while (rv32i_tb->rv32i_seq_tb->inst_fault != 1) {
    context->timeInc(1);
    rv32i_tb->eval();
    context->timeInc(1);
    rv32i_tb->eval();
  }
  assert(rv32i_tb->read_reg_file(10) == expected_result);
}

int main(int argc, char *argv[]) {
  unique_ptr<VerilatedContext> context(new VerilatedContext());
  unique_ptr<Vrv32i_seq_tb> rv32i_tb(new Vrv32i_seq_tb(context.get(), "TOP"));

  const svScope scope = svGetScopeFromName("TOP.rv32i_seq_tb");
  assert(scope); // Check for nullptr if scope not found
  svSetScope(scope);

  test_fibonacci(11, 144, context, rv32i_tb);
}
