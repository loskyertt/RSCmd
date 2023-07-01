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

// 保存图像统计信息
void Image::saveData(string filename)
{
    if (data.empty())
    {
        cout << "无法读取图像：" << getPath() << endl;
        return;
    }
    // 打开文件
    ofstream file(filename);
    if (!file.is_open())
    {
        cout << "无法打开文件：" << filename << endl;
        return;
    }

    // 写入文件头部信息
    file << "Filename: " << filename << endl;
    file << "Dims: Full Scene (" << data.total() << " points)" << endl
         << endl;

    // 统计每个波段的数据
    vector<Mat> bands;
    split(data, bands);

    file << "Basic Stats\tMin\tMax\t\tMean\t\t\tStdev" << endl;
    for (int i = 0; i < bands.size(); ++i)
    {
        Mat band = bands[i];

        // 计算统计数据
        double minVal, maxVal;
        Mat meanVal, stdDev;
        minMaxLoc(band, &minVal, &maxVal);
        meanStdDev(band, meanVal, stdDev);

        // 写入基本统计信息
        file << "\tBand " << i + 1 << "\t " << minVal << "\t" << maxVal << "\t" << meanVal << "\t" << stdDev << endl;
    }

    for (int i = 0; i < bands.size(); ++i)
    {
        Mat band = bands[i];

        // 获取波段的最小值和最大值
        double minVal, maxVal;
        minMaxLoc(band, &minVal, &maxVal);

        // 计算直方图
        int histSize = static_cast<int>(maxVal - minVal + 1);
        float range[] = {static_cast<float>(minVal), static_cast<float>(maxVal + 1)};
        const float *histRange = {range};
        bool uniform = true, accumulate = false;

        Mat hist;
        calcHist(&band, 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);

        // 写入直方图数据
        file << endl;
        file << "Histogram\tDN\tNpts\tTotal\tPercent\tAcc Pct" << endl;

        double total = 0.0;
        for (int j = 0; j < histSize; ++j)
        {
            double npts = hist.at<float>(j);
            total += npts;

            double percent = (npts / data.total()) * 100.0;
            double accPct = (total / data.total()) * 100.0;

            file << "Band " << i + 1 << "\t\t" << j << "\t" << npts << "\t" << total << "\t" << percent << "\t" << accPct << endl;
        }

        file << endl;
    }

    for (int i = 0; i < bands.size(); ++i)
    {
        Mat band = bands[i];

        // 写入波段信息
        file << "Band " << i + 1 << ":\n";

        // 统计灰度级数据
        map<int, int> hist;
        for (int y = 0; y < band.rows; ++y)
        {
            for (int x = 0; x < band.cols; ++x)
            {
                int value = band.at<uchar>(y, x);
                hist[value]++;
            }
        }

        // 计算总像素数
        int totalPixels = band.rows * band.cols;

        // 写入灰度级和概率
        for (auto it = hist.begin(); it != hist.end(); ++it)
        {
            int grayLevel = it->first;
            int count = it->second;
            double probability = static_cast<double>(count) / totalPixels;

            file << "灰度级: " << grayLevel << "\t";
            file << "出现的概率: " << probability << endl;
        }

        file << endl;
    }

    // 关闭文件
    file.close();

    cout << "图像数据已保存到文件：" << filename << endl;
}

// 显示图像直方图
void Image::showHistogram()
{
    if (data.empty())
    {
        cout << "无法读取图像：" << getPath() << endl;
        return;
    }

    // 分割图像通道
    vector<Mat> channels;
    split(data, channels);

    // 创建直方图
    int histSize = 256;       // 灰度级的数量
    float range[] = {0, 256}; // 灰度级的范围
    const float *histRange = {range};
    bool uniform = true, accumulate = false;

    // 显示每个波段的直方图
    for (int i = 0; i < channels.size(); ++i)
    {
        // 计算直方图
        Mat hist;
        calcHist(&channels[i], 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);

        // 创建直方图图像
        int histWidth = 512, histHeight = 400;
        int binWidth = cvRound((double)histWidth / histSize);
        Mat histImage(histHeight, histWidth, CV_8UC3, Scalar(0, 0, 0));

        // 归一化直方图值
        normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

        // 绘制直方图
        for (int j = 1; j < histSize; ++j)
        {
            line(histImage, Point(binWidth * (j - 1), histHeight - cvRound(hist.at<float>(j - 1))),
                 Point(binWidth * j, histHeight - cvRound(hist.at<float>(j))),
                 Scalar(255, 255, 255), 2, 8, 0);
        }

        // 显示直方图
        imshow("Histogram of Band " + to_string(i + 1), histImage);
    }

    waitKey(0);
}

