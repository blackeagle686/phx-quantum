# math — Matrix<T>, linear algebra utilities
# Header-only (templates), but we keep a library target for include paths
add_library(phx_math INTERFACE)

target_include_directories(phx_math INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(phx_math INTERFACE phx_core)
