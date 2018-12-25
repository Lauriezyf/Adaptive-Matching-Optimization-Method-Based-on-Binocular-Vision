#include <iostream>
#include <iomanip>
#include <vector>
#include <math.h>
#include <algorithm>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

#define coef 4
#define disp 59

//计算坏点率
void Evaluate(Mat standard, Mat myMap, Mat test_origion);

//三通道彩色图像转成单通道灰度图，type表示不同的转换方式，其实优化效果并不大
Mat turnIntoGray(Mat input, int type);

//SSD实现
Mat SSD(Mat Left, Mat Right, bool LeftOrRight, int size);

void Evaluate(Mat standard, Mat myMap, Mat test_origion)
{
  
  int count = 0, ans = 0;
  double sum = 0.0;
  for (int i = 0; i < standard.rows; i ++)
  {

    for (int j = 0; j < standard.cols; j ++)
	{
	  if(test_origion.ptr<uchar>(i)[j] == 255)
	    {
	      sum=sum+1;
	      ans = standard.ptr<uchar>(i)[j] - myMap.ptr<uchar>(i)[j];
	      if (ans > coef || ans < -coef) count ++;
	    }
	  else
	    sum=sum;
	}
    }
  
  double result = (count + 0.0)/sum;
  cout <<setiosflags(ios::fixed);
  cout <<setprecision(2) << result * 100 << '%' << endl;
  
}



//type == 1, 2, 3 分别对应取均值、调用opencv的API、运用转换公式
Mat turnIntoGray(Mat input, int type)
{
  if (input.channels() == 1)return input;
  int row = input.rows;
  int col = input.cols;
  Mat output(row, col, CV_8UC1);

    if (type == 1 || type == 3) {
        for (int i = 0; i < row; i ++) {
            for (int j = 0; j < col; j ++) {
                double temp1 = input.at<Vec3b>(i, j)[0]; //B
                double temp2 = input.at<Vec3b>(i, j)[1]; //G
                double temp3 = input.at<Vec3b>(i, j)[2]; //R
                if (type == 1)output.ptr<uchar>(i)[j] = static_cast<uchar>((temp1 + temp2 + temp3) / 3);
                else output.ptr<uchar>(i)[j] = static_cast<uchar>(temp3 * 0.299 + temp2 * 0.587 + temp1 * 0.114);
             }
         }
    }
    else if (type == 2) {
            cvtColor(input, output, CV_BGR2GRAY);
    }
    return output;
}

Mat SSD(Mat Left, Mat Right, bool LeftOrRight, int size) {
	int row = Left.rows;
	int col = Left.cols;
	Mat output(row, col, CV_8UC1);
	vector<pair<double, int> > v;
	int size_ = size / 2;
	for (int i = 0; i < row; i ++) {
		for (int j = 0; j < col; j ++) {
			v.clear();
			for (int d = 0; d <= disp; d ++) {

				if (j - d < 0 && LeftOrRight) break;
				else if (j + d >= col && (!LeftOrRight))break;

				double temp = 0;
				
				int count = 0; 
				if (!LeftOrRight)d = -d;
				for (int x = i - size_; x <= i + size_; x ++) {
			    	for (int y = j - d - size_; y <= j - d + size_; y ++) {
				        if (x < 0 || y < 0 || x >= row || y >= col || y + d < 0 || y + d >= col)continue;
			        	else {
			                	double temp1;
			                	double temp2;
			                	if (LeftOrRight) {
			                        	temp1 = static_cast<double>(Right.ptr<uchar>(x)[y]);
			                         	temp2 = static_cast<double>(Left.ptr<uchar>(x)[y + d]);
			                	}
			                	else {
			                        	temp1 = static_cast<double>(Right.ptr<uchar>(x)[y + d]);
			                        	temp2 = static_cast<double>(Left.ptr<uchar>(x)[y]);
			                       	}
			                    	count ++;
			                      	temp += pow(temp1 - temp2, 2);
			           	}
			    	}
				}
				if (!LeftOrRight)d = -d;
				v.push_back(make_pair(temp/count, d));
		}
				
		double min = 2000000000;
		int index = 0;
		for (int k = 0; k < v.size(); k ++) {
	    	if (v[k].first - min < 0) {
		        	min = v[k].first;
		        	index = v[k].second;
	      	}
	   	}
		output.ptr<uchar>(i)[j] = static_cast<uchar>(index * coef);
    	}
    }
	return output;
}
