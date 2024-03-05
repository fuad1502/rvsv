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
#define PIPE_DELAY 5

void bytes_to_pattern_file(uint8_t *bytes, size_t size, const char *file_name) {
  auto f = fopen(file_name, "w");
  for (int i = 0; i < size; i++) {
    fprintf(f, "%02x\n", bytes[i]);
  }
  fclose(f);
}

void run_bytes(uint8_t *bytes, const size_t size,
               unique_ptr<VerilatedContext> &context,
               unique_ptr<Vrv32i_pipe_tb> &rv32i_tb) {
  bytes_to_pattern_file(bytes, size, "code_hex.txt");
  rv32i_tb->write_inst_mem("code_hex.txt");
  while (rv32i_tb->rv32i_pipe_tb->fault == 0) {
    rv32i_tb->eval();
    context->timeInc(1);
    rv32i_tb->eval();
    context->timeInc(1);
  }
}

void store_bytes(uint8_t *bytes, const size_t size,
                 unique_ptr<VerilatedContext> &context,
                 unique_ptr<Vrv32i_pipe_tb> &rv32i_tb) {
  bytes_to_pattern_file(bytes, size, "code_hex.txt");
  rv32i_tb->write_mem("code_hex.txt");
}

void single_inst_test(unique_ptr<VerilatedContext> &context,
                      unique_ptr<Vrv32i_pipe_tb> &rv32i_tb) {
  auto source = R"(
    addi a0, zero, 6
  )";
  auto bytes = (uint8_t *)malloc(sizeof(uint8_t) * MAX_BYTES);
  uintptr_t size = MAX_BYTES;
  auto ok = rubble(source, bytes, &size);
  assert(ok);
  run_bytes(bytes, 4, context, rv32i_tb);
  auto a0 = rv32i_tb->read_reg_file(10);
  assert(a0 == 6);
}

void data_hazard_1(unique_ptr<VerilatedContext> &context,
                   unique_ptr<Vrv32i_pipe_tb> &rv32i_tb) {
  auto source = R"(
    addi a0, zero, 6
    addi a0, a0, 9
  )";
  auto bytes = (uint8_t *)malloc(sizeof(uint8_t) * MAX_BYTES);
  uintptr_t size = MAX_BYTES;
  auto ok = rubble(source, bytes, &size);
  assert(ok);
  run_bytes(bytes, 8, context, rv32i_tb);
  auto a0 = rv32i_tb->read_reg_file(10);
  assert(a0 == 15);
}

void data_hazard_2(unique_ptr<VerilatedContext> &context,
                   unique_ptr<Vrv32i_pipe_tb> &rv32i_tb) {
  auto source = R"(
    addi a0, zero, 6
    add zero, zero, zero
    addi a0, a0, 9
  )";
  auto bytes = (uint8_t *)malloc(sizeof(uint8_t) * MAX_BYTES);
  uintptr_t size = MAX_BYTES;
  auto ok = rubble(source, bytes, &size);
  assert(ok);
  run_bytes(bytes, 12, context, rv32i_tb);
  auto a0 = rv32i_tb->read_reg_file(10);
  assert(a0 == 15);
}

void data_hazard_3(unique_ptr<VerilatedContext> &context,
                   unique_ptr<Vrv32i_pipe_tb> &rv32i_tb) {
  auto source = R"(
    addi a0, zero, 6
    add zero, zero, zero
    add zero, zero, zero
    addi a0, a0, 9
  )";
  auto bytes = (uint8_t *)malloc(sizeof(uint8_t) * MAX_BYTES);
  uintptr_t size = MAX_BYTES;
  auto ok = rubble(source, bytes, &size);
  assert(ok);
  run_bytes(bytes, 16, context, rv32i_tb);
  auto a0 = rv32i_tb->read_reg_file(10);
  assert(a0 == 15);
}

void no_hazard_1(unique_ptr<VerilatedContext> &context,
                 unique_ptr<Vrv32i_pipe_tb> &rv32i_tb) {
  auto source = R"(
    addi a0, zero, 6
    add zero, zero, zero
    add zero, zero, zero
    add zero, zero, zero
    addi a0, a0, 9
  )";
  auto bytes = (uint8_t *)malloc(sizeof(uint8_t) * MAX_BYTES);
  uintptr_t size = MAX_BYTES;
  auto ok = rubble(source, bytes, &size);
  assert(ok);
  run_bytes(bytes, 20, context, rv32i_tb);
  auto a0 = rv32i_tb->read_reg_file(10);
  assert(a0 == 15);
}

void misprediction(unique_ptr<VerilatedContext> &context,
                   unique_ptr<Vrv32i_pipe_tb> &rv32i_tb) {
  auto source = R"(
    addi a0, zero, 6
    jal zero, x
    add a0, a0, a0
    x:
    addi a0, a0, 1
  )";
  auto bytes = (uint8_t *)malloc(sizeof(uint8_t) * MAX_BYTES);
  uintptr_t size = MAX_BYTES;
  auto ok = rubble(source, bytes, &size);
  assert(ok);
  run_bytes(bytes, 16, context, rv32i_tb);
  auto a0 = rv32i_tb->read_reg_file(10);
  assert(a0 == 7);
}

