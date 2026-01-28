# 1. 遥感图像处理程序

> 这是本人大一时， C++ 的课程设计。

Histogram：标签列，当前是哪一个波段

| 表头名称 | 全称 | 解释 |
| --- | --- | --- |
| **DN** | Digital Number | **数字量化值**（即像素的亮度级）。对于 8 位图，范围是 0-255。它代表“这是哪个亮度等级”。 |
| **Npts** | Number of Points | **像素点数**。在该亮度等级（DN）下，图像中一共有多少个像素点。 |
| **Total** | Cumulative Total | **累计点数**。从亮度 0 到当前亮度 DN 的所有像素点总和。最后一行对应的 Total 应该等于图像总像素数。 |
| **Percent** | Percentage | **当前百分比**。该亮度级的像素点数占全图总像素数的比例（）。 |
| **Acc Pct** | Accumulated Percentage | **累计百分比**。到当前亮度为止，所有像素占全图的比例。用于观察图像对比度分布。 |

---

# 2. 项目结果说明

```bash
.
├── cmake_build_debug.sh
├── CMakeLists.txt
├── format_count.sh
├── include
│   ├── image.h
│   ├── menu.h
│   └── utils.h
├── lib
│   ├── CMakeLists.txt
│   ├── image.cpp
│   ├── menu.cpp
│   └── utils.cpp
├── README.md
└── src
    ├── CMakeLists.txt
    └── main.cpp

4 directories, 13 files
```

- `include`：头文件存放处；
- `lib`：动态库/静态库存放处；
- `src`：源文件存放处；

---

# 3. CMake 优化指南

## 3.1 最小化 GLOB 操作

~~`file(GLOB ...)`~~

推荐：
```cmake
set(SOURCES
    main.cpp
    utiles/utiles.cpp
    ...
)

add_executable(myApp ${SOURCES})
```

>> - **CMake 文档明确指出：**
>> "We do not recommend using GLOB to collect a list of source files from your source tree. If no CMakeLists.txt file changes when a source is added or removed then the generated build system cannot know when to ask CMake to regenerate."

## 3.2 使用 Ninja

```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
```

`Ninja`的构建速度很快。

## 3.3 使用现代 CMake

~~`include_directories(${YOUR_DIRECTORY})`~~

~~`link_directories(${YOUR_DIRECTORY})`~~

推荐：
```cmake
target_include_directories(myLib PRIVATE include/)
target_link_libraries(myApp PRIVATE myLib)
```

## 3.4 指定目标编译

如果只想编译`src`目录下的代码文件：
```bash
cmake --build build --target my_program -j12
```

这里的`my_program`与`src/CMakeLists.txt`下的`add_executable(my_program ${ALL_SRCS})`相对应。
