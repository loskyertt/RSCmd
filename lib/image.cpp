#include "image.h"
#include <fstream>
#include <iostream>

// 默认构造函数
Image::Image()
    : m_path(""),
      m_rows(0),
      m_cols(0),
      m_bands(0),
      m_type(0),
      m_data(cv::Mat())  // 初始化矩阵为空
{}

// 构造函数：读取图像文件信息
Image::Image(std::string path) {
  m_path = path;
  m_data =
      cv::imread(path, cv::IMREAD_UNCHANGED);  // 读取图像文件（会保留 Alpha）

  if (m_data.empty()) {
    std::cout << path << " 路径不符！读取图像文件失败!" << std::endl;
    return;
  } else {
    std::cout << "读取图像文件成功!" << std::endl;
  }

  // 设置图像信息
  m_rows = m_data.rows;
  m_cols = m_data.cols;
  m_bands = m_data.channels();
  m_type = m_data.type();
}

// 析构函数
Image::~Image() {
  m_data.release();  // 释放图像数据矩阵占用的内存空间
}

// 获取图像文件路径
std::string Image::getPath() {
  return m_path;
}

// 获取图像行数
int Image::getRows() {
  return m_rows;
}

// 获取图像列数
int Image::getCols() {
  return m_cols;
}

// 获取图像波段数
int Image::getBands() {
  return m_bands;
}

// 获取图像数据类型
int Image::getType() {
  return m_type;
}

// 判断图像对象是否为空
bool Image::isEmpty() {
  return m_data.empty();  // 返回图像数据矩阵是否为空的布尔值
}

// 显示图像信息
void Image::showInfo() {
  if (isEmpty()) {
    std::cerr << "错误：内存中没有图像数据，无法保存统计信息！" << std::endl;
    return;
  }

  std::cout << "路径: " << getPath() << std::endl;
  std::cout << "行: " << getRows() << std::endl;
  std::cout << "列: " << getCols() << std::endl;
  std::cout << "波段: " << getBands() << std::endl;
  std::cout << "数据类型: " << getType() << " => ";

  uchar depth = CV_MAT_DEPTH(getType());

  switch (depth) {
    case CV_8U:
      std::cout << "8-bit 无符号整数";
      break;
    case CV_8S:
      std::cout << "8-bit 有符号整数";
      break;
    case CV_16U:
      std::cout << "16-bit 无符号整数";
      break;
    case CV_16S:
      std::cout << "16-bit 有符号整数";
      break;
    case CV_32S:
      std::cout << "32-bit 有符号整数";
      break;
    case CV_32F:
      std::cout << "32-bit 浮点数";
      break;
    case CV_64F:
      std::cout << "64-bit 浮点数";
      break;
    default:
      std::cout << "未知";
      break;
  }

  std::cout << "\n";

  // 检查图像的排列方式
  std::string arrangement;
  switch (getBands()) {
    case 1:
      arrangement = "灰度图像";
      break;
    case 3:
      // 检查颜色通道顺序
      if (getType() == CV_8UC3)
        arrangement = "BGR彩色图像";
      else if (getType() == CV_8UC3)
        arrangement = "RGB彩色图像";
      else
        arrangement = "彩色图像";
      break;
    case 4:
      arrangement = "带透明通道的图像";
      break;
    default:
      arrangement = "未知";
      break;
  }

  // 输出排列方式信息
  std::cout << "排列方式: " << arrangement << std::endl;
}

// 显示图像统计信息
void Image::showStats() {
  if (isEmpty()) {
    std::cout << "无法读取图像！" << std::endl;
    return;
  }

  std::vector<cv::Mat> bands;
  cv::split(m_data, bands);  // 通道分离

  for (int i = 0; i < getBands(); i++) {
    cv::Mat band = bands[i];

    // 计算统计数据
    double minVal, maxVal;
    cv::Mat meanVal, stddevVal;

    meanStdDev(band, meanVal, stddevVal);
    minMaxLoc(band, &minVal, &maxVal);

    std::cout << "通道类型：" << band.type() << std::endl;
    std::cout << "Min = " << minVal << std::endl;
    std::cout << "Max = " << maxVal << std::endl;
    std::cout << "Mean = " << meanVal << std::endl;
    std::cout << "Stdev = " << stddevVal << std::endl;
    std::cout << "---------------------------------------------------"
              << std::endl;
  }
}