void load_store_hazard(unique_ptr<VerilatedContext> &context,
                       unique_ptr<Vrv32i_pipe_tb> &rv32i_tb) {
  auto source = R"(
    .section .data
    five:
    .byte 5, 0, 0, 0
    .section .text
    lw a0, 8(zero)
    addi a0, a0, 7
  )";
  auto bytes = (uint8_t *)malloc(sizeof(uint8_t) * MAX_BYTES);
  uintptr_t size = MAX_BYTES;
  auto ok = rubble(source, bytes, &size);
  assert(ok);
  store_bytes(bytes, 12, context, rv32i_tb);
  run_bytes(bytes, 12, context, rv32i_tb);
  auto a0 = rv32i_tb->read_reg_file(10);
  assert(a0 == 12);
}

void test_fibonacci(int n, int expected_result,
                    unique_ptr<VerilatedContext> &context,
                    unique_ptr<Vrv32i_pipe_tb> &rv32i_tb) {
  auto bytes = (uint8_t *)malloc(sizeof(uint8_t) * MAX_BYTES);
  uintptr_t size = MAX_BYTES;
  auto ok = rubble_file("asm_tests/fibonacci.asm", bytes, &size);
  assert(ok);
  rv32i_tb->write_reg_file(10, n);
  run_bytes(bytes, 256, context, rv32i_tb);
  auto a0 = rv32i_tb->read_reg_file(10);
  assert(a0 == expected_result);
}

int main(int argc, char *argv[]) {
  {
    unique_ptr<VerilatedContext> context(new VerilatedContext());
    unique_ptr<Vrv32i_pipe_tb> rv32i_tb(
        new Vrv32i_pipe_tb(context.get(), "TOP"));

    const svScope scope = svGetScopeFromName("TOP.rv32i_pipe_tb");
    assert(scope); // Check for nullptr if scope not found
    svSetScope(scope);

    single_inst_test(context, rv32i_tb);
  }
  {
    unique_ptr<VerilatedContext> context(new VerilatedContext());
    unique_ptr<Vrv32i_pipe_tb> rv32i_tb(
        new Vrv32i_pipe_tb(context.get(), "TOP"));

    const svScope scope = svGetScopeFromName("TOP.rv32i_pipe_tb");
    assert(scope); // Check for nullptr if scope not found
    svSetScope(scope);

    data_hazard_1(context, rv32i_tb);
  }
  {
    unique_ptr<VerilatedContext> context(new VerilatedContext());
    unique_ptr<Vrv32i_pipe_tb> rv32i_tb(
        new Vrv32i_pipe_tb(context.get(), "TOP"));

    const svScope scope = svGetScopeFromName("TOP.rv32i_pipe_tb");
    assert(scope); // Check for nullptr if scope not found
    svSetScope(scope);

    data_hazard_2(context, rv32i_tb);
  }
  {
    unique_ptr<VerilatedContext> context(new VerilatedContext());
    unique_ptr<Vrv32i_pipe_tb> rv32i_tb(
        new Vrv32i_pipe_tb(context.get(), "TOP"));

    const svScope scope = svGetScopeFromName("TOP.rv32i_pipe_tb");
    assert(scope); // Check for nullptr if scope not found
    svSetScope(scope);

    data_hazard_3(context, rv32i_tb);
  }
  {
    unique_ptr<VerilatedContext> context(new VerilatedContext());
    unique_ptr<Vrv32i_pipe_tb> rv32i_tb(
        new Vrv32i_pipe_tb(context.get(), "TOP"));

    const svScope scope = svGetScopeFromName("TOP.rv32i_pipe_tb");
    assert(scope); // Check for nullptr if scope not found
    svSetScope(scope);

    no_hazard_1(context, rv32i_tb);
  }
  {
    unique_ptr<VerilatedContext> context(new VerilatedContext());
    unique_ptr<Vrv32i_pipe_tb> rv32i_tb(
        new Vrv32i_pipe_tb(context.get(), "TOP"));

    const svScope scope = svGetScopeFromName("TOP.rv32i_pipe_tb");
    assert(scope); // Check for nullptr if scope not found
    svSetScope(scope);

    misprediction(context, rv32i_tb);
  }
  {
    unique_ptr<VerilatedContext> context(new VerilatedContext());
    unique_ptr<Vrv32i_pipe_tb> rv32i_tb(
        new Vrv32i_pipe_tb(context.get(), "TOP"));

    const svScope scope = svGetScopeFromName("TOP.rv32i_pipe_tb");
    assert(scope); // Check for nullptr if scope not found
    svSetScope(scope);

    load_store_hazard(context, rv32i_tb);
  }
  {
    unique_ptr<VerilatedContext> context(new VerilatedContext());
    unique_ptr<Vrv32i_pipe_tb> rv32i_tb(
        new Vrv32i_pipe_tb(context.get(), "TOP"));

    const svScope scope = svGetScopeFromName("TOP.rv32i_pipe_tb");
    assert(scope); // Check for nullptr if scope not found
    svSetScope(scope);

    test_fibonacci(12, 144, context, rv32i_tb);
  }
}
