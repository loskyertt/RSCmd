#include <iostream>
#include <string>
#include <vector>

#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

/* 测试 1 */
void test01() {
  std::string path = "data/LinearDisp.png";
  cv::Mat data = cv::imread(path, cv::IMREAD_UNCHANGED);

  std::vector<cv::Mat> channels;
  cv::split(data, channels);

  std::cout << "拆分后第一个通道的类型: " << channels[0].type() << std::endl;
  std::cout << "拆分后通道数量: " << channels.size() << std::endl;

  // 1. 把蓝色通道变暗（所有值除以 2）
  channels[0] = channels[0] / 2;

  // 2. 把红色通道变亮（所有值设为 255）
  channels[2] = cv::Scalar(255);

  // 3. 把这四个“灰色”的面包片叠回去
  cv::Mat merged_data;
  cv::merge(channels, merged_data);

  // 4. 显示合并后的图像
  cv::imshow("Modified Image", merged_data);
  cv::waitKey(0);
}

int main() {
  test01();
}