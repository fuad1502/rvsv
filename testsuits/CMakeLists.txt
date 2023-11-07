add_executable(inst_mem_testsuit inst_mem_testsuit.cpp)
verilate(inst_mem_testsuit SOURCES ../systemverilog/inst_mem.sv)
add_test(NAME inst_mem_testsuit COMMAND inst_mem_testsuit)

add_executable(fetch_testsuit fetch_testsuit.cpp)
target_link_libraries(fetch_testsuit PRIVATE ${RUBBLER_LIB_DEBUG})
verilate(fetch_testsuit 
INCLUDE_DIRS ../systemverilog/ 
SOURCES ../systemverilog/fetch.sv)
add_test(NAME fetch_testsuit COMMAND fetch_testsuit)
