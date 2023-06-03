project(Deroute)
cmake_minimum_required(VERSION 3.25)
set (CMAKE_CXX_STANDARD 17)

set(CMAKE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)

#paths 
set(faiss_path "C:/Program Files (x86)/faiss")
set(fasttext_path "C:/Program Files (x86)/fasttext")
set(intel_mkl "C:/Program Files (x86)/Intel/oneAPI/mkl/2022.2.1")
set(compiler_path "C:/Program Files/mingw64")


#library files for fasttext and faiss
set(libfaiss "${faiss_path}/lib/libfaiss.dll.a")
set(libfasttext "${fasttext_path}/lib/libfasttext.dll.a")

#include paths of faiss and fasttext
set(faiss_include "${faiss_path}/include/")
set(fasttext_include "${fasttext_path}/include/")

add_compile_options(-O2)

#Deroute Library
add_library(deroute SHARED  src/deroute.cpp src/search.cpp src/utils.cpp src/embed.cpp src/dictionary.cpp src/vector.cpp)
set_target_properties( deroute PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(deroute PRIVATE ${faiss_include} "src/")
target_link_libraries(deroute ${libfaiss})

#LSH
add_library(lsh SHARED  src/local_hash.cpp src/utils.cpp)
set_target_properties(lsh PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(lsh PRIVATE ${faiss_include} "src/")
target_link_libraries(lsh ${libfaiss})

#Compress
add_executable(compress src/compress.cpp)
set_target_properties( compress PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(compress PRIVATE ${fasttext_include} "src/")
target_link_libraries(compress ${libfasttext})


#command to get needed dlls
add_custom_target(copy_dlls
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${compiler_path}/bin/libstdc++-6.dll" "${CMAKE_CURRENT_BINARY_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${compiler_path}/bin/libgcc_s_seh-1.dll" "${CMAKE_CURRENT_BINARY_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${compiler_path}/bin/libwinpthread-1.dll" "${CMAKE_CURRENT_BINARY_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${compiler_path}/bin/libgomp-1.dll" "${CMAKE_CURRENT_BINARY_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${intel_mkl}/redist/intel64/mkl_rt.2.dll" "${CMAKE_CURRENT_BINARY_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${faiss_path}/bin/libfaiss.dll" "${CMAKE_CURRENT_BINARY_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${fasttext_path}/bin/libfasttext.dll" "${CMAKE_CURRENT_BINARY_DIR}"
    COMMENT "Copying DLLs to ${CMAKE_CURRENT_BINARY_DIR}"
)

#Build Controller
set(PYTHON_SCRIPT "./build.py")
set(PYTHON_SCRIPT_ARGS "py2exe")
add_custom_target(controller
    COMMAND python ${PYTHON_SCRIPT} ${PYTHON_SCRIPT_ARGS}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Building Controller"
    DEPENDS copy_dlls deroute
)


#Search (old) do not use
add_library(search SHARED  src/search.cpp src/utils.cpp)
set_target_properties( search PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(search PRIVATE ${faiss_include} "src/")
target_link_libraries(search ${libfaiss})

#Embed (old) do not use
add_library(embed SHARED  src/embed.cpp src/dictionary.cpp src/vector.cpp)
set_target_properties( embed PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(embed PRIVATE "src/")
