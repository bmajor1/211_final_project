cmake_minimum_required(VERSION 3.3)
# TODO: CHANGE THE NEXT LINE TO THE INCLUDE THE TITLE OF YOUR GAME:
project(eight-ball-cue CXX)
include(.cs211/cmake/CMakeLists.txt)

# TODO: PUT ADDITIONAL MODEL .cxx FILES IN THIS LIST:
set(MODEL_SRC
        src/model.cxx
        src/ball.cxx
        src/pool_table.cxx
        )

# TODO: PUT ADDITIONAL NON-MODEL (UI) .cxx FILES IN THIS LIST:
add_program(eight-ball-cue
        src/eight-ball-cue.cxx
        src/controller.cxx
        src/view.cxx
    ${MODEL_SRC})
target_link_libraries(eight-ball-cue ge211)

add_test_program(model_test
    test/controller_test.cxx
    ${MODEL_SRC})
target_link_libraries(model_test ge211)

add_test_program(controller_test
        test/controller_test.cxx
        ${MODEL_SRC})
target_link_libraries(controller_test ge211)

add_test_program(helper_test
        test/ball_test.cxx
        test/pool_table_test.cxx
        ${MODEL_SRC})
target_link_libraries(helper_test ge211)
# vim: ft=cmake
