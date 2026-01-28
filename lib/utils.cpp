#include "utils.h"

// 打开图像文件
Image *Utils::openImage() {
  std::string path;  // 图像文件路径
  std::cout << "请输入图像文件路径：";
  std::cin >> path;
  Image *image = new Image(path);  // 创建图像对象，根据路径读取图像文件
  if (image->isEmpty()) {
    delete image;
    image = NULL;
    return NULL;
  }
  return image;  // 返回图像对象指针
}

// 关闭图像文件
void Utils::closeImage(Image *image) {
  if (image == NULL) {
    std::cout << "没有图像被打开！" << std::endl;
    return;
  }
  delete image;  // 释放图像对象指针
  image = NULL;  // 设置图像对象指针为空
  std::cout << "图像成功关闭！" << std::endl;
}

// 显示图像信息
void Utils::showInfo(Image *image) {
  if (image == NULL) {
    std::cout << "没有图像被打开！" << std::endl;
    return;
  }
  image->showInfo();
}

// 显示图像统计信息
void Utils::showStats(Image *image) {
  if (image == NULL) {
    std::cout << "没有图像被打开！" << std::endl;
    return;
  }
  image->showStats();
}

// 保存图像统计信息
void Utils::saveData(Image *image) {
  if (image == NULL) {
    std::cout << "没有图像被打开！" << std::endl;
    return;
  }

  std::string filename;
  std::cout << "请输入保存文件路径：";
  std::cin >> filename;

  image->saveData(filename);
}

// 显示图像直方图
void Utils::showHistogram(Image *image) {
  if (image == NULL) {
    std::cout << "没有图像被打开！" << std::endl;
    return;
  }
  image->showHistogram();
}

// 显示图像
void Utils::displayImage(Image *image) {
  if (image == NULL) {
    std::cout << "没有图像被打开！" << std::endl;
    return;
  }
  image->displayImage();
}

// 旋转图像
void Utils::rotateImage(Image *image) {
  if (image == NULL) {
    std::cout << "没有图像被打开！" << std::endl;
    return;
  }

  double angle;
  std::cout << "请输入旋转角度（以度为单位）：";
  std::cin >> angle;
  image->rotateImage(angle);
}

// 缩放图像
void Utils::zoomImage(Image *image) {
  if (image == NULL) {
    std::cout << "没有图像被打开！" << std::endl;
    return;
  }

  double ratio;
  std::cout << "请输入缩放比例：";
  std::cin >> ratio;
  image->zoomImage(ratio);
}

// 滤波图像
void Utils::filterImage(Image *image) {
  if (image == NULL) {
    std::cout << "没有图像被打开！" << std::endl;
    return;
  }

  int kernelSize;
  std::cout << "请输入滤波核大小（奇数）：";
  std::cin >> kernelSize;

  if (kernelSize % 2 != 1) {
    std::cout << "滤波核大小不正确！" << std::endl;
    return;
  }

  cv::Mat kernel(kernelSize, kernelSize, CV_32F);
  std::cout << "请输入滤波核矩阵（" << kernelSize << " * " << kernelSize
            << "）：" << std::endl;
  for (int i = 0; i < kernelSize; i++) {
    for (int j = 0; j < kernelSize; j++) {
      std::cin >> kernel.at<float>(i, j);
    }
  }

  image->filterImage(kernel);
}

// 保存图像为文件
void Utils::saveImage(Image *image) {
  if (image == NULL) {
    std::cout << "没有图像被打开！" << std::endl;
    return;
  }
  std::string path;
  std::cout << "请输入图像文件路径： ";
  std::cin >> path;
  image->saveImage(path);
}