#include "../include/Image.h"

// 构造函数
Image::Image()
{
    path = "";    // 初始化图像文件路径为空
    rows = 0;     // 初始化图像行数为0
    cols = 0;     // 初始化图像列数为0
    bands = 0;    // 初始化图像波段数为0
    type = 0;     // 初始化图像数据类型为0
    data = Mat(); // 初始化图像数据矩阵为空
}

// 带参数的构造函数，根据路径读取图像文件
Image::Image(string path)
{
    this->path = path;                     // 设置图像文件路径
    data = imread(path, IMREAD_UNCHANGED); // 读取图像文件，保持原始格式
    if (data.empty())
    {
        cout << "读取图像文件失败!" << path << endl;
        return;
    }
    else
    {
        cout << "读取图像文件成功!" << endl;
    }
    rows = data.rows;        // 设置图像行数
    cols = data.cols;        // 设置图像列数
    bands = data.channels(); // 设置图像波段数
    type = data.type();      // 设置图像数据类型
}

// 析构函数
Image::~Image()
{
    data.release(); // 释放图像数据矩阵占用的内存空间
}

// 获取图像文件路径的方法
string Image::getPath()
{
    return path;
}

// 获取图像行数的方法
int Image::getRows()
{
    return rows;
}

// 获取图像列数的方法
int Image::getCols()
{
    return cols;
}

// 获取图像波段数的方法
int Image::getBands()
{
    return bands;
}

// 获取图像数据类型的方法
int Image::getType()
{
    return type;
}

// 判断图像对象是否为空
bool Image::isEmpty()
{
    return data.empty(); // 返回图像数据矩阵是否为空的布尔值
}

