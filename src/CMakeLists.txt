# src/CMakeLists.txt

set(ALL_SRCS main.cpp)

add_executable(my_program ${ALL_SRCS})

# 只需要链接 utils 即可，OpenCV 的路径和库会自动传递过来
target_link_libraries(my_program PRIVATE utils)

# 如果 main.cpp 没用到其他的 include 路径，下面这行甚至可以省略
target_include_directories(my_program PRIVATE ${PROJECT_SOURCE_DIR}/include)
