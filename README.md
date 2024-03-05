# RVSV

[![CI](https://github.com/fuad1502/rvsv/actions/workflows/CI.yml/badge.svg)](https://github.com/fuad1502/rvsv/actions/workflows/CI.yml)

`RVSV` is a SystemVerilog implementation of a 5-stage pipelined RISC-V CPU.

# Top level modules

- systemverilog/rv32i_seq.sv (sequential implementation)
- systemverilog/rv32i_pipe.sv (5-stage pipelined implementation)

# Testing

## Prerequisites

- Rust
- Verilator
- CMake (version 3.14 or higher)

## Runnig the testsuits

For simulation and testing, we use [`verilator`](https://github.com/verilator/verilator) to create *verilated* models that are then connected to our C++ testsuits (inside `testsuits` folder). Within the testsuis, we use [`rubbler`](https://github.com/fuad1502/rubbler) to generate test instructions from assembly lines. Execute the following commands to run all test.

```sh
git clone https://github.com/fuad1502/rvsv.git
cd rvsv
cmake -Bbuild -S.
cmake --build build
cd build
ctest
```
