add_executable(inst_mem_testsuit inst_mem_testsuit.cpp)
verilate(inst_mem_testsuit SOURCES ../systemverilog/inst_mem.sv)
add_test(NAME inst_mem_testsuit COMMAND inst_mem_testsuit)

add_executable(fetch_stage_testsuit fetch_stage_testsuit.cpp)
target_link_libraries(fetch_stage_testsuit PRIVATE ${RUBBLER_LIB_DEBUG})
verilate(fetch_stage_testsuit 
INCLUDE_DIRS ../systemverilog/ 
SOURCES ../systemverilog/fetch_stage.sv)
add_test(NAME fetch_stage_testsuit COMMAND fetch_stage_testsuit)

add_executable(reg_file_testsuit reg_file_testsuit.cpp)
verilate(reg_file_testsuit SOURCES ../systemverilog/reg_file.sv)
add_test(NAME reg_file_testsuit COMMAND reg_file_testsuit)
