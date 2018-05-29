add_executable(game main.cpp)

target_include_directories(game PRIVATE ${SFML_INCLUDE_DIR})
target_compile_features(game PUBLIC cxx_std_17)
target_compile_definitions(game PRIVATE SFML_STATIC)

target_link_libraries(game ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})