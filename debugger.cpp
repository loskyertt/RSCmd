#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// Define a function to display the image
void displayImage()
{
    // Read the image from the file
    Mat img = imread("image.tif", IMREAD_UNCHANGED);

    // Check if the image is loaded successfully
    if (img.empty())
    {
        cout << "Error: Could not load the image." << endl;
        return;
    }

    // Get the number of bands, rows and columns of the image
    int bands = img.channels();
    int rows = img.rows;
    int cols = img.cols;

    // Print some basic information of the image
    cout << "The image has " << bands << " bands, " << rows << " rows and " << cols << " columns." << endl;

    // Declare some variables for user input
    int displayType;                  // 0 for gray, 1 for color
    char stretchType;                 // L for linear stretch, others for normal
    int redBand, greenBand, blueBand; // The band numbers for RGB channels

    // Add a new variable for the number of bands to input
    int bandNum; // The number of bands to input

    // Prompt the user to input the display parameters
    cout << "Input Display Parameters ( Display Type£¬Stretch Type£¬Red£¬Green£¬Blue )" << endl;
    cout << "Input Display Type 0-Gray or 1-Color : ";
    cin >> displayType;

    // Check if the input is valid
    if (displayType != 0 && displayType != 1)
    {
        cout << "Error: Invalid display type." << endl;
        return;
    }

    cout << "Input L-Linear stretch, Others - default is Normal : ";
    cin >> stretchType;

    // Decide the number of bands to input based on the display type
    if (displayType == 0)
    {
        bandNum = 1; // Only one band for gray display
    }
    else
    {
        bandNum = 3; // Three bands for color display
    }

    // Modify the prompt message to include the band number
    cout << "Input " << bandNum << " bands in base 0 : ";

    // Declare an array to store the band numbers
    int bandArray[3];

    // Read bandNum numbers from the user input
    for (int i = 0; i < bandNum; i++)
    {
        cin >> bandArray[i];
    }

    // Assign the band numbers to the corresponding variables
    redBand = bandArray[0];
    greenBand = bandArray[1];
    blueBand = bandArray[2];

    // Check if the user input the correct number of bands
    if (cin.fail() || cin.peek() != '\n')
    {
        cout << "Error: Invalid number of bands." << endl;
        return;
    }

    // Check if the input is valid
    if (redBand < 0 || redBand >= bands || greenBand < 0 || greenBand >= bands || blueBand < 0 || blueBand >= bands)
    {
        cout << "Error: Invalid band number." << endl;
        return;
    }

    // Declare a variable to store the displayed image
    Mat displayImg;

    // If the display type is gray, extract the gray band from the image and assign it to the display image
    if (displayType == 0)
    {
        vector<Mat> channels(bands);
        split(img, channels);
        displayImg = channels[redBand];
    }

    // If the display type is color, extract the RGB bands from the image and merge them into a color image
    else
    {
        vector<Mat> channels(bands);
        split(img, channels);
        vector<Mat> rgbChannels(3);
        rgbChannels[0] = channels[blueBand];
        rgbChannels[1] = channels[greenBand];
        rgbChannels[2] = channels[redBand];
        merge(rgbChannels, displayImg);

        // Convert the color image from BGR to RGB format
        cvtColor(displayImg, displayImg, COLOR_BGR2RGB);

        // Convert the color image from 16-bit to 8-bit format
        displayImg.convertTo(displayImg, CV_8U, 1.0 / 256.0);

        // Convert the color image from RGB to HSV format for easier manipulation of brightness and contrast
        cvtColor(displayImg, displayImg, COLOR_RGB2HSV);

        // Split the HSV image into three channels: hue, saturation and value
        vector<Mat> hsvChannels(3);
        split(displayImg, hsvChannels);

        // If the stretch type is linear, apply a linear stretch to the value channel to adjust the brightness and contrast of the image
        if (stretchType == 'L' || stretchType == 'l')
        {
            // Find the minimum and maximum value in the value channel
            double minVal, maxVal;
            minMaxLoc(hsvChannels[2], &minVal, &maxVal);

            // Apply a linear stretch to the value channel
            hsvChannels[2] = (hsvChannels[2] - minVal) * 255.0 / (maxVal - minVal);
        }

        // Merge the HSV channels back into a HSV image
        merge(hsvChannels, displayImg);

        // Convert the HSV image back to RGB format for display
        cvtColor(displayImg, displayImg, COLOR_HSV2RGB);
    }

    // Create a window to display the image
    namedWindow("Display Image", WINDOW_AUTOSIZE);

    // Show the image in the window
    imshow("Display Image", displayImg);

    // Wait for the user to press any key or close the window
    waitKey(0);

    // Destroy the window
    destroyWindow("Display Image");

    // Declare a variable for user input
    char updateType; // Y for yes, N for no

    // Prompt the user to input whether to update the image information or not
    cout << "Do you want to update the image information? (Y/N) : ";
    cin >> updateType;

    // If the user input is yes, update the image information with the displayed image
    if (updateType == 'Y' || updateType == 'y')
    {
        img = displayImg;
        cout << "The image information has been updated." << endl;
    }

    // If the user input is no, do nothing
    else if (updateType == 'N' || updateType == 'n')
    {
        cout << "The image information has not been updated." << endl;
    }

    // If the user input is invalid, print an error message and exit
    else
    {
        cout << "Error: Invalid input." << endl;
        return;
    }
}

// The main function to test the displayImage function
int main()
{
    // Call the displayImage function
    displayImage();

    // Return 0 to indicate success
    return 0;
}