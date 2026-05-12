# quantum — Qubit, Gates, Circuit, Measurement
add_library(phx_quantum
    src/qubit.cpp
    src/gates.cpp
    src/circuit.cpp
    src/measurement.cpp
)

target_include_directories(phx_quantum PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(phx_quantum PUBLIC phx_math phx_core)
