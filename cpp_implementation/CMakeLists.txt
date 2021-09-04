cmake_minimum_required(VERSION 3.10)

project(slh)

find_package(Boost REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

include_directories(src/Data src/Logic src/Parse src/Transform)

set(SOURCES
    src/main.cpp	
    src/Logic/solver.cpp
    src/Logic/stage.cpp
    src/Data/graph.cpp
    src/Data/ordering.cpp
    src/Data/ordering_iterator.cpp
    src/Data/pair.cpp
    src/Transform/transform.cpp
    src/Transform/isomorphism.cpp
    src/Transform/closing_transformation.cpp
    src/Transform/floating_transformation.cpp
    src/Transform/floating_opt_transformation.cpp
    src/Transform/opening_transformation.cpp
    src/Parse/parse.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
