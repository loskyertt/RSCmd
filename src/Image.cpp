#include "../include/Image.h"

// ���캯��
Image::Image()
{
    path = "";    // ��ʼ��ͼ���ļ�·��Ϊ��
    rows = 0;     // ��ʼ��ͼ������Ϊ0
    cols = 0;     // ��ʼ��ͼ������Ϊ0
    bands = 0;    // ��ʼ��ͼ�񲨶���Ϊ0
    type = 0;     // ��ʼ��ͼ����������Ϊ0
    data = Mat(); // ��ʼ��ͼ�����ݾ���Ϊ��
}

// �������Ĺ��캯��������·����ȡͼ���ļ�
Image::Image(string path)
{
    this->path = path;                     // ����ͼ���ļ�·��
    data = imread(path, IMREAD_UNCHANGED); // ��ȡͼ���ļ�������ԭʼ��ʽ
    if (data.empty())
    {
        cout << "��ȡͼ���ļ�ʧ��!" << path << endl;
        return;
    }
    else
    {
        cout << "��ȡͼ���ļ��ɹ�!" << endl;
    }
    rows = data.rows;        // ����ͼ������
    cols = data.cols;        // ����ͼ������
    bands = data.channels(); // ����ͼ�񲨶���
    type = data.type();      // ����ͼ����������
}

// ��������
Image::~Image()
{
    data.release(); // �ͷ�ͼ�����ݾ���ռ�õ��ڴ�ռ�
}

// ��ȡͼ���ļ�·���ķ���
string Image::getPath()
{
    return path;
}

// ��ȡͼ�������ķ���
int Image::getRows()
{
    return rows;
}

// ��ȡͼ�������ķ���
int Image::getCols()
{
    return cols;
}

// ��ȡͼ�񲨶����ķ���
int Image::getBands()
{
    return bands;
}

// ��ȡͼ���������͵ķ���
int Image::getType()
{
    return type;
}

// �ж�ͼ������Ƿ�Ϊ��
bool Image::isEmpty()
{
    return data.empty(); // ����ͼ�����ݾ����Ƿ�Ϊ�յĲ���ֵ
}

// ��ʾͼ����Ϣ
void Image::showInfo()
{
    if (data.empty())
    {
        cout << "�޷���ȡͼ��" << getPath() << endl;
        return;
    }

    cout << "·��: " << getPath() << endl;
    cout << "��: " << getRows() << endl;
    cout << "��: " << getCols() << endl;
    cout << "����: " << getBands() << endl;
    cout << "��������: " << getType() << " => ";
    switch (getType())
    {
    case CV_8U:
        cout << "8-bit �޷�������";
        break;
    case CV_8S:
        cout << "8-bit �з�������";
        break;
    case CV_16U:
        cout << "16-bit �޷�������";
        break;
    case CV_16S:
        cout << "16-bit �з�������";
        break;
    case CV_32S:
        cout << "32-bit �з�������";
        break;
    case CV_32F:
        cout << "32-bit ������";
        break;
    case CV_64F:
        cout << "64-bit ������";
        break;
    default:
        cout << "δ֪";
        break;
    }
    cout << endl;
    // ���ͼ������з�ʽ
    string arrangement;
    switch (getBands())
    {
    case 1:
        arrangement = "�Ҷ�ͼ��";
        break;
    case 3:
        // �����ɫͨ��˳��
        if (getType() == CV_8UC3)
            arrangement = "BGR��ɫͼ��";
        else if (getType() == CV_8UC3)
            arrangement = "RGB��ɫͼ��";
        else
            arrangement = "��ɫͼ��";
        break;
    case 4:
        arrangement = "��͸��ͨ����ͼ��";
        break;
    default:
        arrangement = "δ֪";
        break;
    }

    // ������з�ʽ��Ϣ
    cout << "���з�ʽ: " << arrangement << endl;
}

