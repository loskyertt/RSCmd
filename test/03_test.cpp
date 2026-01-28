#include <fstream>
#include <iomanip>
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <string>
#include <vector>

#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

/* 测试 1 */
void test01() {
  std::string path = "data/LinearDisp.png";
  cv::Mat data = cv::imread(path, cv::IMREAD_UNCHANGED);

  std::ofstream file1("data/test/03_basic_info.txt");
  // 如果路径非法或无权限，直接返回
  if (!file1.is_open()) {
    std::cout << "ERROR!\n";
    return;
  }

  // 1. 写入文件头部信息
  file1 << "总像素点：\t" << data.total() << "\n" << std::endl;

  // 统计每个波段的数据
  std::vector<cv::Mat> bands;
  cv::split(data, bands);

  file1 << "Basic Stats\tMin\tMax\t\tMean\t\t\tStdev" << std::endl;

  // 2. 写入基本统计信息
  for (int i = 0; i < data.channels(); ++i) {
    cv::Mat band = bands[i];

    // 计算统计数据
    double minVal, maxVal;
    cv::Mat meanVal, stdDev;

    minMaxLoc(band, &minVal, &maxVal);
    meanStdDev(band, meanVal, stdDev);

    file1 << "\tBand " << i << "\t " << minVal << "\t" << maxVal << "\t"
          << meanVal.at<double>(0, 0) << "\t" << stdDev.at<double>(0, 0)
          << std::endl;
  }

  uchar depth = CV_MAT_DEPTH(data.type());  // 提取深度部分
  if (static_cast<int>(depth) != CV_8U) {
    std::cout << "非 8-bit 灰度图像，退出！\\n";
    return;
  }

  std::ofstream file2("data/test/03_hist_info_1.txt");
  std::ofstream file3("data/test/03_hist_info_2.txt");
  // 如果路径非法或无权限，直接返回
  if (!file1.is_open() || !file3.is_open()) {
    std::cout << "ERROR!\n";
    return;
  }

  // 3. 写入直方图统计信息
  file2 << "Histogram,DN,Npts,Total,Percent,Acc Pct" << std::endl;
  file3 << "Histogram,灰度级,出现的概率" << std::endl;

  for (int i = 0; i < data.channels(); ++i) {
    cv::Mat band = bands[i];

    double minVal, maxVal;
    cv::minMaxLoc(band, &minVal, &maxVal);

    // 计算直方图
    int histSize = static_cast<int>(maxVal - minVal + 1);

    // calcHist，OpenCV 反人类的 API 设计 ......
    float range[] = {static_cast<float>(minVal),
                     static_cast<float>(maxVal + 1)};
    const float *histRange = {range};
    bool uniform = true, accumulate = false;
    cv::Mat hist;
    cv::calcHist(&band, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange,
                 uniform, accumulate);

    double total = 0.0;
    for (int j = 0; j < histSize; ++j) {
      double npts = hist.at<float>(j);  // 取出 hist 矩阵中第 j 行数据
      total += npts;

      double percent = (npts / data.total()) * 100.0;
      double accPct = (total / data.total()) * 100.0;

      // 写入直方图信息
      file2 << "Band " << i << "," << j << "," << npts << "," << total << ","
            << percent << "," << accPct << std::endl;

      // 写入灰度级数据
      file3 << "Band " << i << std::endl;
    }

    file2 << std::endl;
  }
}

/* 测试 2 */
void test02() {
  std::string path = "data/LinearDisp.png";
  cv::Mat data = cv::imread(path, cv::IMREAD_UNCHANGED);

  if (data.empty()) {
    std::cerr << "错误：内存中没有图像数据，无法保存统计信息！" << std::endl;
    return;
  }

  std::ofstream file("data/test/03_info.txt");
  if (!file.is_open()) {
    std::cerr << "错误：无法创建或打开文件！" << std::endl;
    return;
  }

  // 1. 写入文件头部信息
  file << "OpenCV Image Analysis Report" << std::endl;
  file << "Total Pixels: " << data.total() << " (" << data.cols << "x"
       << data.rows << ")" << std::endl;
  file << "Channels: " << data.channels() << std::endl << std::endl;

  // 2. 通道分离 (Bands/Channels)
  std::vector<cv::Mat> bands;
  cv::split(data, bands);

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
    double totalPixels = (double)data.total();

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
  std::cout << "成功：统计报告已保存完毕！ " << std::endl;
}

int main() {
  // savaInfo();
  test01();
}