// 保存图像统计信息
void Image::saveData(std::string filename) {
  if (isEmpty()) {
    std::cerr << "错误：内存中没有图像数据，无法保存统计信息！" << std::endl;
    return;
  }

  std::ofstream file(filename);
  if (!file.is_open()) {
    std::cerr << "错误：无法创建或打开文件：" << filename << std::endl;
    return;
  }

  // 1. 写入文件头部信息
  file << "OpenCV Image Analysis Report" << std::endl;
  file << "Total Pixels: " << m_data.total() << " (" << m_data.cols << "x"
       << m_data.rows << ")" << std::endl;
  file << "Bands: " << m_data.channels() << std::endl << std::endl;

  // 2. 通道分离 (Bands/Channels)
  std::vector<cv::Mat> bands;
  cv::split(m_data, bands);

  // 3. 统计并写入每个波段的数据
  for (int i = 0; i < (int)bands.size(); ++i) {
    cv::Mat band = bands[i];

    // --- 阶段 A: 基础统计 (Min, Max, Mean, Stdev) ---
    double minVal, maxVal;
    cv::Mat meanVal, stdDev;
    cv::minMaxLoc(band, &minVal, &maxVal);
    cv::meanStdDev(band, meanVal, stdDev);

    file << "---------- Band " << i + 1 << " Statistics ----------"
         << std::endl;
    file << "Min: " << minVal << "\tMax: " << maxVal << std::endl;
    file << "Mean: " << meanVal.at<double>(0)
         << "\tStdev: " << stdDev.at<double>(0) << std::endl
         << std::endl;

    // --- 阶段 B: 计算直方图 (统一 0-256 范围) ---
    // 我们固定 256 个桶，这样无论图片多暗，DN 值都能对应上真实的亮度
    int histSize = 256;
    float range[] = {0, 256};
    const float *histRange = {range};
    cv::Mat hist;

    cv::calcHist(&band, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange, true,
                 false);

    // --- 阶段 C: 写入详细的频率和占比信息 ---
    file << "DN\tNpts\t\tTotal\t\tPercent\t\tAcc_Pct" << std::endl;

    double runningTotal = 0.0;
    double totalPixels = (double)m_data.total();

    for (int j = 0; j < histSize; ++j) {
      float npts = hist.at<float>(j);  // 获取当前灰度级 j 的像素个数
      if (npts == 0 && j > maxVal)
        continue;  // 如果后面全是0则可以考虑跳过，或者全部打印

      runningTotal += npts;
      double percent = (npts / totalPixels) * 100.0;
      double accPct = (runningTotal / totalPixels) * 100.0;

      // 只有当该灰度级有像素或者是必经范围时才打印
      if (npts > 0 || (j >= minVal && j <= maxVal)) {
        file << j << "\t"                    // DN：真实的亮度值
             << (int)npts << "\t\t"          // Npts
             << (int)runningTotal << "\t\t"  // Total
             << std::fixed << std::setprecision(4) << percent
             << "%\t"                        // Percent
             << accPct << "%" << std::endl;  // Acc_Pct
      }
    }
    file << std::endl << std::endl;
  }

  file.close();
  std::cout << "成功！统计报告已保存至：" << filename << std::endl;
}

