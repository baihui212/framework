set(OUTPUTNAME ServerTest)
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")

aux_source_directory(. DIR_SRCS)

include_directories(${PROJECT_SOURCE_DIR}/tests/MessageDef)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories($ENV{BOOST_INCLUDE_PATH})

link_directories(${PROJECT_SOURCE_DIR}/lib)
link_directories($ENV{THRIFT_LIB_PATH})
link_directories($ENV{BOOST_LIB_PATH})

add_executable(${OUTPUTNAME} ${DIR_SRCS})

target_link_libraries(${OUTPUTNAME} libthrift_wrapper.a)
target_link_libraries(${OUTPUTNAME} libthrift.a)
target_link_libraries(${OUTPUTNAME} libboost_serialization.a)
target_link_libraries(${OUTPUTNAME} libpthread.so)
