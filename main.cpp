#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/ml/ml.hpp>  
using namespace std; 
#include <iostream>  
#include <fstream>  
#include <sstream>  

using namespace cv;  
using namespace std;  

char ad[128]={0};

uchar GetMinBinary(uchar *binary)
{
	// ����8��������
	uchar LBPValue[8] = { 0 };
	for (int i = 0; i <= 7; ++i)
	{
		LBPValue[0] += binary[i] <<(7-i);
		LBPValue[1] += binary[(i+7)%8] << (7 - i);
		LBPValue[2] += binary[(i + 6) % 8] << (7 - i);
		LBPValue[3] += binary[(i + 5) % 8] << (7 - i);
		LBPValue[4] += binary[(i + 4) % 8] << (7 - i);
		LBPValue[5] += binary[(i + 3) % 8] << (7 - i);
		LBPValue[6] += binary[(i + 2) % 8] << (7 - i);
		LBPValue[7] += binary[(i + 1) % 8] << (7 - i);
	}
	// ѡ����С��
	uchar minValue = LBPValue[0];
	for (int i = 1; i <= 7; ++i)
	{
		if (LBPValue[i] < minValue)
		{
			minValue = LBPValue[i];
		}
	}

	return minValue;
}
//int GetMinBinary(int binary)
//{
//	static const int miniBinaryLUT[256] = { 0, 1, 1, 3, 1, 5, 3, 7, 1, 9, 5, 11, 3, 13, 7, 15, 1, 17, 9, 19, 5,
//		21, 11, 23, 3, 25, 13, 27, 7, 29, 15, 31, 1, 9, 17, 25, 9, 37, 19, 39, 5, 37, 21, 43, 11, 45,
//		23, 47, 3, 19, 25, 51, 13, 53, 27, 55, 7, 39, 29, 59, 15, 61, 31, 63, 1, 5, 9, 13, 17, 21, 25,
//		29, 9, 37, 37, 45, 19, 53, 39, 61, 5, 21, 37, 53, 21, 85, 43, 87, 11, 43, 45, 91, 23, 87, 47, 95,
//		3, 11, 19, 27, 25, 43, 51, 59, 13, 45, 53, 91, 27, 91, 55, 111, 7, 23, 39, 55, 29, 87, 59, 119, 15,
//		47, 61, 111, 31, 95, 63, 127, 1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 9, 25, 37,
//		39, 37, 43, 45, 47, 19, 51, 53, 55, 39, 59, 61, 63, 5, 13, 21, 29, 37, 45, 53, 61, 21, 53, 85,
//		87, 43, 91, 87, 95, 11, 27, 43, 59, 45, 91, 91, 111, 23, 55, 87, 119, 47, 111, 95, 127, 3,
//		7, 11, 15, 19, 23, 27, 31, 25, 39, 43, 47, 51, 55, 59, 63, 13, 29, 45, 61, 53, 87, 91, 95, 27, 59,
//		91, 111, 55, 119, 111, 127, 7, 15, 23, 31, 39, 47, 55, 63, 29, 61, 87, 95, 59, 111, 119, 127, 15, 31, 47, 63,
//		61, 95, 111, 127, 31, 63, 95, 127, 63, 127, 127, 255};
//
//	return miniBinaryLUT[binary];
//}
int ComputeValue9(int value58)
{
	int value9 = 0;
	switch (value58)
	{
	case 1: 
		value9 = 1;
		break;
	case 2:
		value9 = 2;
		break;
	case 4:
		value9 = 3;
		break;
	case 7:
		value9 = 4;
		break;
	case 11:
		value9 = 5;
		break;
	case 16:
		value9 = 6;
		break;
	case 22:
		value9 = 7;
		break;
	case 29:
		value9 = 8;
		break;
	case 58:
		value9 = 9;
		break;
	}

	return value9;

}
//�ҶȲ��䳣��LBP(256)
void NormalLBPImage(const Mat &srcImage, Mat &LBPImage)
{
	CV_Assert(srcImage.depth() == CV_8U&&srcImage.channels() == 1);
	LBPImage.create(srcImage.size(), srcImage.type());


	Mat extendedImage;
	copyMakeBorder(srcImage, extendedImage, 1, 1, 1, 1, BORDER_DEFAULT);

    // ����LBP����ͼ
	int heightOfExtendedImage = extendedImage.rows;
	int widthOfExtendedImage = extendedImage.cols;
	int widthOfLBP=LBPImage.cols;
	uchar *rowOfExtendedImage= extendedImage.data+widthOfExtendedImage+1;
	uchar *rowOfLBPImage = LBPImage.data;
	for (int y = 1; y <= heightOfExtendedImage - 2; ++y, rowOfExtendedImage += widthOfExtendedImage, rowOfLBPImage += widthOfLBP)
	{
		// ��
		uchar *colOfExtendedImage = rowOfExtendedImage;
		uchar *colOfLBPImage = rowOfLBPImage;
		for (int x = 1; x <= widthOfExtendedImage - 2; ++x,++colOfExtendedImage,++colOfLBPImage)
		{
			// ����LBPֵ
			int LBPValue = 0;
			if (colOfExtendedImage[0 - widthOfExtendedImage - 1] >= colOfExtendedImage[0])
				LBPValue += 128;
			if (colOfExtendedImage[0 - widthOfExtendedImage ] >= colOfExtendedImage[0])
				LBPValue += 64;
			if (colOfExtendedImage[0 - widthOfExtendedImage +1] >= colOfExtendedImage[0])
				LBPValue += 32;
			if (colOfExtendedImage[0 +1] >= colOfExtendedImage[0])
				LBPValue += 16;
			if (colOfExtendedImage[0 + widthOfExtendedImage + 1] >= colOfExtendedImage[0])
				LBPValue += 8;
			if (colOfExtendedImage[0 + widthOfExtendedImage] >= colOfExtendedImage[0])
				LBPValue += 4;
			if (colOfExtendedImage[0 + widthOfExtendedImage - 1] >= colOfExtendedImage[0])
				LBPValue += 2;
			if (colOfExtendedImage[0 - 1] >= colOfExtendedImage[0])
				LBPValue += 1;
			colOfLBPImage[0] = LBPValue;
		} 
	}
}
// �ȼۻҶȲ���LBP(58)
void UniformNormalLBPImage(const Mat &srcImage, Mat &LBPImage)// ����ȼ�ģʽLBP����ͼ
{
	// ������飬�ڴ����
	CV_Assert(srcImage.depth() == CV_8U&&srcImage.channels() == 1);
	LBPImage.create(srcImage.size(), srcImage.type());

	// ����LBPͼ
	// ����ԭͼ��߽磬���ڱ߽紦��
	Mat extendedImage;
	copyMakeBorder(srcImage, extendedImage, 1, 1, 1, 1, BORDER_DEFAULT);

	// ����LBP �ȼ�ģʽ���ұ�
	//int table[256];
	//BuildUniformPatternTable(table);

	// LUT(256��ÿһ��ģʽ��Ӧ�ĵȼ�ģʽ)
	static const int table[256] = { 1, 2, 3, 4, 5, 0, 6, 7, 8, 0, 0, 0, 9, 0, 10, 11, 12, 0, 0, 0, 0, 0, 0, 0, 13, 0, 0, 0, 14, 0, 15, 16, 17, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18, 0, 0, 0, 0, 0, 0, 0, 19, 0, 0, 0, 20, 0, 21, 22, 23, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25,
		0, 0, 0, 0, 0, 0, 0, 26, 0, 0, 0, 27, 0, 28, 29, 30, 31, 0, 32, 0, 0, 0, 33, 0, 0, 0, 0, 0, 0, 0, 34, 0, 0, 0, 0
		, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 36, 37, 38, 0, 39, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 42
		, 43, 44, 0, 45, 0, 0, 0, 46, 0, 0, 0, 0, 0, 0, 0, 47, 48, 49, 0, 50, 0, 0, 0, 51, 52, 53, 0, 54, 55, 56, 57, 58 };

	// ����LBP
	int heightOfExtendedImage = extendedImage.rows;
	int widthOfExtendedImage = extendedImage.cols;
	int widthOfLBP=LBPImage.cols;
	uchar *rowOfExtendedImage = extendedImage.data+widthOfExtendedImage+1;
	uchar *rowOfLBPImage = LBPImage.data;
	for (int y = 1; y <= heightOfExtendedImage - 2; ++y,rowOfExtendedImage += widthOfExtendedImage, rowOfLBPImage += widthOfLBP)
	{
		// ��
		uchar *colOfExtendedImage = rowOfExtendedImage;
		uchar *colOfLBPImage = rowOfLBPImage;
		for (int x = 1; x <= widthOfExtendedImage - 2; ++x, ++colOfExtendedImage, ++colOfLBPImage)
		{
			// ����LBPֵ
			int LBPValue = 0;
			if (colOfExtendedImage[0 - widthOfExtendedImage - 1] >= colOfExtendedImage[0])
				LBPValue += 128;
			if (colOfExtendedImage[0 - widthOfExtendedImage] >= colOfExtendedImage[0])
				LBPValue += 64;
			if (colOfExtendedImage[0 - widthOfExtendedImage + 1] >= colOfExtendedImage[0])
				LBPValue += 32;
			if (colOfExtendedImage[0 + 1] >= colOfExtendedImage[0])
				LBPValue += 16;
			if (colOfExtendedImage[0 + widthOfExtendedImage + 1] >= colOfExtendedImage[0])
				LBPValue += 8;
			if (colOfExtendedImage[0 + widthOfExtendedImage] >= colOfExtendedImage[0])
				LBPValue += 4;
			if (colOfExtendedImage[0 + widthOfExtendedImage - 1] >= colOfExtendedImage[0])
				LBPValue += 2;
			if (colOfExtendedImage[0 - 1] >= colOfExtendedImage[0])
				LBPValue += 1;

			colOfLBPImage[0] = table[LBPValue];
		}
	}
}
// �ȼ���ת����LBP(9)
void UniformRotInvLBPImage(const Mat &srcImage, Mat &LBPImage)
{
	// ������飬�ڴ����
	CV_Assert(srcImage.depth() == CV_8U&&srcImage.channels() == 1);
	LBPImage.create(srcImage.size(), srcImage.type());

	// ����ͼ�񣬴���߽����
	Mat extendedImage;
	copyMakeBorder(srcImage, extendedImage, 1, 1, 1, 1, BORDER_DEFAULT);

	// ����LBP �ȼ�ģʽ���ұ�
	//int table[256];
	//BuildUniformPatternTable(table);

	// ͨ�����ұ�
	static const int table[256] = { 1, 2, 3, 4, 5, 0, 6, 7, 8, 0, 0, 0, 9, 0, 10, 11, 12, 0, 0, 0, 0, 0, 0, 0, 13, 0, 0, 0, 14, 0, 15, 16, 17, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18, 0, 0, 0, 0, 0, 0, 0, 19, 0, 0, 0, 20, 0, 21, 22, 23, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25,
		0, 0, 0, 0, 0, 0, 0, 26, 0, 0, 0, 27, 0, 28, 29, 30, 31, 0, 32, 0, 0, 0, 33, 0, 0, 0, 0, 0, 0, 0, 34, 0, 0, 0, 0
		, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 36, 37, 38, 0, 39, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 42
		, 43, 44, 0, 45, 0, 0, 0, 46, 0, 0, 0, 0, 0, 0, 0, 47, 48, 49, 0, 50, 0, 0, 0, 51, 52, 53, 0, 54, 55, 56, 57, 58 };

	uchar binary[8] = { 0 };// ��¼ÿ�����ص�LBPֵ
	int heigthOfExtendedImage = extendedImage.rows;
	int widthOfExtendedImage = extendedImage.cols;
	int widthOfLBPImage = LBPImage.cols;

	uchar *rowOfExtendedImage = extendedImage.data + widthOfExtendedImage + 1;
	uchar *rowOfLBPImage = LBPImage.data;
	for (int y = 1; y <= heigthOfExtendedImage - 2; ++y, rowOfExtendedImage += widthOfExtendedImage, rowOfLBPImage += widthOfLBPImage)
	{
		// ��
		uchar *colOfExtendedImage = rowOfExtendedImage;
		uchar *colOfLBPImage = rowOfLBPImage;
		for (int x = 1; x <= widthOfExtendedImage - 2; ++x, ++colOfExtendedImage, ++colOfLBPImage)
		{
			// ������ת����LBP(��С�Ķ�����ģʽ)
			binary[0] = colOfExtendedImage[0 - widthOfExtendedImage - 1] >= colOfExtendedImage[0] ? 1 : 0;
			binary[1] = colOfExtendedImage[0 - widthOfExtendedImage] >= colOfExtendedImage[0] ? 1 : 0;
			binary[2] = colOfExtendedImage[0 - widthOfExtendedImage + 1] >= colOfExtendedImage[0] ? 1 : 0;
			binary[3] = colOfExtendedImage[0 + 1] >= colOfExtendedImage[0] ? 1 : 0;
			binary[4] = colOfExtendedImage[0 + widthOfExtendedImage + 1] >= colOfExtendedImage[0] ? 1 : 0;
			binary[5] = colOfExtendedImage[0 + widthOfExtendedImage] >= colOfExtendedImage[0] ? 1 : 0;
			binary[6] = colOfExtendedImage[0 + widthOfExtendedImage - 1] >= colOfExtendedImage[0] ? 1 : 0;
			binary[7] = colOfExtendedImage[0 - 1] >= colOfExtendedImage[0] ? 1 : 0;

			int minValue = GetMinBinary(binary);

			// ����58�ֵȼ�ģʽLBP
			int value58=table[minValue];

			// ����9�ֵȼ�ģʽ
			colOfLBPImage[0] = ComputeValue9(value58);
		}

	}

}
//�ҶȲ��䳣��LBP(256)����
void NormalLBPFeature(const Mat &srcImage, Size cellSize, Mat &featureVector)
{
	// ������飬�ڴ����
	CV_Assert(srcImage.depth() == CV_8U&&srcImage.channels() == 1);

	Mat LBPImage;
	NormalLBPImage(srcImage,LBPImage);

	// ����cell����
	int widthOfCell = cellSize.width;
	int heightOfCell = cellSize.height;
	int numberOfCell_X = srcImage.cols / widthOfCell;// X����cell�ĸ���
	int numberOfCell_Y = srcImage.rows / heightOfCell;

	// ���������ĸ���
	int numberOfDimension = 256 * numberOfCell_X*numberOfCell_Y;
	featureVector.create(1, numberOfDimension, CV_32FC1);
	featureVector.setTo(Scalar(0));

	// ����LBP��������
	int stepOfCell=srcImage.cols;
	int pixelCount = cellSize.width*cellSize.height;
	float *dataOfFeatureVector=(float *)featureVector.data;

	// cell�������������������������е���ʼλ��
	int index = -256;
	for (int y = 0; y <= numberOfCell_Y - 1; ++y)
	{
		for (int x = 0; x <= numberOfCell_X - 1; ++x)
		{
			index+=256;

			// ����ÿ��cell��LBPֱ��ͼ
			Mat cell = LBPImage(Rect(x * widthOfCell, y * heightOfCell, widthOfCell, heightOfCell));
			uchar *rowOfCell=cell.data;
			for(int y_Cell=0;y_Cell<=cell.rows-1;++y_Cell,rowOfCell+=stepOfCell)
			{
				uchar *colOfCell=rowOfCell;
				for(int x_Cell=0;x_Cell<=cell.cols-1;++x_Cell,++colOfCell)
				{
					++dataOfFeatureVector[index + colOfCell[0]];
				}
			}

			// һ��Ҫ��һ��������������������ܴ�
			for (int i = 0; i <= 255; ++i)
				dataOfFeatureVector[index + i] /= pixelCount;

		}
	}

}
// �ȼۻҶȲ���LBP(58)����
void UniformNormalLBPFeature(const Mat &srcImage, Size cellSize, Mat &featureVector)
{
	// ������飬�ڴ����
	CV_Assert(srcImage.depth() == CV_8U&&srcImage.channels() == 1);

	Mat LBPImage;
	UniformNormalLBPImage(srcImage,LBPImage);

	// ����cell����
	int widthOfCell = cellSize.width;
	int heightOfCell = cellSize.height;
	int numberOfCell_X = srcImage.cols / widthOfCell;// X����cell�ĸ���
	int numberOfCell_Y = srcImage.rows / heightOfCell;

	// ���������ĸ���
	int numberOfDimension = 58 * numberOfCell_X*numberOfCell_Y;
	featureVector.create(1, numberOfDimension, CV_32FC1);
	featureVector.setTo(Scalar(0));

	// ����LBP��������
	int stepOfCell=srcImage.cols;
	int index = -58;// cell�������������������������е���ʼλ��
	float *dataOfFeatureVector=(float *)featureVector.data;
	for (int y = 0; y <= numberOfCell_Y - 1; ++y)
	{
		for (int x = 0; x <= numberOfCell_X - 1; ++x)
		{
			index+=58;

			// ����ÿ��cell��LBPֱ��ͼ
			Mat cell = LBPImage(Rect(x * widthOfCell, y * heightOfCell, widthOfCell, heightOfCell));
			uchar *rowOfCell=cell.data;
			int sum = 0; // ÿ��cell�ĵȼ�ģʽ����
			for(int y_Cell=0;y_Cell<=cell.rows-1;++y_Cell,rowOfCell+=stepOfCell)
			{
				uchar *colOfCell=rowOfCell;
				for(int x_Cell=0;x_Cell<=cell.cols-1;++x_Cell,++colOfCell)
				{
					if(colOfCell[0]!=0)
					{
						// ��ֱ��ͼ��ת��Ϊ0~57��������colOfCell[0] - 1
						++dataOfFeatureVector[index + colOfCell[0]-1];
						++sum;
					}
				}
			}

			// һ��Ҫ��һ��������������������ܴ�
			for (int i = 0; i <= 57; ++i)
				dataOfFeatureVector[index + i] /= sum;

		}
	}
}
// �ȼ���ת����LBP(9)����
void UniformRotInvLBPFeature(const Mat &srcImage, Size cellSize, Mat &featureVector)
{
	// ������飬�ڴ����
	CV_Assert(srcImage.depth() == CV_8U&&srcImage.channels() == 1);

	Mat LBPImage;
	UniformRotInvLBPImage(srcImage,LBPImage);

	// ����cell����
	int widthOfCell = cellSize.width;
	int heightOfCell = cellSize.height;
	int numberOfCell_X = srcImage.cols / widthOfCell;// X����cell�ĸ���
	int numberOfCell_Y = srcImage.rows / heightOfCell;

	// ���������ĸ���
	int numberOfDimension = 9 * numberOfCell_X*numberOfCell_Y;
	featureVector.create(1, numberOfDimension, CV_32FC1);
	featureVector.setTo(Scalar(0));

	// ����LBP��������
	int stepOfCell=srcImage.cols;
	int index = -9;// cell�������������������������е���ʼλ��
	float *dataOfFeatureVector=(float *)featureVector.data;
	for (int y = 0; y <= numberOfCell_Y - 1; ++y)
	{
		for (int x = 0; x <= numberOfCell_X - 1; ++x)
		{
			index+=9;

			// ����ÿ��cell��LBPֱ��ͼ
			Mat cell = LBPImage(Rect(x * widthOfCell, y * heightOfCell, widthOfCell, heightOfCell));
			uchar *rowOfCell=cell.data;
			int sum = 0; // ÿ��cell�ĵȼ�ģʽ����
			for(int y_Cell=0;y_Cell<=cell.rows-1;++y_Cell,rowOfCell+=stepOfCell)
			{
				uchar *colOfCell=rowOfCell;
				for(int x_Cell=0;x_Cell<=cell.cols-1;++x_Cell,++colOfCell)
				{
					if(colOfCell[0]!=0)
					{
						// ��ֱ��ͼ��ת��Ϊ0~8��������colOfCell[0] - 1
						++dataOfFeatureVector[index + colOfCell[0]-1];
						++sum;
					}
				}
			}

			// ֱ��ͼ��һ��
			for (int i = 0; i <= 8; ++i)
				dataOfFeatureVector[index + i] /= sum;

		}
	}
}
int main()  
{  
	//Mat img = cv::imread("12.jpg", 0);  
	//imshow("ԭͼ", img);  

	//Mat dst1 = Mat(img.rows,img.cols,CV_8UC1,Scalar(0));
	//NormalLBPImage(img,dst1);  
	//imshow("�ҶȲ���", dst1); 	

	//Mat dst2 = Mat(img.rows,img.cols,CV_8UC1,Scalar(0));
	//UniformNormalLBPImage(img,dst2);  
	//imshow("�ȼۻҶȲ���", dst2); 	

	//Mat dst3;
	//UniformRotInvLBPImage(img,dst3);  
	//imshow("�ȼ���ת����", dst3); 	

	//Mat fea1;
	//NormalLBPFeature(img,Size(50,50),fea1);


	//Mat fea2;
	//UniformNormalLBPFeature(img,Size(50,50),fea2);


	//Mat fea3;
	//UniformRotInvLBPFeature(img,Size(50,50),fea3);


	Mat samFeatureMat ,samLabelMat;
	Mat Feature;
	 int k=5,testnum=0,truenum=0;
	 for (int i = 0; i < 10; i++)
	 {
		 for (int j =0;j<400;j++)
		 {
			 sprintf_s(ad, "D:\\data\\%d\\%d.jpg",i,j);
			 Mat srcimage = imread(ad,0);
			 UniformRotInvLBPFeature(srcimage,Size(4,4),Feature);
			 if ( i == 0&&j==0)
			 {
				 samFeatureMat = Mat::zeros(4000  , Feature.cols, CV_32FC1);
				 samLabelMat = Mat::zeros(4000  , 1, CV_32FC1);
			 }
			 for(int k=0; k<Feature.cols; k++)
			 {
				 samFeatureMat.at<float>(i*400+j,k) = Feature.at<float>(0,k);
				 samLabelMat.at<float>(i*400+j,0) = i;
			 }
		 }
	 }

	 samFeatureMat.convertTo(samFeatureMat,CV_32F);
	 CvKNearest knn( samFeatureMat, samLabelMat, cv::Mat(), false, k );  
	 cv::Mat nearests( 1, k, CV_32F);  
	 //��ȡ��������  1000��
	 for (int i = 0; i < 10; i++)
	 {
		 for (int j =400;j<500;j++)
		 {
			 testnum++;
			 sprintf_s(ad, "D:\\data\\%d\\%d.jpg",i,j);
			 Mat testFeatureMat ;
			 Mat testdata = imread(ad,0);
			 UniformRotInvLBPFeature(testdata,Size(4,4),testFeatureMat);
			 testFeatureMat.convertTo(testFeatureMat,CV_32F);
			 int  response = knn.find_nearest(testFeatureMat,k,0,0,&nearests,0); 
			 if (response==i)
			 {
				 truenum++;
			 }
		 }
	 }
	 cout<<"��������"<<testnum<<endl;
	 cout<<"��ȷ������"<<truenum<<endl;
	 cout<<"׼ȷ�ʣ�"<<(float)truenum/testnum*100<<"%"<<endl;

	waitKey(0);  
	return 0;
}  