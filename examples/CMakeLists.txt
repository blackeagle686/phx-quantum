# examples — demo circuits
add_executable(bell_state bell_state.cpp)
target_link_libraries(bell_state PRIVATE phx_quantum)

add_executable(teleportation teleportation.cpp)
target_link_libraries(teleportation PRIVATE phx_quantum)
