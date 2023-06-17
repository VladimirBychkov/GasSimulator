add_executable(test_grid 
	test_grid.cpp
	../Grid.cpp
	../GridCell.cpp
)

 add_executable(test_batch_drawing
	test_batch_drawing.cpp
 )

  add_executable(test_multithreading
	test_multithreading.cpp
  )

target_link_libraries(test_grid PRIVATE sfml-graphics)
target_link_libraries(test_batch_drawing PRIVATE sfml-graphics)
target_link_libraries(test_multithreading PRIVATE sfml-system)