// 显示图像
void Image::displayImage()
{
    if (data.empty())
    {
        cout << "无法读取图像：" << getPath() << endl;
        return;
    }

    cout << "The image has " << getBands() << " bands, " << getRows() << " rows and " << getCols() << " columns." << endl;

    int displayType;
    char stretchType;
    int redBand, greenBand, blueBand;

    int bandNum;

    cout << "Input Display Parameters ( Display Type, Stretch Type, Red, Green, Blue )" << endl;
    cout << "Input Display Type 0-Gray or 1-Color : ";
    cin >> displayType;

    if (displayType != 0 && displayType != 1)
    {
        cout << "Error: Invalid display type." << endl;
        return;
    }

    cout << "Input L-Linear stretch, Others - default is Normal : ";
    cin >> stretchType;

    if (displayType == 0)
    {
        bandNum = 1;
    }
    else
    {
        bandNum = 3;
    }

    cout << "Input " << bandNum << " bands in base 0 : ";

    int bandArray[3];

    for (int i = 0; i < bandNum; i++)
    {
        cin >> bandArray[i];
    }

    redBand = bandArray[0];
    greenBand = bandArray[1];
    blueBand = bandArray[2];

    if (cin.fail() || cin.peek() != '\n')
    {
        cout << "Error: Invalid number of bands." << endl;
        return;
    }

    if (redBand < 0 || redBand >= bands || greenBand < 0 || greenBand >= bands || blueBand < 0 || blueBand >= bands)
    {
        cout << "Error: Invalid band number." << endl;
        return;
    }

    Mat displayImg;

    if (displayType == 0)
    {
        vector<Mat> channels(bands);
        split(data, channels);
        displayImg = channels[redBand];
    }

    else
    {
        vector<Mat> channels(bands);
        split(data, channels);
        vector<Mat> rgbChannels(3);
        rgbChannels[0] = channels[blueBand];
        rgbChannels[1] = channels[greenBand];
        rgbChannels[2] = channels[redBand];
        merge(rgbChannels, displayImg);

        cvtColor(displayImg, displayImg, COLOR_BGR2RGB);

        displayImg.convertTo(displayImg, CV_8U, 1.0 / 256.0);

        cvtColor(displayImg, displayImg, COLOR_RGB2HSV);

        vector<Mat> hsvChannels(3);
        split(displayImg, hsvChannels);

        if (stretchType == 'L' || stretchType == 'l')
        {
            double minVal, maxVal;
            minMaxLoc(hsvChannels[2], &minVal, &maxVal);

            hsvChannels[2] = (hsvChannels[2] - minVal) * 255.0 / (maxVal - minVal);
        }

        merge(hsvChannels, displayImg);

        cvtColor(displayImg, displayImg, COLOR_HSV2RGB);
    }

    namedWindow("Display Image", WINDOW_AUTOSIZE);
    imshow("Display Image", displayImg);
    waitKey(0);
    destroyWindow("Display Image");

    char updateType;
    cout << "你想更新图像信息吗？ (Y/N) : ";
    cin >> updateType;

    if (updateType == 'Y' || updateType == 'y')
    {
        data = displayImg;
        cout << "图像信息已被更新！" << endl;
    }

    else if (updateType == 'N' || updateType == 'n')
    {
        cout << "图像信息未被更新！" << endl;
    }

    else
    {
        cout << "无效的输入！图像信息未被更新！" << endl;
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

    imshow("旋转后的图像", rotatedImage);
    waitKey(0);
    destroyWindow("旋转后的图像");

    // 是否更新图像信息
    char updateType;
    cout << "你想要更新图像信息吗？(Y/N) : ";
    cin >> updateType;

    if (updateType == 'Y' || updateType == 'y')
    {
        data = rotatedImage; // 更新图像数据矩阵
        cout << "图像信息已被更新！" << endl;
    }

    else if (updateType == 'N' || updateType == 'n')
    {
        cout << "图像信息未被更新" << endl;
    }

    else
    {
        cout << "无效的输入！图像信息未被更新！" << endl;
        return;
    }
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

    imshow("缩放后的图像", zoomedImage);
    waitKey(0);
    destroyWindow("缩放后的图像");

    // 是否更新图像信息
    char updateType;
    cout << "你想要更新图像信息吗？(Y/N) : ";
    cin >> updateType;

    if (updateType == 'Y' || updateType == 'y')
    {
        data = zoomedImage; // 更新矩阵
        rows = data.rows;
        cols = data.cols;
        cout << "图像信息已被更新！" << endl;
    }

    else if (updateType == 'N' || updateType == 'n')
    {
        cout << "图像信息未被更新" << endl;
    }

    else
    {
        cout << "无效的输入！图像信息未被更新！" << endl;
        return;
    }
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
    destroyWindow("滤波后的图像");

    // 是否更新图像信息
    char updateType;
    cout << "你想要更新图像信息吗？(Y/N) : ";
    cin >> updateType;

    if (updateType == 'Y' || updateType == 'y')
    {
        data = filteredImage;
        cout << "图像信息已被更新！" << endl;
    }

    else if (updateType == 'N' || updateType == 'n')
    {
        cout << "图像信息未被更新！" << endl;
    }

    else
    {
        cout << "无效的输入！图像信息未被更新！" << endl;
        return;
    }
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