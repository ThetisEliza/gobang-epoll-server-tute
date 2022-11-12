cmake_minimum_required(VERSION 3.2.4)
project(GES)

set(CMAKE_CXX_FLAGS -lnuma)
set(CMAKE_CXX_FLAGS -pthread)
set(EXECUTABLE_OUTPUT_PATH ../release)

aux_source_directory(./src/server SRC)
aux_source_directory(./src/utils UTILS)

add_executable(ges ../src/main.cc ${SRC} ${UTILS})
add_executable(test ../src/test.cc ${SRC} ${UTILS})

# add_executable(tute1 ../tutes/1.tutes-build-a-dump-server.cc ${SRC} ${UTILS})

# target_include_directories(server PRIVATE ${PROJECT_SOURCE_DIR}/inc)
# target_link_libraries(server PRIVATE ${PROJECT_SOURCE_DIR}/lib/libfuture_static.a)

# target_include_directories(client PRIVATE ${PROJECT_SOURCE_DIR}/inc)
# target_link_libraries(client PRIVATE ${PROJECT_SOURCE_DIR}/lib/libfuture_static.a)