// 显示图像信息
void Image::showInfo()
{
    if (data.empty())
    {
        cout << "无法读取图像：" << getPath() << endl;
        return;
    }

    cout << "路径: " << getPath() << endl;
    cout << "行: " << getRows() << endl;
    cout << "列: " << getCols() << endl;
    cout << "波段: " << getBands() << endl;
    cout << "数据类型: " << getType() << " => ";
    switch (getType())
    {
    case CV_8U:
        cout << "8-bit 无符号整数";
        break;
    case CV_8S:
        cout << "8-bit 有符号整数";
        break;
    case CV_16U:
        cout << "16-bit 无符号整数";
        break;
    case CV_16S:
        cout << "16-bit 有符号整数";
        break;
    case CV_32S:
        cout << "32-bit 有符号整数";
        break;
    case CV_32F:
        cout << "32-bit 浮点数";
        break;
    case CV_64F:
        cout << "64-bit 浮点数";
        break;
    default:
        cout << "未知";
        break;
    }
    cout << endl;
    // 检查图像的排列方式
    string arrangement;
    switch (getBands())
    {
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
    cout << "排列方式: " << arrangement << endl;
}

// 显示图像统计信息
void Image::showStats()
{
    if (data.empty())
    {
        cout << "无法读取图像：" << getPath() << endl;
        return;
    }

    vector<Mat> channels;
    split(data, channels);

    for (int i = 0; i < getBands(); i++)
    {
        double minVal, maxVal;
        Mat meanVal, stddevVal;

        meanStdDev(channels[i], meanVal, stddevVal);
        minMaxLoc(channels[i], &minVal, &maxVal);

        cout << "Statics of Band " << i + 1 << endl;
        cout << "Min = " << minVal << endl;
        cout << "Max = " << maxVal << endl;
        cout << "Mean = " << meanVal << endl;
        cout << "Stdev = " << stddevVal << endl;
        cout << "---------------------------------------------------" << endl;
    }
}

// 显示图像直方图
void Image::showHistogram()
{
    if (data.empty())
    {
        cout << "无法读取图像：" << getPath() << endl;
        return;
    }

    if (getBands() != 1)
    {
        cout << "图像不是灰度图像，正在转换为灰度图像..." << endl;
        cvtColor(data, data, COLOR_BGR2GRAY);
        cout << "转换成功！" << endl;
        imshow("灰度图像", data);
        waitKey(0);

        destroyWindow("灰度图像");
    }

    int histSize = 256;       // 灰度级数量
    float range[] = {0, 256}; // 灰度级范围
    const float *histRange = {range};
    bool uniform = true, accumulate = false;

    Mat histogram;

    // 计算直方图
    calcHist(&data, 1, 0, Mat(), histogram, 1, &histSize, &histRange, uniform, accumulate);

    // 创建画布
    int canvasWidth = 512, canvasHeight = 400;
    int binWidth = cvRound((double)canvasWidth / histSize);
    Mat canvas(canvasHeight, canvasWidth, CV_8UC3, Scalar(0, 0, 0));

    // 归一化直方图数据
    normalize(histogram, histogram, 0, canvasHeight, NORM_MINMAX, -1, Mat());

    // 绘制直方图
    for (int i = 0; i < histSize; i++)
    {
        rectangle(canvas, Point(binWidth * i, canvasHeight), Point(binWidth * (i + 1), canvasHeight - cvRound(histogram.at<float>(i))), Scalar(255, 255, 255), -1);
    }

    // 显示直方图
    imshow("灰度直方图", canvas);
    waitKey(0);

    // 保存直方图到当前文件夹
    string histPath = "./image/histogram.jpg";
    imwrite(histPath, canvas);
    cout << "直方图已保存为: " << histPath << endl;
}

// 显示图像
void Image::displayImage()
{
    if (data.empty())
    {
        cout << "没有图像被打开！" << endl;
        return;
    }

    // 打印图像的一些基本信息
    cout << "The image has " << getBands() << " bands, " << getRows() << " rows and " << getCols() << " columns." << endl;

    int displayType;
    char stretchType;
    int redBand, greenBand, blueBand;

    cout << "Input Display Parameters ( Display Type, Stretch Type, Red, Green, Blue )" << endl;
    cout << "Input Display Type 0-Gray or 1-Color : ";
    cin >> displayType;

    if (displayType != 0 && displayType != 1)
    {
        cout << "无效的显示类型！" << endl;
        return;
    }

    cout << "Input L-Linear stretch, Others - default is Normal : ";
    cin >> stretchType;

    cout << "Input RGB bands or gray Band in base 0 " << endl;
    cin >> redBand >> greenBand >> blueBand;

    if (redBand < 0 || redBand >= bands || greenBand < 0 || greenBand >= bands || blueBand < 0 || blueBand >= bands)
    {
        cout << "无效的波段数！" << endl;
        return;
    }

    // 声明一个变量以存储显示的图像
    Mat displayImg;

    // 如果显示类型为灰色，则从图像中提取灰色带并将其分配给显示图像
    if (displayType == 0)
    {
        vector<Mat> channels(bands);
        split(data, channels);
        displayImg = channels[redBand];
    }

    // 如果显示类型为彩色，则从图像中提取RGB波段并将其合并为彩色图像
    else
    {
        vector<Mat> channels(bands);
        split(data, channels);
        vector<Mat> rgbChannels(3);
        rgbChannels[0] = channels[blueBand];
        rgbChannels[1] = channels[greenBand];
        rgbChannels[2] = channels[redBand];
        merge(rgbChannels, displayImg);

        // 将彩色图像从BGR转换为RGB格式
        cvtColor(displayImg, displayImg, COLOR_BGR2RGB);

        // 将彩色图像从16位格式转换为8位格式
        displayImg.convertTo(displayImg, CV_8U, 1.0 / 256.0);

        // 将彩色图像从RGB转换为HSV格式，以便更容易地操作亮度和对比度
        cvtColor(displayImg, displayImg, COLOR_RGB2HSV);

        // 将HSV图像拆分为三个通道：色调、饱和度和值
        vector<Mat> hsvChannels(3);
        split(displayImg, hsvChannels);

        if (stretchType == 'L' || stretchType == 'l')
        {
            // 在值通道中查找最小值和最大值
            double minVal, maxVal;
            minMaxLoc(hsvChannels[2], &minVal, &maxVal);

            // 对值通道应用线性拉伸
            hsvChannels[2] = (hsvChannels[2] - minVal) * 255.0 / (maxVal - minVal);
        }

        // 将HSV通道合并回HSV图像
        merge(hsvChannels, displayImg);

        // 将HSV图像转换回RGB格式以进行显示
        cvtColor(displayImg, displayImg, COLOR_HSV2RGB);
    }

    cout << "图像显示成功！按“ESC”键退出" << endl;

    namedWindow("Display Image", WINDOW_AUTOSIZE);
    imshow("Display Image", displayImg);
    waitKey(0);
    destroyWindow("Display Image");

    // 是否更新图像信息
    char updateType;
    cout << "你想要更新图像信息吗？(Y/N) : ";
    cin >> updateType;

    if (updateType == 'Y' || updateType == 'y')
    {
        data = displayImg;
        cout << "图像信息已被更新！" << endl;
    }

    else if (updateType == 'N' || updateType == 'n')
    {
        cout << "图像信息未被更新" << endl;
    }

    else
    {
        cout << "无效的输入" << endl;
        return;
    }
}

// 旋转图像
void Image::rotateImage(double angle)
{
    if (data.empty())
    {
        cout << "没有图像被打开！" << endl;
        return;
    }

    Point2f center(getCols() / 2.0, getRows() / 2.0);
    Mat rotMat = getRotationMatrix2D(center, angle, 1.0);

    Mat rotatedImage;
    warpAffine(data, rotatedImage, rotMat, data.size()); // 对图像进行仿射变换

    data = rotatedImage; // 更新图像数据矩阵

    imshow("旋转后的图像", rotatedImage);
    waitKey(0);
}

// 缩放图像
void Image::zoomImage(double ratio)
{
    if (data.empty())
    {
        cout << "没有图像被打开！" << endl;
        return;
    }

    int newWidth = static_cast<int>(getCols() * ratio);
    int newHeight = static_cast<int>(getRows() * ratio);

    Mat zoomedImage;
    resize(data, zoomedImage, Size(newWidth, newHeight));

    data = zoomedImage; // 更新矩阵
    rows = data.rows;
    cols = data.cols;

    imshow("缩放后的图像", zoomedImage);
    waitKey(0);
}

// 滤波图像
void Image::filterImage(Mat kernel)
{
    if (data.empty())
    {
        cout << "没有图像被打开！" << endl;
        return;
    }

    if (kernel.empty() || kernel.rows % 2 == 0 || kernel.cols % 2 == 0)
    {
        cout << "滤波核大小不正确" << endl;
        return;
    }

    Mat filteredImage;
    filter2D(data, filteredImage, -1, kernel);

    imshow("滤波后的图像", filteredImage);
    waitKey(0);

    // 更新图像信息
    data = filteredImage;
}

// 保存图像为文件
void Image::saveImage(string path)
{
    if (data.empty())
    {
        cout << "没有图像被打开！" << endl;
        return;
    }

    bool result = imwrite(path, data); // 写入图像文件，返回结果
    if (result)
    {
        cout << "保存图像成功！" << path << endl;
    }
    else
    {
        cout << "保存图像失败！ " << path << endl;
    }
}