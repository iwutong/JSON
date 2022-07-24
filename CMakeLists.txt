cmake_minimum_required(VERSION 2.6)
project(leptjson_test C)


set(LIB_SRC leptjson.c)
set(APP_SRC test.c)
add_library(leptjsonlib ${LIB_SRC})
add_executable(leptjson_test ${APP_SRC})
target_link_libraries(leptjson_test leptjsonlib)