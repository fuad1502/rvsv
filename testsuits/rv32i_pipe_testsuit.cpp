#include "Vrv32i_pipe_tb.h"
#include "Vrv32i_pipe_tb_rv32i_pipe_tb.h"
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

int main(int argc, char *argv[]) {
  unique_ptr<VerilatedContext> context(new VerilatedContext());
  unique_ptr<Vrv32i_pipe_tb> rv32i_tb(new Vrv32i_pipe_tb(context.get(), "TOP"));

  const svScope scope = svGetScopeFromName("TOP.rv32i_pipe_tb");
  assert(scope); // Check for nullptr if scope not found
  svSetScope(scope);

  auto source = R"(
    addi a0, zero, 6
    addi a1, zero, 7
  )";
  auto bytes = (uint8_t *)malloc(sizeof(uint8_t) * MAX_BYTES);
  auto ok = rubble(source, bytes, MAX_BYTES);
  assert(ok);
  bytes_to_pattern_file(bytes, 128, "code_hex.txt");
  rv32i_tb->write_inst_mem("code_hex.txt");

  for (int i = 0; i < 6; i++) {
    rv32i_tb->eval();
    context->timeInc(1);
    rv32i_tb->eval();
    context->timeInc(1);
    printf("a0 = %d\n", rv32i_tb->read_reg_file(10));
    printf("a1 = %d\n", rv32i_tb->read_reg_file(11));
  }
}