// 显示图像直方图
void Image::showHistogram() {
  if (m_data.empty()) {
    std::cout << "无法读取图像：" << getPath() << std::endl;
    return;
  }

  // 分割图像通道
  std::vector<cv::Mat> channels;
  split(m_data, channels);

  // 创建直方图
  int histSize = 256;        // 灰度级的数量
  float range[] = {0, 256};  // 灰度级的范围
  const float *histRange = {range};
  bool uniform = true, accumulate = false;

  // 显示每个波段的直方图
  for (int i = 0; i < channels.size(); ++i) {
    // 计算直方图
    cv::Mat hist;
    calcHist(&channels[i], 1, 0, cv::Mat(), hist, 1, &histSize, &histRange,
             uniform, accumulate);

    // 创建直方图图像
    int histWidth = 512, histHeight = 400;
    int binWidth = cvRound((double)histWidth / histSize);
    cv::Mat histImage(histHeight, histWidth, CV_8UC3, cv::Scalar(0, 0, 0));

    // 归一化直方图值
    normalize(hist, hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

    // 绘制直方图
    for (int j = 1; j < histSize; ++j) {
      line(histImage,
           cv::Point(binWidth * (j - 1),
                     histHeight - cvRound(hist.at<float>(j - 1))),
           cv::Point(binWidth * j, histHeight - cvRound(hist.at<float>(j))),
           cv::Scalar(255, 255, 255), 2, 8, 0);
    }

    // 显示直方图
    imshow("Histogram of Band " + std::to_string(i + 1), histImage);
  }

  cv::waitKey(0);
}

// 显示图像
void Image::displayImage() {
  if (m_data.empty()) {
    std::cout << "无法读取图像：" << getPath() << std::endl;
    return;
  }

  std::cout << "The image has " << getBands() << " bands, " << getRows()
            << " rows and " << getCols() << " columns." << std::endl;

  int displayType;
  char stretchType;
  int redBand, greenBand, blueBand;

  int bandNum;

  std::cout
      << "Input Display Parameters ( Display Type, Stretch Type, Red, Green, "
         "Blue )"
      << std::endl;
  std::cout << "Input Display Type 0-Gray or 1-Color : ";
  std::cin >> displayType;

  if (displayType != 0 && displayType != 1) {
    std::cout << "Error: Invalid display type." << std::endl;
    return;
  }

  std::cout << "Input L-Linear stretch, Others - default is Normal : ";
  std::cin >> stretchType;

  if (displayType == 0) {
    bandNum = 1;
  } else {
    bandNum = 3;
  }

  std::cout << "Input " << bandNum << " bands in base 0 : ";

  int bandArray[3];

  for (int i = 0; i < bandNum; i++) {
    std::cin >> bandArray[i];
  }

  redBand = bandArray[0];
  greenBand = bandArray[1];
  blueBand = bandArray[2];

  if (std::cin.fail() || std::cin.peek() != '\n') {
    std::cout << "Error: Invalid number of bands." << std::endl;
    return;
  }

  if (redBand < 0 || redBand >= m_bands || greenBand < 0 ||
      greenBand >= m_bands || blueBand < 0 || blueBand >= m_bands) {
    std::cout << "Error: Invalid band number." << std::endl;
    return;
  }

  cv::Mat displayImg;

  if (displayType == 0) {
    std::vector<cv::Mat> channels(m_bands);
    split(m_data, channels);
    displayImg = channels[redBand];
  }

  else {
    std::vector<cv::Mat> channels(m_bands);
    split(m_data, channels);
    std::vector<cv::Mat> rgbChannels(3);
    rgbChannels[0] = channels[blueBand];
    rgbChannels[1] = channels[greenBand];
    rgbChannels[2] = channels[redBand];
    merge(rgbChannels, displayImg);

    cvtColor(displayImg, displayImg, cv::COLOR_BGR2RGB);

    displayImg.convertTo(displayImg, CV_8U, 1.0 / 256.0);

    cvtColor(displayImg, displayImg, cv::COLOR_RGB2HSV);

    std::vector<cv::Mat> hsvChannels(3);
    split(displayImg, hsvChannels);

    if (stretchType == 'L' || stretchType == 'l') {
      double minVal, maxVal;
      minMaxLoc(hsvChannels[2], &minVal, &maxVal);

      hsvChannels[2] = (hsvChannels[2] - minVal) * 255.0 / (maxVal - minVal);
    }

    merge(hsvChannels, displayImg);

    cvtColor(displayImg, displayImg, cv::COLOR_HSV2RGB);
  }

  namedWindow("Display Image", cv::WINDOW_AUTOSIZE);
  imshow("Display Image", displayImg);
  cv::waitKey(0);
  cv::destroyWindow("Display Image");

  char updateType;
  std::cout << "你想更新图像信息吗？ (Y/N) : ";
  std::cin >> updateType;

  if (updateType == 'Y' || updateType == 'y') {
    m_data = displayImg;
    std::cout << "图像信息已被更新！" << std::endl;
  }

  else if (updateType == 'N' || updateType == 'n') {
    std::cout << "图像信息未被更新！" << std::endl;
  }

  else {
    std::cout << "无效的输入！图像信息未被更新！" << std::endl;
    return;
  }
}

// 旋转图像
void Image::rotateImage(double angle) {
  if (m_data.empty()) {
    std::cout << "没有图像被打开！" << std::endl;
    return;
  }

  cv::Point2f center(getCols() / 2.0, getRows() / 2.0);
  cv::Mat rotMat = cv::getRotationMatrix2D(center, angle, 1.0);

  cv::Mat rotatedImage;
  warpAffine(m_data, rotatedImage, rotMat,
             m_data.size());  // 对图像进行仿射变换

  imshow("旋转后的图像", rotatedImage);
  cv::waitKey(0);
  cv::destroyWindow("旋转后的图像");

  // 是否更新图像信息
  char updateType;
  std::cout << "你想要更新图像信息吗？(Y/N) : ";
  std::cin >> updateType;

  if (updateType == 'Y' || updateType == 'y') {
    m_data = rotatedImage;  // 更新图像数据矩阵
    std::cout << "图像信息已被更新！" << std::endl;
  }

  else if (updateType == 'N' || updateType == 'n') {
    std::cout << "图像信息未被更新" << std::endl;
  }

  else {
    std::cout << "无效的输入！图像信息未被更新！" << std::endl;
    return;
  }
}

// 缩放图像
void Image::zoomImage(double ratio) {
  if (m_data.empty()) {
    std::cout << "没有图像被打开！" << std::endl;
    return;
  }

  int newWidth = static_cast<int>(getCols() * ratio);
  int newHeight = static_cast<int>(getRows() * ratio);

  cv::Mat zoomedImage;
  resize(m_data, zoomedImage, cv::Size(newWidth, newHeight));

  imshow("缩放后的图像", zoomedImage);
  cv::waitKey(0);
  cv::destroyWindow("缩放后的图像");

  // 是否更新图像信息
  char updateType;
  std::cout << "你想要更新图像信息吗？(Y/N) : ";
  std::cin >> updateType;

  if (updateType == 'Y' || updateType == 'y') {
    m_data = zoomedImage;  // 更新矩阵
    m_rows = m_data.rows;
    m_cols = m_data.cols;
    std::cout << "图像信息已被更新！" << std::endl;
  }

  else if (updateType == 'N' || updateType == 'n') {
    std::cout << "图像信息未被更新" << std::endl;
  }

  else {
    std::cout << "无效的输入！图像信息未被更新！" << std::endl;
    return;
  }
}

// 滤波图像
void Image::filterImage(cv::Mat kernel) {
  if (m_data.empty()) {
    std::cout << "没有图像被打开！" << std::endl;
    return;
  }

  if (kernel.empty() || kernel.rows % 2 == 0 || kernel.cols % 2 == 0) {
    std::cout << "滤波核大小不正确" << std::endl;
    return;
  }

  cv::Mat filteredImage;
  filter2D(m_data, filteredImage, -1, kernel);

  imshow("滤波后的图像", filteredImage);
  cv::waitKey(0);
  cv::destroyWindow("滤波后的图像");

  // 是否更新图像信息
  char updateType;
  std::cout << "你想要更新图像信息吗？(Y/N) : ";
  std::cin >> updateType;

  if (updateType == 'Y' || updateType == 'y') {
    m_data = filteredImage;
    std::cout << "图像信息已被更新！" << std::endl;
  }

  else if (updateType == 'N' || updateType == 'n') {
    std::cout << "图像信息未被更新！" << std::endl;
  }

  else {
    std::cout << "无效的输入！图像信息未被更新！" << std::endl;
    return;
  }
}

// 保存图像为文件
void Image::saveImage(std::string path) {
  if (m_data.empty()) {
    std::cout << "没有图像被打开！" << std::endl;
    return;
  }

  bool result = imwrite(path, m_data);  // 写入图像文件，返回结果
  if (result) {
    std::cout << "保存图像成功！" << path << std::endl;
  } else {
    std::cout << "保存图像失败！ " << path << std::endl;
  }
}