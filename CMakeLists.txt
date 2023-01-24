project(Deroute)
cmake_minimum_required(VERSION 3.25)

set(libfaiss "C:/Program Files (x86)/faiss/lib/libfaiss.dll.a")
set(libfasttext "C:/Program Files (x86)/fasttext/lib/libfasttext.dll.a")

add_compile_options(-O2)

add_executable(search EXCLUDE_FROM_ALL src/search.cpp)
set_target_properties( search PROPERTIES RUNTIME_OUTPUT_DIRECTORY CMAKE_CURRENT_LIST_DIR/../../bin/)
target_include_directories(search PRIVATE "C:/Program Files (x86)/faiss/include")
target_link_libraries(search ${libfaiss})

add_executable(embed EXCLUDE_FROM_ALL src/embed.cpp)
set_target_properties( embed PROPERTIES RUNTIME_OUTPUT_DIRECTORY CMAKE_CURRENT_LIST_DIR/../../bin/)
target_include_directories(embed PRIVATE "C:/Program Files (x86)/fasttext/include/fasttext")
target_link_libraries(embed ${libfasttext})

set(SOURCES src/vec_tools.cpp)
add_executable(vectools EXCLUDE_FROM_ALL ${SOURCES})
set_target_properties(vectools PROPERTIES RUNTIME_OUTPUT_DIRECTORY CMAKE_CURRENT_LIST_DIR/../../bin/)
target_include_directories(vectools PRIVATE "C:/Program Files (x86)/fasttext/include/fasttext" "src/")
target_link_libraries(vectools ${libfasttext})
