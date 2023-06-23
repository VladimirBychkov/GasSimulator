add_executable(gas_simulator 
	main.cpp 
	World.cpp 
	Particle.cpp 
	GridCell.cpp
	Grid.cpp
	utils_math.h
  "Renderer.h" "Renderer.cpp" "multithreading.h")

 add_subdirectory(tests)

target_link_libraries(gas_simulator PRIVATE sfml-system sfml-network sfml-graphics sfml-window)