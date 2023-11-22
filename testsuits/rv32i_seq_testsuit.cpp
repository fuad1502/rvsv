#include "Vrv32i_seq.h"
#include "rubbler.h"
#include "verilated.h"
#include <cassert>
#include <cstdio>
#include <memory>

using namespace std;

void clock_memory(unique_ptr<Vrv32i_seq> &memory,
                  unique_ptr<VerilatedContext> &context);

int main(int argc, char *argv[]) {
  unique_ptr<VerilatedContext> context(new VerilatedContext());
  unique_ptr<Vrv32i_seq> memory(new Vrv32i_seq(context.get(), "TOP"));

  const svScope scope = svGetScopeFromName("TOP.rv32i_seq");
  assert(scope); // Check for nullptr if scope not found
  svSetScope(scope);
}
