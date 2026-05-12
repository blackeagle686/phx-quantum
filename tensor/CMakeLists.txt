# tensor — N-dimensional tensor core
add_library(phx_tensor INTERFACE)

target_include_directories(phx_tensor INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(phx_tensor INTERFACE phx_core)
