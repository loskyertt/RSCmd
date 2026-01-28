#include <iostream>
#include <string>
#include <vector>

#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

struct ImageInfo {
  int depth;  // 提取深度部分
  int chans;  // 提取通道部分

  ImageInfo(int type) : depth(CV_MAT_DEPTH(type)), chans(CV_MAT_CN(type)) {}
};

/* 测试 1 */
void test01() {
  std::string path = "data/LinearDisp.png";
  cv::Mat data = cv::imread(path, cv::IMREAD_UNCHANGED);

  int type = data.type();

  std::cout << "type : " << type << "\n";

  uchar depth = type & CV_MAT_DEPTH_MASK;  // 提取深度部分
  int chans = 1 + (type >> CV_CN_SHIFT);   // 提取通道部分

  std::cout << "Depth: " << (int)depth << " (Match the macros above)"
            << std::endl;
  std::cout << "Channels: " << chans << std::endl;
}

/* 测试 2 */
void test02() {
  std::string path = "data/LinearDisp.png";
  cv::Mat data = cv::imread(path, cv::IMREAD_UNCHANGED);

  ImageInfo info(data.type());

  std::cout << "Depth: " << info.depth << std::endl;
  std::cout << "Channels: " << info.chans << std::endl;
}

/* 测试 3 */
void test_03() {
  // 初始化矩阵为空
  cv::Mat data = cv::Mat();

  int rows = data.rows;
  int cols = data.cols;

  std::cout << "rows: " << rows << std::endl;
  std::cout << "cols: " << cols << std::endl;
}

/* 测试 4 */
void test04() {
  std::string path = "data/LinearDisp.png";
  cv::Mat data = cv::imread(path, cv::IMREAD_UNCHANGED);

  std::vector<cv::Mat> channels;
  cv::split(data, channels);

  std::cout << "拆分后第一个通道的类型: " << channels[0].type() << std::endl;
  std::cout << "拆分后通道数量: " << channels.size() << std::endl;

  // std::cout << "bands: " << data.channels() << std::endl;

  cv::imshow("Channel 0", channels[0]);  // Blue
  cv::imshow("Channel 1", channels[1]);  // Green
  cv::imshow("Channel 2", channels[2]);  //Red
  cv::imshow("Channel 3", channels[3]);  // Alpha
  cv::waitKey(0);                        // 必须加上这一句，窗口才不会闪退
}

int main() {
  // test01();
  // test02();
  // test_03();
  test04();
}