# runtime — CLI entry point
add_executable(phx-quantum src/main.cpp)

target_link_libraries(phx-quantum PRIVATE phx_quantum phx_tensor)
