# lib/CMakeLists.txt

# 1. 查找 OpenCV（建议在根目录或此目录下查找）
find_package(OpenCV REQUIRED)

# 2. 打印查找结果
message(STATUS "OpenCV library status:")
message(STATUS "version: ${OpenCV_VERSION}")
message(STATUS "libraries: ${OpenCV_LIBRARIES}")
message(STATUS "include path: ${OpenCV_INCLUDE_DIRS}")

# 3. 定义库
add_library(utils SHARED
  image.cpp
  menu.cpp
  utils.cpp
)

# 4. 设置头文件路径
target_include_directories(utils PRIVATE ${PROJECT_SOURCE_DIR}/include)

# 5. 将 OpenCV 链接到 utils 库
# 使用 PUBLIC，意味着任何链接 utils 的目标（如 main）都会自动获得 OpenCV 的头文件路径
target_include_directories(utils PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(utils PUBLIC ${OpenCV_LIBRARIES})

message("\n--------------------------------------------")
