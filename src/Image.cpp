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

// ��ʾͼ��ֱ��ͼ
void Image::showHistogram()
{
    if (data.empty())
    {
        cout << "�޷���ȡͼ��" << getPath() << endl;
        return;
    }

    if (getBands() != 1)
    {
        cout << "ͼ���ǻҶ�ͼ������ת��Ϊ�Ҷ�ͼ��..." << endl;
        cvtColor(data, data, COLOR_BGR2GRAY);
        cout << "ת���ɹ���" << endl;
        imshow("�Ҷ�ͼ��", data);
        waitKey(0);

        destroyWindow("�Ҷ�ͼ��");
    }

    int histSize = 256;       // �Ҷȼ�����
    float range[] = {0, 256}; // �Ҷȼ���Χ
    const float *histRange = {range};
    bool uniform = true, accumulate = false;

    Mat histogram;

    // ����ֱ��ͼ
    calcHist(&data, 1, 0, Mat(), histogram, 1, &histSize, &histRange, uniform, accumulate);

    // ��������
    int canvasWidth = 512, canvasHeight = 400;
    int binWidth = cvRound((double)canvasWidth / histSize);
    Mat canvas(canvasHeight, canvasWidth, CV_8UC3, Scalar(0, 0, 0));

    // ��һ��ֱ��ͼ����
    normalize(histogram, histogram, 0, canvasHeight, NORM_MINMAX, -1, Mat());

    // ����ֱ��ͼ
    for (int i = 0; i < histSize; i++)
    {
        rectangle(canvas, Point(binWidth * i, canvasHeight), Point(binWidth * (i + 1), canvasHeight - cvRound(histogram.at<float>(i))), Scalar(255, 255, 255), -1);
    }

    // ��ʾֱ��ͼ
    imshow("�Ҷ�ֱ��ͼ", canvas);
    waitKey(0);

    // ����ֱ��ͼ����ǰ�ļ���
    string histPath = "./image/histogram.jpg";
    imwrite(histPath, canvas);
    cout << "ֱ��ͼ�ѱ���Ϊ: " << histPath << endl;
}

// ��ʾͼ��
void Image::displayImage()
{
    if (data.empty())
    {
        cout << "û��ͼ�񱻴򿪣�" << endl;
        return;
    }

    // ��ӡͼ���һЩ������Ϣ
    cout << "The image has " << getBands() << " bands, " << getRows() << " rows and " << getCols() << " columns." << endl;

    int displayType;
    char stretchType;
    int redBand, greenBand, blueBand;

    cout << "Input Display Parameters ( Display Type, Stretch Type, Red, Green, Blue )" << endl;
    cout << "Input Display Type 0-Gray or 1-Color : ";
    cin >> displayType;

    if (displayType != 0 && displayType != 1)
    {
        cout << "��Ч����ʾ���ͣ�" << endl;
        return;
    }

    cout << "Input L-Linear stretch, Others - default is Normal : ";
    cin >> stretchType;

    cout << "Input RGB bands or gray Band in base 0 " << endl;
    cin >> redBand >> greenBand >> blueBand;

    if (redBand < 0 || redBand >= bands || greenBand < 0 || greenBand >= bands || blueBand < 0 || blueBand >= bands)
    {
        cout << "��Ч�Ĳ�������" << endl;
        return;
    }

    // ����һ�������Դ洢��ʾ��ͼ��
    Mat displayImg;

    // �����ʾ����Ϊ��ɫ�����ͼ������ȡ��ɫ��������������ʾͼ��
    if (displayType == 0)
    {
        vector<Mat> channels(bands);
        split(data, channels);
        displayImg = channels[redBand];
    }

    // �����ʾ����Ϊ��ɫ�����ͼ������ȡRGB���β�����ϲ�Ϊ��ɫͼ��
    else
    {
        vector<Mat> channels(bands);
        split(data, channels);
        vector<Mat> rgbChannels(3);
        rgbChannels[0] = channels[blueBand];
        rgbChannels[1] = channels[greenBand];
        rgbChannels[2] = channels[redBand];
        merge(rgbChannels, displayImg);

        // ����ɫͼ���BGRת��ΪRGB��ʽ
        cvtColor(displayImg, displayImg, COLOR_BGR2RGB);

        // ����ɫͼ���16λ��ʽת��Ϊ8λ��ʽ
        displayImg.convertTo(displayImg, CV_8U, 1.0 / 256.0);

        // ����ɫͼ���RGBת��ΪHSV��ʽ���Ա�����׵ز������ȺͶԱȶ�
        cvtColor(displayImg, displayImg, COLOR_RGB2HSV);

        // ��HSVͼ����Ϊ����ͨ����ɫ�������ͶȺ�ֵ
        vector<Mat> hsvChannels(3);
        split(displayImg, hsvChannels);

        if (stretchType == 'L' || stretchType == 'l')
        {
            // ��ֵͨ���в�����Сֵ�����ֵ
            double minVal, maxVal;
            minMaxLoc(hsvChannels[2], &minVal, &maxVal);

            // ��ֵͨ��Ӧ����������
            hsvChannels[2] = (hsvChannels[2] - minVal) * 255.0 / (maxVal - minVal);
        }

        // ��HSVͨ���ϲ���HSVͼ��
        merge(hsvChannels, displayImg);

        // ��HSVͼ��ת����RGB��ʽ�Խ�����ʾ
        cvtColor(displayImg, displayImg, COLOR_HSV2RGB);
    }

    cout << "ͼ����ʾ�ɹ�������ESC�����˳�" << endl;

    namedWindow("Display Image", WINDOW_AUTOSIZE);
    imshow("Display Image", displayImg);
    waitKey(0);
    destroyWindow("Display Image");

    // �Ƿ����ͼ����Ϣ
    char updateType;
    cout << "����Ҫ����ͼ����Ϣ��(Y/N) : ";
    cin >> updateType;

    if (updateType == 'Y' || updateType == 'y')
    {
        data = displayImg;
        cout << "ͼ����Ϣ�ѱ����£�" << endl;
    }

    else if (updateType == 'N' || updateType == 'n')
    {
        cout << "ͼ����Ϣδ������" << endl;
    }

    else
    {
        cout << "��Ч������" << endl;
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

    data = rotatedImage; // ����ͼ�����ݾ���

    imshow("��ת���ͼ��", rotatedImage);
    waitKey(0);
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

    data = zoomedImage; // ���¾���
    rows = data.rows;
    cols = data.cols;

    imshow("���ź��ͼ��", zoomedImage);
    waitKey(0);
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

    // ����ͼ����Ϣ
    data = filteredImage;
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