// ��ʾͼ��ͳ����Ϣ
void Image::showStats()
{
    if (data.empty())
    {
        cout << "�޷���ȡͼ��" << getPath() << endl;
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

// ����ͼ��ͳ����Ϣ
void Image::saveData(string filename)
{
    if (data.empty())
    {
        cout << "�޷���ȡͼ��" << getPath() << endl;
        return;
    }
    // ���ļ�
    ofstream file(filename);
    if (!file.is_open())
    {
        cout << "�޷����ļ���" << filename << endl;
        return;
    }

    // д���ļ�ͷ����Ϣ
    file << "Filename: " << filename << endl;
    file << "Dims: Full Scene (" << data.total() << " points)" << endl
         << endl;

    // ͳ��ÿ�����ε�����
    vector<Mat> bands;
    split(data, bands);

    file << "Basic Stats\tMin\tMax\t\tMean\t\t\tStdev" << endl;
    for (int i = 0; i < bands.size(); ++i)
    {
        Mat band = bands[i];

        // ����ͳ������
        double minVal, maxVal;
        Mat meanVal, stdDev;
        minMaxLoc(band, &minVal, &maxVal);
        meanStdDev(band, meanVal, stdDev);

        // д�����ͳ����Ϣ
        file << "\tBand " << i + 1 << "\t " << minVal << "\t" << maxVal << "\t" << meanVal << "\t" << stdDev << endl;
    }

    for (int i = 0; i < bands.size(); ++i)
    {
        Mat band = bands[i];

        // ��ȡ���ε���Сֵ�����ֵ
        double minVal, maxVal;
        minMaxLoc(band, &minVal, &maxVal);

        // ����ֱ��ͼ
        int histSize = static_cast<int>(maxVal - minVal + 1);
        float range[] = {static_cast<float>(minVal), static_cast<float>(maxVal + 1)};
        const float *histRange = {range};
        bool uniform = true, accumulate = false;

        Mat hist;
        calcHist(&band, 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);

        // д��ֱ��ͼ����
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

        // д�벨����Ϣ
        file << "Band " << i + 1 << ":\n";

        // ͳ�ƻҶȼ�����
        map<int, int> hist;
        for (int y = 0; y < band.rows; ++y)
        {
            for (int x = 0; x < band.cols; ++x)
            {
                int value = band.at<uchar>(y, x);
                hist[value]++;
            }
        }

        // ������������
        int totalPixels = band.rows * band.cols;

        // д��Ҷȼ��͸���
        for (auto it = hist.begin(); it != hist.end(); ++it)
        {
            int grayLevel = it->first;
            int count = it->second;
            double probability = static_cast<double>(count) / totalPixels;

            file << "�Ҷȼ�: " << grayLevel << "\t";
            file << "���ֵĸ���: " << probability << endl;
        }

        file << endl;
    }

    // �ر��ļ�
    file.close();

    cout << "ͼ�������ѱ��浽�ļ���" << filename << endl;
}

// ��ʾͼ��ֱ��ͼ
void Image::showHistogram()
{
    if (data.empty())
    {
        cout << "�޷���ȡͼ��" << getPath() << endl;
        return;
    }

    // �ָ�ͼ��ͨ��
    vector<Mat> channels;
    split(data, channels);

    // ����ֱ��ͼ
    int histSize = 256;       // �Ҷȼ�������
    float range[] = {0, 256}; // �Ҷȼ��ķ�Χ
    const float *histRange = {range};
    bool uniform = true, accumulate = false;

    // ��ʾÿ�����ε�ֱ��ͼ
    for (int i = 0; i < channels.size(); ++i)
    {
        // ����ֱ��ͼ
        Mat hist;
        calcHist(&channels[i], 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);

        // ����ֱ��ͼͼ��
        int histWidth = 512, histHeight = 400;
        int binWidth = cvRound((double)histWidth / histSize);
        Mat histImage(histHeight, histWidth, CV_8UC3, Scalar(0, 0, 0));

        // ��һ��ֱ��ͼֵ
        normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

        // ����ֱ��ͼ
        for (int j = 1; j < histSize; ++j)
        {
            line(histImage, Point(binWidth * (j - 1), histHeight - cvRound(hist.at<float>(j - 1))),
                 Point(binWidth * j, histHeight - cvRound(hist.at<float>(j))),
                 Scalar(255, 255, 255), 2, 8, 0);
        }

        // ��ʾֱ��ͼ
        imshow("Histogram of Band " + to_string(i + 1), histImage);
    }

    waitKey(0);
}

// ��ʾͼ��
void Image::displayImage()
{
    if (data.empty())
    {
        cout << "�޷���ȡͼ��" << getPath() << endl;
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
    cout << "�������ͼ����Ϣ�� (Y/N) : ";
    cin >> updateType;

    if (updateType == 'Y' || updateType == 'y')
    {
        data = displayImg;
        cout << "ͼ����Ϣ�ѱ����£�" << endl;
    }

    else if (updateType == 'N' || updateType == 'n')
    {
        cout << "ͼ����Ϣδ�����£�" << endl;
    }

    else
    {
        cout << "��Ч�����룡ͼ����Ϣδ�����£�" << endl;
        return;
    }
}

// ��תͼ��
void Image::rotateImage(double angle)
{
    if (data.empty())
    {
        cout << "û��ͼ�񱻴򿪣�" << endl;
        return;
    }

    Point2f center(getCols() / 2.0, getRows() / 2.0);
    Mat rotMat = getRotationMatrix2D(center, angle, 1.0);

    Mat rotatedImage;
    warpAffine(data, rotatedImage, rotMat, data.size()); // ��ͼ����з���任

    imshow("��ת���ͼ��", rotatedImage);
    waitKey(0);
    destroyWindow("��ת���ͼ��");

    // �Ƿ����ͼ����Ϣ
    char updateType;
    cout << "����Ҫ����ͼ����Ϣ��(Y/N) : ";
    cin >> updateType;

    if (updateType == 'Y' || updateType == 'y')
    {
        data = rotatedImage; // ����ͼ�����ݾ���
        cout << "ͼ����Ϣ�ѱ����£�" << endl;
    }

    else if (updateType == 'N' || updateType == 'n')
    {
        cout << "ͼ����Ϣδ������" << endl;
    }

    else
    {
        cout << "��Ч�����룡ͼ����Ϣδ�����£�" << endl;
        return;
    }
}

// ����ͼ��
void Image::zoomImage(double ratio)
{
    if (data.empty())
    {
        cout << "û��ͼ�񱻴򿪣�" << endl;
        return;
    }

    int newWidth = static_cast<int>(getCols() * ratio);
    int newHeight = static_cast<int>(getRows() * ratio);

    Mat zoomedImage;
    resize(data, zoomedImage, Size(newWidth, newHeight));

    imshow("���ź��ͼ��", zoomedImage);
    waitKey(0);
    destroyWindow("���ź��ͼ��");

    // �Ƿ����ͼ����Ϣ
    char updateType;
    cout << "����Ҫ����ͼ����Ϣ��(Y/N) : ";
    cin >> updateType;

    if (updateType == 'Y' || updateType == 'y')
    {
        data = zoomedImage; // ���¾���
        rows = data.rows;
        cols = data.cols;
        cout << "ͼ����Ϣ�ѱ����£�" << endl;
    }

    else if (updateType == 'N' || updateType == 'n')
    {
        cout << "ͼ����Ϣδ������" << endl;
    }

    else
    {
        cout << "��Ч�����룡ͼ����Ϣδ�����£�" << endl;
        return;
    }
}

// �˲�ͼ��
void Image::filterImage(Mat kernel)
{
    if (data.empty())
    {
        cout << "û��ͼ�񱻴򿪣�" << endl;
        return;
    }

    if (kernel.empty() || kernel.rows % 2 == 0 || kernel.cols % 2 == 0)
    {
        cout << "�˲��˴�С����ȷ" << endl;
        return;
    }

    Mat filteredImage;
    filter2D(data, filteredImage, -1, kernel);

    imshow("�˲����ͼ��", filteredImage);
    waitKey(0);
    destroyWindow("�˲����ͼ��");

    // �Ƿ����ͼ����Ϣ
    char updateType;
    cout << "����Ҫ����ͼ����Ϣ��(Y/N) : ";
    cin >> updateType;

    if (updateType == 'Y' || updateType == 'y')
    {
        data = filteredImage;
        cout << "ͼ����Ϣ�ѱ����£�" << endl;
    }

    else if (updateType == 'N' || updateType == 'n')
    {
        cout << "ͼ����Ϣδ�����£�" << endl;
    }

    else
    {
        cout << "��Ч�����룡ͼ����Ϣδ�����£�" << endl;
        return;
    }
}

// ����ͼ��Ϊ�ļ�
void Image::saveImage(string path)
{
    if (data.empty())
    {
        cout << "û��ͼ�񱻴򿪣�" << endl;
        return;
    }

    bool result = imwrite(path, data); // д��ͼ���ļ������ؽ��
    if (result)
    {
        cout << "����ͼ��ɹ���" << path << endl;
    }
    else
    {
        cout << "����ͼ��ʧ�ܣ� " << path << endl;
    }
}