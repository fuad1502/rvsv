add_executable(inst_mem_testsuit inst_mem_testsuit.cpp)
verilate(inst_mem_testsuit SOURCES ../systemverilog/inst_mem.sv)
add_test(NAME inst_mem_testsuit COMMAND inst_mem_testsuit)

add_executable(fetch_stage_testsuit fetch_stage_testsuit.cpp)
target_link_libraries(fetch_stage_testsuit PRIVATE ${RUBBLER_LIB})
verilate(fetch_stage_testsuit 
INCLUDE_DIRS ../systemverilog/ 
SOURCES ../systemverilog/fetch_stage.sv)
add_test(NAME fetch_stage_testsuit COMMAND fetch_stage_testsuit)

add_executable(reg_file_testsuit reg_file_testsuit.cpp)
verilate(reg_file_testsuit SOURCES ../systemverilog/reg_file.sv)
add_test(NAME reg_file_testsuit COMMAND reg_file_testsuit)

add_executable(execute_stage_testsuit execute_stage_testsuit.cpp)
target_link_libraries(execute_stage_testsuit PRIVATE ${RUBBLER_LIB})
verilate(execute_stage_testsuit 
INCLUDE_DIRS ../systemverilog/ 
SOURCES ../systemverilog/execute_stage_tb.sv)
add_test(NAME execute_stage_testsuit COMMAND execute_stage_testsuit)

add_executable(memory_testsuit memory_testsuit.cpp)
target_link_libraries(memory_testsuit PRIVATE ${RUBBLER_LIB})
verilate(memory_testsuit 
INCLUDE_DIRS ../systemverilog/ 
SOURCES ../systemverilog/memory_tb.sv)
configure_file(${CMAKE_SOURCE_DIR}/systemverilog/memory_tb_pattern.txt ${CMAKE_BINARY_DIR}/testsuits/memory_tb_pattern.txt COPYONLY)
add_test(NAME memory_testsuit COMMAND memory_testsuit)

add_executable(rv32i_seq_testsuit rv32i_seq_testsuit.cpp)
target_link_libraries(rv32i_seq_testsuit PRIVATE ${RUBBLER_LIB})
verilate(rv32i_seq_testsuit 
INCLUDE_DIRS ../systemverilog/ 
SOURCES ../systemverilog/rv32i_seq_tb.sv
VERILATOR_ARGS --vpi --timing)
file(COPY ${CMAKE_SOURCE_DIR}/testsuits/asm_tests/ DESTINATION ${CMAKE_BINARY_DIR}/testsuits/asm_tests)
add_test(NAME rv32i_seq_testsuit COMMAND rv32i_seq_testsuit)

add_executable(rv32i_pipe_testsuit rv32i_pipe_testsuit.cpp)
target_link_libraries(rv32i_pipe_testsuit PRIVATE ${RUBBLER_LIB})
verilate(rv32i_pipe_testsuit 
INCLUDE_DIRS ../systemverilog/ 
SOURCES ../systemverilog/rv32i_pipe_tb.sv
VERILATOR_ARGS --vpi --timing)
file(COPY ${CMAKE_SOURCE_DIR}/testsuits/asm_tests/ DESTINATION ${CMAKE_BINARY_DIR}/testsuits/asm_tests)
add_test(NAME rv32i_pipe_testsuit COMMAND rv32i_pipe_testsuit)
