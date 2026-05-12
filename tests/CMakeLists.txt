# tests — unit test executables
set(TEST_SOURCES
    test_matrix.cpp
    test_qubit.cpp
    test_gates.cpp
    test_circuit.cpp
    test_measurement.cpp
    test_tensor.cpp
)

foreach(test_src ${TEST_SOURCES})
    get_filename_component(test_name ${test_src} NAME_WE)
    add_executable(${test_name} ${test_src})
    target_link_libraries(${test_name} PRIVATE phx_quantum phx_tensor)
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
