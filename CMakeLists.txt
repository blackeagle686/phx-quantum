cmake_minimum_required(VERSION 3.20)
project(phx-quantum
    VERSION 0.1.0
    DESCRIPTION "Pure CPU Quantum State Simulator"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# MSVC specific flags
if(MSVC)
    add_compile_options(/W4 /permissive- /Zc:__cplusplus)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ── Libraries ──────────────────────────────────────────────
add_subdirectory(core)
add_subdirectory(math)
add_subdirectory(quantum)
add_subdirectory(tensor)

# ── Executables ────────────────────────────────────────────
add_subdirectory(runtime)
add_subdirectory(examples)

# ── Tests ──────────────────────────────────────────────────
enable_testing()
add_subdirectory(tests)
