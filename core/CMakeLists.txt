# core — type aliases, RNG utilities
add_library(phx_core
    src/random.cpp
)

target_include_directories(phx_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
