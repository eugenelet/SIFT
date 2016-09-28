#include <stdio.h>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <fstream>

#define PI 3.14159265358979323846264338327
#define SIFT_ORI_HIST_BINS 36// default number of bins in histogram for orientation assignment
#define SIFT_ORI_SIG 1.5f// determines gaussian sigma for orientation assignment
#define SIFT_ORI_RADIUS 3 * SIFT_ORI_SIG// determines the radius of the region used in orientation assignment
#define SCALING 1.6
#define DOG_DETECT_KPT_SIZE 3
#define FAST_16_THRESHOLD 20
int FAST_8_THRESHOLD = 2;
#define FAST_8 1

using namespace cv;
using namespace std;

class mySIFT;
typedef vector< vector<double> > double2D;
class Key_Point;
void match(mySIFT& left, mySIFT& right, string targetFile, Mat img_scene, clock_t s);
void match(mySIFT& left, mySIFT& right, string targetFile, string targetFile1, Mat img_scene, clock_t s, mySIFT& left1);
Mat concat2Img(Mat& i1, Mat& i2);
Mat concat3Img(Mat& t1, Mat& t2, Mat& i1);
Point2f MatMulti(Mat matrix, Point2f point);

class mySIFT{
public:
	mySIFT(double sig, double kk, int nOct = 2, int nLpO = 5) : nOctave(nOct), nLayersPerOctave(nLpO), sigma(sig), k(kk){}
	void createDoG();
	void detectKeypoints();
	void LoadImage(Mat imgOri);
	void filterKeyPoints_FAST_16();
	void filterKeyPoints_FAST_8();
	void filterKeyPoints2();//Hessian
	void drawKeyPoints(string imageName);
	void computeDescriptor();
	void computeDescriptor_5x5();
	void computeDescriptor_6x6();
	void computeDescriptor_7x7();
	void computeDescriptor_8x8();
	void computeDescriptor_8x8_128dim();
	vector< Key_Point > keyPoints;
	vector< Key_Point > filteredKeyPoints;
	vector< Mat > blurredImgs;

	double2D getGaussianKernel(double sigma);
	Mat GaussianBlur(const Mat& src, double sigma);
	void computeOrientationHist(vector< Key_Point >& keyPoints);
	vector< double > DescriptorHelper(int row, int column, int layer);
	vector< double > DescriptorHelper_5x5(int Row, int Col, int layer);
	vector< double > DescriptorHelper_6x6(int Row, int Col, int layer);
	vector< double > DescriptorHelper_7x7(int Row, int Col, int layer);
	vector< double > DescriptorHelper_8x8(int Row, int Col, int layer);
	void filterKeyPoints_FAST_16_Helper1(vector< int >& brighter, vector< int >& darker, Mat& thisMat, Key_Point& thisKpt, int value, int threshold);
	void filterKeyPoints_FAST_16_Helper2(vector< int >& brighter, vector< int >& darker, int& nbrighter, int& ndarker);
	void filterKeyPoints_FAST_8_Helper1(vector< int >& brighter, vector< int >& darker, Mat& thisMat, Key_Point& thisKpt, int value, int threshold);
	void filterKeyPoints_FAST_8_Helper2(vector< int >& brighter, vector< int >& darker, int& nBrighter, int& nDarker);
	

	vector< Mat > DoGs;
	int nLayersPerOctave;
	int nOctave;
	double sigma;
	double k;
};

class Key_Point{
public:
	Key_Point(double s, int r, int c, int t, int l) : scale(s), row(r), col(c), type(t), layer(l){}
	int cornerValue;
	int row;
	int col;
	int orientation;
	int type;//0:min, 1:max
	double scale;
	int layer;//在DoG的哪一個Layer
	vector< int > descriptor;
	float R;
};

void dumpFilterKeyPoints(mySIFT& image)
{
	ofstream fout1, fout2;
	char* layer1_output_file = "keypoint_layer1.txt";
	char* layer2_output_file = "keypoint_layer2.txt";
	fout1.open(layer1_output_file);
	fout2.open(layer2_output_file);

	vector< Key_Point >& kpt = image.keyPoints;
	
	for(int i = 0; i < kpt.size(); i++){
		if(kpt[i].layer == 1)
			fout1 << kpt[i].row << ' ' << kpt[i].col << endl;
		else if(kpt[i].layer == 2)
			fout2 << kpt[i].row << ' ' << kpt[i].col << endl;
		else
			cout << "????" << endl;
	}


	fout1.close();
	fout2.close();

}

void dumpDetectKeyPoints(mySIFT& image)
{
	ofstream fout1, fout2;
	char* layer1_output_file = "detect_layer1.txt";
	char* layer2_output_file = "detect_layer2.txt";
	fout1.open(layer1_output_file);
	fout2.open(layer2_output_file);

	vector< Key_Point >& kpt = image.keyPoints;
	
	for(int i = 0; i < kpt.size(); i++){
		if(kpt[i].layer == 1)
			fout1 << kpt[i].row << ' ' << kpt[i].col << endl;
		else if(kpt[i].layer == 2)
			fout2 << kpt[i].row << ' ' << kpt[i].col << endl;
		else
			cout << "????" << endl;
	}


	fout1.close();
	fout2.close();

}
int main()
{
	srand(time(NULL));
	clock_t start, end;

	if (1){
		mySIFT haha(1.414, 1.414, 1);//sigma k
		string targetFile = "testsmall.jpg";//"jijin4.jpg";
		Mat target = imread(targetFile, CV_LOAD_IMAGE_GRAYSCALE);
		haha.LoadImage(target);
		haha.createDoG();
		haha.detectKeypoints();
		dumpDetectKeyPoints(haha);
		//haha.drawKeyPoints(targetFile);
		//cout << "Target Kpt # Before Filter : " << haha.keyPoints.size() << "\n";
		if (FAST_8)
			haha.filterKeyPoints_FAST_8();
		else
			haha.filterKeyPoints_FAST_16();
		dumpFilterKeyPoints(haha);
		return 0;
		//haha.drawKeyPoints(targetFile);
		//cout << "Target Kpt # After Filter : " << haha.keyPoints.size() << "\n";
		//haha.drawKeyPoints(targetFile);
		haha.computeDescriptor_8x8_128dim();

		//for (int i = 0; i < haha.blurredImgs.size(); ++i){
		//	imshow("blurr", haha.blurredImgs[i]);
		//	cout << i << " " << haha.blurredImgs[i].rows << " " << haha.blurredImgs[i].cols << "\n";
		//	waitKey(0);
		//}

		//mySIFT haha1(1.414, 1.5, 4);//sigma k
		//string targetFile1 = "pro.jpg";
		//Mat target1 = imread(targetFile1, CV_LOAD_IMAGE_GRAYSCALE);
		//haha1.LoadImage(target1);
		//haha1.createDoG();
		//haha1.detectKeypoints();
		////haha1.drawKeyPoints(targetFile);
		////cout << "Target Kpt # Before Filter : " << haha.keyPoints.size() << "\n";
		//if (FAST_8)
		//	haha1.filterKeyPoints_FAST_8();
		//else
		//	haha1.filterKeyPoints_FAST_16();
		////haha1.drawKeyPoints(targetFile);
		////cout << "Target Kpt # After Filter : " << haha.keyPoints.size() << "\n";
		////haha.drawKeyPoints(targetFile);
		//haha1.computeDescriptor();

		
		VideoCapture cap(0);

		//Mat fixed_scene = imread("capture.jpg");//跑硬體的img_scene

		string Location = "VideoTest.avi";
		VideoCapture video(Location);
		if (!video.isOpened()){
			cout << "Video is not loaded\n";
			system("pause");
		}
		//Mat videoFrame;

		while (1){

			clock_t s;
			s = clock();
			Mat img_scene;
			//cap >> img_scene;
			img_scene = imread("testsmall.jpg");// , CV_LOAD_IMAGE_GRAYSCALE);
			//video >> img_scene;
			
			if (img_scene.empty())
			//	break;
			
			clock_t start, end;

			mySIFT hoho(1.414, 1.5, 1);//sigma k
			//hoho.LoadImage(imageName2);
			//Mat imgScene = imread("me.jpg", CV_LOAD_IMAGE_GRAYSCALE);//這張是灰色的
			Mat imgScene;
			//cvtColor(img_scene, imgScene, CV_BGR2GRAY);
			cvtColor(img_scene, imgScene, CV_BGR2GRAY);
			
			//imwrite("capture.jpg", imgScene);
			
			hoho.LoadImage(imgScene);
			//hoho.LoadImage(fixed_scene);
			
			//start = clock();
			hoho.createDoG();
			//end = clock();
			//cout << "Blurred & Create DoGs : " << (double)(end - s) / CLOCKS_PER_SEC << "\n";
			
			//start = clock();
			hoho.detectKeypoints();
			//end = clock();
			//cout << "Detect Keypoints : " << (double)(end - s) / CLOCKS_PER_SEC << "\n";
			//cout << "Image Kpt # Before Filter : " << hoho.keyPoints.size() << "\n";
			//start = clock();
			if (FAST_8)
				hoho.filterKeyPoints_FAST_8();
			else
				hoho.filterKeyPoints_FAST_16();
			//cout << "Image Kpt # After Filter : " << hoho.keyPoints.size() << "\n";
			//end = clock();
			//cout << "Filter Keypoints(FAST) : " << (double)(end - s) / CLOCKS_PER_SEC << "\n";

			//start = clock();
			hoho.computeDescriptor_8x8_128dim();
			//end = clock();
			//cout << "Compute Descriptors : " << (double)(end - s) / CLOCKS_PER_SEC << "\n";

			//start = clock();
			match(haha, hoho, targetFile, img_scene, s);
			//match(haha, hoho, targetFile, targetFile1, img_scene, s, haha1);
			
			//end = clock();
			//cout << "Match : " << (double)(end - s) / CLOCKS_PER_SEC << "\n";
			cout << "\n";

			//fstream fout;
			//fout.open("originalImage1.txt", ios::out);//輸出給硬體的img_scene
			//fout << hoho.keyPoints.size() << "\n";
			//for (int row = 0; row < 480; ++row){
			//	for (int col = 0; col < 640; ++col)
			//		fout << (int)imgScene.at<uchar>(row, col) << " ";
			//	fout << "\n";
			//}
			//fout.close();
		}
		
	}
	else if(0){//測試串接2張圖
		Mat i1 = imread("jijin.jpg");// , CV_LOsAD_IMAGE_GRAYSCALE);
		Mat i2 = imread("gg.jpg");// , CV_LOAD_IMAGE_GRAYSCALE);
		Mat i3 = concat2Img(i1, i2);
		Mat smalli1;
		resize(i1, smalli1, Size(i1.cols / 2, i1.rows / 2));
		imshow("??", smalli1);
		imshow("!!", i1);
		waitKey(0);
	}
	else{
		Mat src = imread("cc.jpg", CV_LOAD_IMAGE_GRAYSCALE);//這張是灰色的
		Mat dst;
		resize(src, dst, Size(src.cols / 1.1, src.rows / 1.1), 0, 0, INTER_NEAREST);
		imshow("??", dst);
		waitKey(0);
	}

	system("pause");
	return 0;
}

void match(mySIFT& left, mySIFT& right, string targetFile, string targetFile1, Mat img_scene, clock_t s, mySIFT& left1)
{
	vector< Key_Point >& a = left.keyPoints;
	vector< Key_Point >& b = left1.keyPoints;
	vector< Key_Point >& c = right.keyPoints;

	//cout << "Matching... target kpt # : " << a.size() << " " << "image kpt # : " << b.size() << "\n";

	/*fstream fout_target, fout_image;
	fout_target.open("targetRowColDespt2.txt", ios::out);
	fout_image.open("imageRowColDespt2.txt", ios::out);
	fout_target << a.size() << "\n";
	fout_image << b.size() << "\n";
	for (int i = 0; i < a.size(); ++i){
		int aScaleNum = a[i].layer / left.nLayersPerOctave;// == 0) ? 1 : 1.6;
		double aScaling = 1;
		for (int k = 0; k < aScaleNum; ++k){
			aScaling *= SCALING;
		}
		int r = a[i].row * aScaling;//浮點到int的truncation
		int c = a[i].col * aScaling;//浮點到int的truncation


		fout_target << r << "\n";

		fout_target << c << "\n";
		//element
		for (int j = 31; j >= 0; --j){//高維放前面
			int jthElement = a[i].descriptor[j];

			fout_target << jthElement << "\n";
		}
		fout_target << "\n";
	}

	for (int i = 0; i < b.size(); ++i){
		int r = b[i].row;
		int c = b[i].col;

		fout_image << r << "\n";

		fout_image << c << "\n";

		for (int j = 31; j >= 0; --j){//高維放前面
			int jthElement = b[i].descriptor[j];

			fout_image << jthElement << "\n";
		}
		fout_image << "\n";
	}
	fout_target.close();
	fout_image.close();
	*/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Mat target = imread(targetFile);//我要彩色的
	Mat target1 = imread(targetFile1);//我要彩色的
	Mat find = img_scene;
	Mat result = concat3Img(target, target1, find);

	vector< Point2f > obj;
	vector< Point2f > scene;

	for (int i = 0; i < a.size(); ++i){//對每一個左邊的Key_Point:a[i]

		int index = -1;//index of minimum distance;
		double min = INT_MAX;
		//int indexMin2 = -1;
		double min2 = INT_MAX;
		double maxDist = 0;
		for (int j = 0; j < c.size(); ++j){//暴力去掃每一個右邊的Key_Point:b[j]
			double dist = 0;
			for (int k = 0; k < 32; ++k)
				dist += abs(a[i].descriptor[k] - c[j].descriptor[k]);// *(a[i].descriptor[k] - b[j].descriptor[k]);
			if (dist < min){//最小的要被取代了
				min2 = min;
				//indexMin2 = index;
				min = dist;
				index = j;//左邊i match到右邊index
			}
			else if (dist < min2){
				min2 = dist;
			}
			if (dist > maxDist)
				maxDist = dist;
		}
		//cout << "maxDist : " << maxDist << "\n";

		//circle(garha, Point(a[i].col, a[i].row), 3, Scalar(0, 0, 255));
		//circle(garho, Point(b[index].col, b[index].row), 3, Scalar(0, 0, 255));
		//imshow("target", garha);
		//imshow("find", garho);
		//while (waitKey(0) != 'e')
		//	continue;

		int B = rand() % 256;
		int G = rand() % 256;
		int R = rand() % 256;

		if (min < 0.68 * min2){//good matches
			int aScaleNum = a[i].layer / left.nLayersPerOctave;// == 0) ? 1 : 1.6;
			double aScaling = 1;
			for (int k = 0; k < aScaleNum; ++k){
				aScaling *= SCALING;
			}
			int bScaleNum = c[index].layer / right.nLayersPerOctave;// == 0) ? 1 : 1.6;//縮小幾倍的，要放大回來
			double bScaling = 1;
			for (int k = 0; k < bScaleNum; ++k){
				bScaling *= SCALING;
			}

			line(result, Point(a[i].col * aScaling, a[i].row * aScaling), Point(max(target.cols, target1.cols) + c[index].col * bScaling, c[index].row * bScaling), Scalar(255, 0, 255));
			obj.push_back(Point2f((int)(a[i].col * aScaling), (int)(a[i].row * aScaling)));
			scene.push_back(Point2f((int)(c[index].col * bScaling), (int)(c[index].row * bScaling)));
		}
	}

	vector< Point2f > obj1;
	vector< Point2f > scene1;

	for (int i = 0; i < b.size(); ++i){//對每一個左邊的Key_Point:a[i]

		int index = -1;//index of minimum distance;
		double min = INT_MAX;
		//int indexMin2 = -1;
		double min2 = INT_MAX;
		double maxDist = 0;
		for (int j = 0; j < c.size(); ++j){//暴力去掃每一個右邊的Key_Point:b[j]
			double dist = 0;
			for (int k = 0; k < 32; ++k)
				dist += abs(b[i].descriptor[k] - c[j].descriptor[k]);// *(a[i].descriptor[k] - b[j].descriptor[k]);
			if (dist < min){//最小的要被取代了
				min2 = min;
				//indexMin2 = index;
				min = dist;
				index = j;//左邊i match到右邊index
			}
			else if (dist < min2){
				min2 = dist;
			}
			if (dist > maxDist)
				maxDist = dist;
		}
		//cout << "maxDist : " << maxDist << "\n";

		//circle(garha, Point(a[i].col, a[i].row), 3, Scalar(0, 0, 255));
		//circle(garho, Point(b[index].col, b[index].row), 3, Scalar(0, 0, 255));
		//imshow("target", garha);
		//imshow("find", garho);
		//while (waitKey(0) != 'e')
		//	continue;

		int B = rand() % 256;
		int G = rand() % 256;
		int R = rand() % 256;

		if (min < 0.68 * min2){//good matches
			int aScaleNum = b[i].layer / left.nLayersPerOctave;// == 0) ? 1 : 1.6;
			double aScaling = 1;
			for (int k = 0; k < aScaleNum; ++k){
				aScaling *= SCALING;
			}
			int bScaleNum = c[index].layer / right.nLayersPerOctave;// == 0) ? 1 : 1.6;//縮小幾倍的，要放大回來
			double bScaling = 1;
			for (int k = 0; k < bScaleNum; ++k){
				bScaling *= SCALING;
			}

			line(result, Point(b[i].col * aScaling, b[i].row * aScaling + target.rows), Point(max(target.cols, target1.cols) + c[index].col * bScaling, c[index].row * bScaling), Scalar(0, 255, 255));
			obj1.push_back(Point2f((int)(b[i].col * aScaling), (int)(b[i].row * aScaling)));
			scene1.push_back(Point2f((int)(c[index].col * bScaling), (int)(c[index].row * bScaling)));
		}
	}
	//結束match
	/*fstream fout_ans;
	fout_ans.open("ans2.txt", ios::out);
	fout_ans << obj.size() << "\n";
	for (int i = 0; i < obj.size(); ++i){
		fout_ans << obj[i].y << " " << obj[i].x << " " << scene[i].y << " " << scene[i].x << "\n";//target row col, image row col
	}
	fout_ans.close();*/

	if (obj.empty() || scene.empty())
		return;// system("pause");

	if (obj1.empty() || scene1.empty())
		return;// system("pause");
	
	//cout << "前面全部 : " << (double)(end - s) / CLOCKS_PER_SEC << "\n";



	//Mat H = findHomography(obj, scene, CV_RANSAC);
	Mat H;
	if (obj.size()>3 && scene.size()>3)
		H = findHomography(obj, scene, CV_RANSAC);
	else
		return;

	Mat H1;
	if (obj1.size()>3 && scene1.size()>3)
		H1 = findHomography(obj1, scene1, CV_RANSAC);
	else
		return;
	
	//cout << "Find Homography Use " << (double)(end - start) / CLOCKS_PER_SEC << "\n\n\n";

	vector< Point2f > obj_corners(4);
	obj_corners[0] = cvPoint(0, 0);
	obj_corners[1] = cvPoint(left.blurredImgs[0].cols, 0);
	obj_corners[2] = cvPoint(left.blurredImgs[0].cols, left.blurredImgs[0].rows);
	obj_corners[3] = cvPoint(0, left.blurredImgs[0].rows);

	vector< Point2f > obj_corners1(4);
	obj_corners1[0] = cvPoint(0, 0);
	obj_corners1[1] = cvPoint(left1.blurredImgs[0].cols, 0);
	obj_corners1[2] = cvPoint(left1.blurredImgs[0].cols, left1.blurredImgs[0].rows);
	obj_corners1[3] = cvPoint(0, left1.blurredImgs[0].rows);

	vector< Point2f > scene_corners(4);
	vector< Point2f > computed_corners(4);
	vector< Point2f > computed_corners1(4);
	for (int i = 0; i < 4; ++i)
		computed_corners[i] = MatMulti(H, obj_corners[i]);//自己算
	for (int i = 0; i < 4; ++i)
		computed_corners1[i] = MatMulti(H1, obj_corners1[i]);//自己算
	//end = clock();
	//cout << "Find Homography Use " << (double)(end - start) / CLOCKS_PER_SEC << "\n\n\n";

	//perspectiveTransform(obj_corners, scene_corners, H);
	//這是用openCV的function算的
	//line(result, scene_corners[0] + obj_corners[1], scene_corners[1] + obj_corners[1], Scalar(0, 255, 0), 4);
	//line(result, scene_corners[1] + obj_corners[1], scene_corners[2] + obj_corners[1], Scalar(0, 255, 0), 4);
	//line(result, scene_corners[2] + obj_corners[1], scene_corners[3] + obj_corners[1], Scalar(0, 255, 0), 4);
	//line(result, scene_corners[3] + obj_corners[1], scene_corners[0] + obj_corners[1], Scalar(0, 255, 0), 4);

	//這是自己算的
	line(result, computed_corners[0] + obj_corners[1], computed_corners[1] + obj_corners[1], Scalar(255, 0, 255), 4);
	line(result, computed_corners[1] + obj_corners[1], computed_corners[2] + obj_corners[1], Scalar(255, 0, 255), 4);
	line(result, computed_corners[2] + obj_corners[1], computed_corners[3] + obj_corners[1], Scalar(255, 0, 255), 4);
	line(result, computed_corners[3] + obj_corners[1], computed_corners[0] + obj_corners[1], Scalar(255, 0, 255), 4);

	line(result, computed_corners1[0] + obj_corners[1], computed_corners1[1] + obj_corners[1], Scalar(0, 255, 255), 4);
	line(result, computed_corners1[1] + obj_corners[1], computed_corners1[2] + obj_corners[1], Scalar(0, 255, 255), 4);
	line(result, computed_corners1[2] + obj_corners[1], computed_corners1[3] + obj_corners[1], Scalar(0, 255, 255), 4);
	line(result, computed_corners1[3] + obj_corners[1], computed_corners1[0] + obj_corners[1], Scalar(0, 255, 255), 4);

	imshow("haha", result);
	imwrite("result.jpg", result);
	waitKey(1);
}

void match(mySIFT& left, mySIFT& right, string targetFile, Mat img_scene, clock_t s)
{
	vector< Key_Point >& a = left.keyPoints;
	vector< Key_Point >& b = right.keyPoints;

	cout << "Matching... target kpt # : " << a.size() << " " << "image kpt # : " << b.size() << "\n";
	if (b.size() < 1500)
		--FAST_8_THRESHOLD;
	else if (b.size() > 2000)
		++FAST_8_THRESHOLD;
	//fstream fout_target, fout_image_layer_1, fout_image_layer_2, fout_image;
	//fout_target.open("targetRowColDespt.txt", ios::out);
	//fout_image.open("imageRowColDescpt.txt", ios::out);
	////fout_image_layer_1.open("imageRowCol_Layer1.txt", ios::out);
	////fout_image_layer_2.open("imageRowCol_Layer2.txt", ios::out);
	//fout_target << a.size() << "\n";
	//fout_image << b.size() << "\n";
	//for (int i = 0; i < a.size(); ++i){
	//	int aScaleNum = a[i].layer / left.nLayersPerOctave;// == 0) ? 1 : 1.6;
	//	double aScaling = 1;
	//	for (int k = 0; k < aScaleNum; ++k){
	//		aScaling *= SCALING;
	//	}
	//	int r = a[i].row * aScaling;//浮點到int的truncation
	//	int c = a[i].col * aScaling;//浮點到int的truncation
	//
	//	
	//	fout_target << r << "\n";
	//	
	//	fout_target << c << "\n";
	//	//element
	//	for (int j = 0; j < 32; ++j){
	//		int jthElement = a[i].descriptor[j];
	//		
	//		fout_target << jthElement << "\n";
	//	}
	//	fout_target << "\n";
	//}
	//
	//for (int i = 0; i < b.size(); ++i){
	//	int r = b[i].row;
	//	int c = b[i].col;
	//	//if (b[i].layer == 1)
	//	fout_image << r << " " << c << "\n";
	//	//else
	//		//fout_image_layer_2 << r << " " << c << "\n";
	//	for (int j = 0; j < 32; ++j){
	//		int jthElement = b[i].descriptor[j];
	//		
	//		fout_image << jthElement << " ";
	//	}
	//	fout_image << "\n";
	//}
	//fout_target.close();
	//fout_image.close();
	//fout_image_layer_2.close();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Mat target = imread(targetFile);//我要彩色的
	Mat find = img_scene;
	Mat result = concat2Img(target, find);

	vector< Point2f > obj;
	vector< Point2f > scene;

	for (int i = 0; i < a.size(); ++i){//對每一個左邊的Key_Point:a[i]
		
		int index = -1;//index of minimum distance;
		double min = INT_MAX;
		//int indexMin2 = -1;
		double min2 = INT_MAX;
		double maxDist = 0;
		for (int j = 0; j < b.size(); ++j){//暴力去掃每一個右邊的Key_Point:b[j]
			double dist = 0;
			for (int k = 0; k < 128; ++k)
				dist += abs(a[i].descriptor[k] - b[j].descriptor[k]);// *(a[i].descriptor[k] - b[j].descriptor[k]);
			if (dist < min){//最小的要被取代了
				min2 = min;
				//indexMin2 = index;
				min = dist;
				index = j;//左邊i match到右邊index
			}
			else if (dist < min2){
				min2 = dist;
			}
			if (dist > maxDist)
				maxDist = dist;
		}
		//cout << "maxDist : " << maxDist << "\n";

		//circle(garha, Point(a[i].col, a[i].row), 3, Scalar(0, 0, 255));
		//circle(garho, Point(b[index].col, b[index].row), 3, Scalar(0, 0, 255));
		//imshow("target", garha);
		//imshow("find", garho);
		//while (waitKey(0) != 'e')
		//	continue;

		//int B = rand() % 256;
		//int G = rand() % 256;
		//int R = rand() % 256;

		if (min < 0.72 * min2){//good matches 
			int aScaleNum = a[i].layer / left.nLayersPerOctave;// == 0) ? 1 : 1.6;
			double aScaling = 1;
			for (int k = 0; k < aScaleNum; ++k){
				aScaling *= SCALING;
			}
			int bScaleNum = b[index].layer / right.nLayersPerOctave;// == 0) ? 1 : 1.6;//縮小幾倍的，要放大回來
			double bScaling = 1;
			for (int k = 0; k < bScaleNum; ++k){
				bScaling *= SCALING;
			}

			line(result, Point(a[i].col * aScaling, a[i].row * aScaling), Point(target.cols + b[index].col * bScaling, b[index].row * bScaling), Scalar(0, 255, 255));
			obj.push_back(Point2f((int)(a[i].col * aScaling), (int)(a[i].row * aScaling)));
			scene.push_back(Point2f((int)(b[index].col * bScaling), (int)(b[index].row * bScaling)));
		}
	}
	//結束match
	//fstream fout_ans;
	//fout_ans.open("ans.txt", ios::out);
	//fout_ans << obj.size() << "\n";
	//for (int i = 0; i < obj.size(); ++i){
	//	fout_ans << obj[i].y << " " << obj[i].x << " " << scene[i].y << " " << scene[i].x << "\n";//target row col, image row col
	//}
	//fout_ans.close();
	cout << "Good match num : " << scene.size() << "\n";
	if (obj.empty() || scene.empty())
		return;// system("pause");

	clock_t end = clock();
	//cout << "前面全部 : " << (double)(end - s) / CLOCKS_PER_SEC << "\n";

	vector< Point2f > fixed_obj;
	vector< Point2f > fixed_scene;

	//fstream fin;
	//fin.open("match.txt", ios::in);//硬體算的keypoint pair
	//int tarRow, tarCol, imgRow, imgCol;
	//for (int i = 0; i < 119; ++i){
	//	fin >> tarRow;
	//	fin >> tarCol;
	//	fin >> imgRow;
	//	fin >> imgCol;
	//	fixed_obj.push_back(Point2f(tarCol, tarRow));
	//	fixed_scene.push_back(Point2f(imgCol, imgRow));
	//}


	clock_t start = clock();
	
	//Mat H = findHomography(obj, scene, CV_RANSAC);
	Mat H;
	if (obj.size()>3 && scene.size()>3)
		H = findHomography(obj, scene, CV_RANSAC);
	else
		return;
	end = clock();
	//cout << "Find Homography Use " << (double)(end - start) / CLOCKS_PER_SEC << "\n\n\n";

	vector< Point2f > obj_corners(4);
	obj_corners[0] = cvPoint(0, 0);
	obj_corners[1] = cvPoint(left.blurredImgs[0].cols, 0);
	obj_corners[2] = cvPoint(left.blurredImgs[0].cols, left.blurredImgs[0].rows);
	obj_corners[3] = cvPoint(0, left.blurredImgs[0].rows);
	
	vector< Point2f > scene_corners(4);
	vector< Point2f > computed_corners(4);
	for (int i = 0; i < 4; ++i)
		computed_corners[i] = MatMulti(H, obj_corners[i]);//自己算

	//end = clock();
	//cout << "Find Homography Use " << (double)(end - start) / CLOCKS_PER_SEC << "\n\n\n";

	//perspectiveTransform(obj_corners, scene_corners, H);
	//這是用openCV的function算的
	//line(result, scene_corners[0] + obj_corners[1], scene_corners[1] + obj_corners[1], Scalar(0, 255, 0), 4);
	//line(result, scene_corners[1] + obj_corners[1], scene_corners[2] + obj_corners[1], Scalar(0, 255, 0), 4);
	//line(result, scene_corners[2] + obj_corners[1], scene_corners[3] + obj_corners[1], Scalar(0, 255, 0), 4);
	//line(result, scene_corners[3] + obj_corners[1], scene_corners[0] + obj_corners[1], Scalar(0, 255, 0), 4);

	//這是自己算的
	line(result, computed_corners[0] + obj_corners[1], computed_corners[1] + obj_corners[1], Scalar(0, 255, 255), 4);
	line(result, computed_corners[1] + obj_corners[1], computed_corners[2] + obj_corners[1], Scalar(0, 255, 255), 4);
	line(result, computed_corners[2] + obj_corners[1], computed_corners[3] + obj_corners[1], Scalar(0, 255, 255), 4);
	line(result, computed_corners[3] + obj_corners[1], computed_corners[0] + obj_corners[1], Scalar(0, 255, 255), 4);

	imshow("haha", result);
	imwrite("result.jpg", result);
	waitKey(1);
}

Point2f MatMulti(Mat matrix, Point2f point)//看看跟source code一不一樣
{
	double input[3] = { point.x, point.y, 1 };

	vector < double > output(3);

	for (int row = 0; row < 3; ++row){
		double sum = 0;
		for (int col = 0; col < 3; ++col){
			sum += matrix.at< double >(row, col) * input[col];
		}
		output[row] = sum;
	}
	//for (int i = 0; i < 3; ++i)
	//	cout << output[i] << "\n";
	
	if (output[2] == 0.0)
		return Point2f(0, 0);
	else
		return Point2f((int)(output[0] / output[2]), (int)(output[1] / output[2]));
}

Mat concat3Img(Mat& t1, Mat& t2, Mat& i1)
{
	//cout << i1.type();
	Mat i3(max(t1.rows + t2.rows, i1.rows), max(t1.cols, t2.cols) + i1.cols, i1.type(), Scalar(0, 0, 0));

	for (int row = 0; row < i3.rows; ++row)
		for (int col = 0; col < i3.cols; ++col){
			if (col < t1.cols && row < t1.rows)//用左邊的複製
				i3.at<Vec3b>(row, col) = t1.at<Vec3b>(row, col);
			else if (row >= t1.rows && col < t2.cols)
				i3.at<Vec3b>(row, col) = t2.at<Vec3b>(row - t1.rows, col);
			else if (col >= max(t1.cols, t2.cols))
				i3.at<Vec3b>(row, col) = i1.at<Vec3b>(row, col - max(t1.cols, t2.cols));
		}
	return i3;
}

Mat concat2Img(Mat& i1, Mat& i2)
{	
	//cout << i1.type();
	Mat i3(max(i1.rows, i2.rows), i1.cols + i2.cols, i1.type(), Scalar(0, 0, 0));

	for (int row = 0; row < i3.rows; ++row)
		for (int col = 0; col < i3.cols; ++col){
			if (col < i1.cols && row < i1.rows)//用左邊的複製
				i3.at<Vec3b>(row, col) = i1.at<Vec3b>(row, col);
			else if (col >= i1.cols)
				i3.at<Vec3b>(row, col) = i2.at<Vec3b>(row, col - i1.cols);
		}
	return i3;
}

void mySIFT::computeDescriptor()
{
	int maxElement = 0;

	for (int i = 0; i < keyPoints.size(); ++i){//對每個keypoint要算出一個descriptor(先試試看32維)
		Key_Point& kpt = keyPoints[i];
		int kptRow = kpt.row;
		int kptCol = kpt.col;
		vector< double > temp;
		
		//左上4x4
		temp = DescriptorHelper(kptRow - 3, kptCol - 3, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);
		//右上4x4
		temp = DescriptorHelper(kptRow - 3, kptCol, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);
		//左下4x4
		temp = DescriptorHelper(kptRow, kptCol - 3, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);
		//右下4x4
		temp = DescriptorHelper(kptRow, kptCol, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);

		for (int j = 0; j < 32; ++j){
			if (kpt.descriptor[j] > maxElement)
				maxElement = kpt.descriptor[j];
		}
	}

	cout << "maxElement : " << maxElement << "\n";
}

void mySIFT::computeDescriptor_5x5()
{
	int maxElement = 0;

	for (int i = 0; i < keyPoints.size(); ++i){//對每個keypoint要算出一個descriptor(先試試看32維)
		Key_Point& kpt = keyPoints[i];
		int kptRow = kpt.row;
		int kptCol = kpt.col;
		vector< double > temp;

		//左上5x5
		temp = DescriptorHelper_5x5(kptRow - 4, kptCol - 4, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);
		//右上5x5
		temp = DescriptorHelper_5x5(kptRow - 4, kptCol, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);
		//左下5x5
		temp = DescriptorHelper_5x5(kptRow, kptCol - 4, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);
		//右下5x5
		temp = DescriptorHelper_5x5(kptRow, kptCol, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);

		for (int j = 0; j < 32; ++j){
			if (kpt.descriptor[j] > maxElement)
				maxElement = kpt.descriptor[j];
		}
	}

	cout << "maxElement : " << maxElement << "\n";
}

void mySIFT::computeDescriptor_6x6()
{
	int maxElement = 0;

	for (int i = 0; i < keyPoints.size(); ++i){//對每個keypoint要算出一個descriptor(先試試看32維)
		Key_Point& kpt = keyPoints[i];
		int kptRow = kpt.row;
		int kptCol = kpt.col;
		vector< double > temp;

		//左上5x5
		temp = DescriptorHelper_5x5(kptRow - 5, kptCol - 5, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);
		//右上5x5
		temp = DescriptorHelper_5x5(kptRow - 5, kptCol, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);
		//左下5x5
		temp = DescriptorHelper_5x5(kptRow, kptCol - 5, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);
		//右下5x5
		temp = DescriptorHelper_5x5(kptRow, kptCol, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);

		for (int j = 0; j < 32; ++j){
			if (kpt.descriptor[j] > maxElement)
				maxElement = kpt.descriptor[j];
		}
	}

	cout << "maxElement : " << maxElement << "\n";
}

void mySIFT::computeDescriptor_7x7()
{
	int maxElement = 0;

	for (int i = 0; i < keyPoints.size(); ++i){//對每個keypoint要算出一個descriptor(先試試看32維)
		Key_Point& kpt = keyPoints[i];
		int kptRow = kpt.row;
		int kptCol = kpt.col;
		vector< double > temp;

		//左上5x5
		temp = DescriptorHelper_5x5(kptRow - 6, kptCol - 6, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);
		//右上5x5
		temp = DescriptorHelper_5x5(kptRow - 6, kptCol, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);
		//左下5x5
		temp = DescriptorHelper_5x5(kptRow, kptCol - 6, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);
		//右下5x5
		temp = DescriptorHelper_5x5(kptRow, kptCol, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);

		for (int j = 0; j < 32; ++j){
			if (kpt.descriptor[j] > maxElement)
				maxElement = kpt.descriptor[j];
		}
	}

	cout << "maxElement : " << maxElement << "\n";
}

void mySIFT::computeDescriptor_8x8()
{
	int maxElement = 0;

	for (int i = 0; i < keyPoints.size(); ++i){//對每個keypoint要算出一個descriptor(先試試看32維)
		Key_Point& kpt = keyPoints[i];
		int kptRow = kpt.row;
		int kptCol = kpt.col;
		vector< double > temp;

		//左上5x5
		temp = DescriptorHelper_5x5(kptRow - 7, kptCol - 7, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);
		//右上5x5
		temp = DescriptorHelper_5x5(kptRow - 7, kptCol, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);
		//左下5x5
		temp = DescriptorHelper_5x5(kptRow, kptCol - 7, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);
		//右下5x5
		temp = DescriptorHelper_5x5(kptRow, kptCol, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);

		for (int j = 0; j < 32; ++j){
			if (kpt.descriptor[j] > maxElement)
				maxElement = kpt.descriptor[j];
		}
	}

	cout << "maxElement : " << maxElement << "\n";
}

void mySIFT::computeDescriptor_8x8_128dim()
{
	int maxElement = 0;

	for (int i = 0; i < keyPoints.size(); ++i){//對每個keypoint要算出一個descriptor(先試試看32維)
		Key_Point& kpt = keyPoints[i];
		int kptRow = kpt.row;
		int kptCol = kpt.col;
		vector< double > temp;

		//第一列4個
		temp = DescriptorHelper(kptRow - 7, kptCol - 7, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);
		
		temp = DescriptorHelper(kptRow - 7, kptCol - 3, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);
		
		temp = DescriptorHelper(kptRow - 7, kptCol, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);
		
		temp = DescriptorHelper(kptRow - 7, kptCol + 4, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);

		//第二列4個
		temp = DescriptorHelper(kptRow - 3, kptCol - 7, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);

		temp = DescriptorHelper(kptRow - 3, kptCol - 3, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);

		temp = DescriptorHelper(kptRow - 3, kptCol, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);

		temp = DescriptorHelper(kptRow - 3, kptCol + 4, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);

		//第三列4個
		temp = DescriptorHelper(kptRow, kptCol - 7, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);

		temp = DescriptorHelper(kptRow, kptCol - 3, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);

		temp = DescriptorHelper(kptRow, kptCol, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);

		temp = DescriptorHelper(kptRow, kptCol + 4, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);

		//第四列4個
		temp = DescriptorHelper(kptRow + 4, kptCol - 7, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);

		temp = DescriptorHelper(kptRow + 4, kptCol - 3, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);

		temp = DescriptorHelper(kptRow + 4, kptCol, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);

		temp = DescriptorHelper(kptRow + 4, kptCol + 4, kpt.layer);
		for (int j = 0; j < 8; ++j)
			kpt.descriptor.push_back(temp[j]);

		for (int j = 0; j < 128; ++j){
			if (kpt.descriptor[j] > maxElement)
				maxElement = kpt.descriptor[j];
		}
	}

	cout << "maxElement : " << maxElement << "\n";
}

vector< double > mySIFT::DescriptorHelper(int Row, int Col, int layer)//左上角row, column
{
	Mat& image = blurredImgs[layer];
	vector< double > bins(8, 0);

	for (int r = Row; r <= Row + 3; ++r)
		for (int c = Col; c <= Col + 3; ++c){
			if (r >= 1 && c >= 1 && r < image.rows - 1 && c < image.cols - 1){
				int temp1 = (int)image.at<uchar>(r, c + 1) - (int)image.at<uchar>(r, c - 1);
				int temp2 = (int)image.at<uchar>(r + 1, c) - (int)image.at<uchar>(r - 1, c);//pixel值的差
				double magnitude = abs(temp1) + abs(temp2);//sqrt(temp1 * temp1 + temp2 * temp2);
				double theta = atan2(temp2, temp1) * 180 / PI;
				if (theta < 0)
					theta += 360;
				
				//int offset = ((int)theta % 45 < 23) ? 0 : 1;
				//bins[((int)theta / 45 + offset) % 8] +=  magnitude;
				bins[(int)theta / 45] += magnitude;// 9/6改的
			}
		}
	return bins;
}

vector< double > mySIFT::DescriptorHelper_5x5(int Row, int Col, int layer)//左上角row, column
{
	Mat& image = blurredImgs[layer];
	vector< double > bins(8, 0);

	for (int r = Row; r <= Row + 4; ++r)
		for (int c = Col; c <= Col + 4; ++c){
			if (r >= 1 && c >= 1 && r < image.rows - 1 && c < image.cols - 1){
				int temp1 = (int)image.at<uchar>(r, c + 1) - (int)image.at<uchar>(r, c - 1);
				int temp2 = (int)image.at<uchar>(r + 1, c) - (int)image.at<uchar>(r - 1, c);//pixel值的差
				double magnitude = abs(temp1) + abs(temp2);//sqrt(temp1 * temp1 + temp2 * temp2);
				double theta = atan2(temp2, temp1) * 180 / PI;
				if (theta < 0)
					theta += 360;

				//int offset = ((int)theta % 45 < 23) ? 0 : 1;
				//bins[((int)theta / 45 + offset) % 8] +=  magnitude;
				bins[(int)theta / 45] += magnitude;// 9/6改的
			}
		}
	return bins;
}

vector< double > mySIFT::DescriptorHelper_6x6(int Row, int Col, int layer)
{
	Mat& image = blurredImgs[layer];
	vector< double > bins(8, 0);

	for (int r = Row; r <= Row + 5; ++r)
		for (int c = Col; c <= Col + 5; ++c){
			if (r >= 1 && c >= 1 && r < image.rows - 1 && c < image.cols - 1){
				int temp1 = (int)image.at<uchar>(r, c + 1) - (int)image.at<uchar>(r, c - 1);
				int temp2 = (int)image.at<uchar>(r + 1, c) - (int)image.at<uchar>(r - 1, c);//pixel值的差
				double magnitude = abs(temp1) + abs(temp2);//sqrt(temp1 * temp1 + temp2 * temp2);
				double theta = atan2(temp2, temp1) * 180 / PI;
				if (theta < 0)
					theta += 360;

				//int offset = ((int)theta % 45 < 23) ? 0 : 1;
				//bins[((int)theta / 45 + offset) % 8] +=  magnitude;
				bins[(int)theta / 45] += magnitude;// 9/6改的
			}
		}
	return bins;
}

vector< double > mySIFT::DescriptorHelper_7x7(int Row, int Col, int layer)
{
	Mat& image = blurredImgs[layer];
	vector< double > bins(8, 0);

	for (int r = Row; r <= Row + 6; ++r)
		for (int c = Col; c <= Col + 6; ++c){
			if (r >= 1 && c >= 1 && r < image.rows - 1 && c < image.cols - 1){
				int temp1 = (int)image.at<uchar>(r, c + 1) - (int)image.at<uchar>(r, c - 1);
				int temp2 = (int)image.at<uchar>(r + 1, c) - (int)image.at<uchar>(r - 1, c);//pixel值的差
				double magnitude = abs(temp1) + abs(temp2);//sqrt(temp1 * temp1 + temp2 * temp2);
				double theta = atan2(temp2, temp1) * 180 / PI;
				if (theta < 0)
					theta += 360;

				//int offset = ((int)theta % 45 < 23) ? 0 : 1;
				//bins[((int)theta / 45 + offset) % 8] +=  magnitude;
				bins[(int)theta / 45] += magnitude;// 9/6改的
			}
		}
	return bins;
}

vector< double > mySIFT::DescriptorHelper_8x8(int Row, int Col, int layer)
{
	Mat& image = blurredImgs[layer];
	vector< double > bins(8, 0);

	for (int r = Row; r <= Row + 7; ++r)
		for (int c = Col; c <= Col + 7; ++c){
			if (r >= 1 && c >= 1 && r < image.rows - 1 && c < image.cols - 1){
				int temp1 = (int)image.at<uchar>(r, c + 1) - (int)image.at<uchar>(r, c - 1);
				int temp2 = (int)image.at<uchar>(r + 1, c) - (int)image.at<uchar>(r - 1, c);//pixel值的差
				double magnitude = abs(temp1) + abs(temp2);//sqrt(temp1 * temp1 + temp2 * temp2);
				double theta = atan2(temp2, temp1) * 180 / PI;
				if (theta < 0)
					theta += 360;

				//int offset = ((int)theta % 45 < 23) ? 0 : 1;
				//bins[((int)theta / 45 + offset) % 8] +=  magnitude;
				bins[(int)theta / 45] += magnitude;// 9/6改的
			}
		}
	return bins;
}

void mySIFT::drawKeyPoints(string imageName)
{
	Mat imgOriRGB = imread(imageName);
	for (int i = 0; i < keyPoints.size(); ++i){
		Key_Point& kpt = keyPoints[i];
		double scaling = 1;
		int scaleNum = kpt.layer / nLayersPerOctave;
		for (int j = 0; j < scaleNum; ++j)
			scaling *= 1.6;
		circle(imgOriRGB, Point(kpt.col * scaling, kpt.row * scaling), 2, Scalar(0, 0, 255));
		//line(imgOriRGB, Point(kpt.col, kpt.row), Point(kpt.col + 10 * kpt.scale * cos(kpt.orientation * PI / 180), kpt.row + 10 * kpt.scale * sin(kpt.orientation * PI / 180)), Scalar(203, 192, 255));
	}
	//line(Mat, Point, Point, Scalar)
	//imshow("Output", blurredImgs[0]);
	cout << keyPoints.size() << "\n";
	imshow("Ori", imgOriRGB);
	waitKey(0);
}

void mySIFT::filterKeyPoints2()
{
	for (int i = 0; i < keyPoints.size(); ++i){
		Key_Point& thisKpt = keyPoints[i];
		Mat& thisMat = blurredImgs[thisKpt.layer];
		int Dxx = thisMat.at<uchar>(thisKpt.row, thisKpt.col - 1) + thisMat.at<uchar>(thisKpt.row, thisKpt.col + 1) - 2 * thisMat.at<uchar>(thisKpt.row, thisKpt.col);
		int Dyy = thisMat.at<uchar>(thisKpt.row - 1, thisKpt.col) + thisMat.at<uchar>(thisKpt.row + 1, thisKpt.col) - 2 * thisMat.at<uchar>(thisKpt.row, thisKpt.col);
		float Dxy = (thisMat.at<uchar>(thisKpt.row + 1, thisKpt.col - 1) + thisMat.at<uchar>(thisKpt.row - 1, thisKpt.col + 1) - thisMat.at<uchar>(thisKpt.row - 1, thisKpt.col - 1) - thisMat.at<uchar>(thisKpt.row + 1, thisKpt.col + 1)) / 4.0;
		float denominator = (Dxx * Dyy - Dxy * Dxy);
		if (denominator == 0.0)
			thisKpt.R = INT_MAX;
		else
			thisKpt.R = (Dxx + Dyy) * (Dxx + Dyy) / (Dxx * Dyy - Dxy * Dxy);
	}
	for (int i = 1; i < keyPoints.size(); ++i)
		for (int j = i - 1; j >= 0; --j){
			if (keyPoints[j + 1].R > keyPoints[j].R){
				swap(keyPoints[j + 1], keyPoints[j]);
			}
		}
	vector< Key_Point > temp;
	int index = 0;
	for (int i = 0; i < keyPoints.size(); ++i){
		if (keyPoints[i].R != INT_MAX){
			cout << keyPoints[i].R << "\n";
			temp.push_back(keyPoints[i]);
			++index;
			if (index == 500)
				break;
		}
	}
	keyPoints = temp;
}

void mySIFT::filterKeyPoints_FAST_16()
{
	cout << "Using FAST 16\n";
	for (int i = 0; i < keyPoints.size(); ++i){
		Key_Point& thisKpt = keyPoints[i];
		Mat& thisMat = blurredImgs[thisKpt.layer];
		//有n1個連續的pixel都比thisKpt深，有n2個連續的點都比thisKpt淺
		if (thisKpt.row - 3 < 0 || thisKpt.row + 3 >= thisMat.rows || thisKpt.col - 3 < 0 || thisKpt.col + 3 >= thisMat.cols)//檢查範圍，不能夠畫整圈的就直接掰
			continue;
		vector< int > brighter(16, 0);
		vector< int > darker(16, 0);
		int value = thisMat.at<uchar>(thisKpt.row, thisKpt.col);
		int threshold = FAST_16_THRESHOLD;
		filterKeyPoints_FAST_16_Helper1(brighter, darker, thisMat, thisKpt, value, threshold);
		int nBrighter, nDarker;
		filterKeyPoints_FAST_16_Helper2(brighter, darker, nBrighter, nDarker);
		thisKpt.cornerValue = max(nBrighter, nDarker);
		if (thisKpt.cornerValue > 16 || thisKpt.cornerValue < 0)
			cout << "??\n";
	}

	//做sorting摟，大的排前面
	//for (int i = 1; i < keyPoints.size(); ++i)
	//	for (int j = i - 1; j >= 0; --j)
	//		if (keyPoints[j + 1].cornerValue > keyPoints[j].cornerValue)
	//			swap(keyPoints[j + 1], keyPoints[j]);
	//vector< Key_Point > temp(keyPoints.begin(), (keyPoints.size() < 500) ? keyPoints.end() : keyPoints.begin() + 500);
	//cout << keyPoints.size() << " ";
	vector< Key_Point > temp;
	for (int i = 0; i < keyPoints.size(); ++i){
		if (keyPoints[i].cornerValue >= 8)
			temp.push_back(keyPoints[i]);
	}
	cout << temp.size() << "\n";
	keyPoints = temp;
}

void mySIFT::filterKeyPoints_FAST_16_Helper1(vector< int >& brighter, vector< int >& darker, Mat& thisMat, Key_Point& thisKpt, int value, int threshold)
{
	//弄brighter
	int brighterthshod = value + threshold;
	if (thisMat.at<uchar>(thisKpt.row - 3, thisKpt.col) > brighterthshod)
		brighter[0] = 1;
	if (thisMat.at<uchar>(thisKpt.row - 3, thisKpt.col + 1) > brighterthshod)
		brighter[1] = 1;
	if (thisMat.at<uchar>(thisKpt.row - 2, thisKpt.col + 2) > brighterthshod)
		brighter[2] = 1;
	if (thisMat.at<uchar>(thisKpt.row - 1, thisKpt.col + 3) > brighterthshod)
		brighter[3] = 1;
	if (thisMat.at<uchar>(thisKpt.row, thisKpt.col + 3) > brighterthshod)
		brighter[4] = 1;
	if (thisMat.at<uchar>(thisKpt.row + 1, thisKpt.col + 3) > brighterthshod)
		brighter[5] = 1;
	if (thisMat.at<uchar>(thisKpt.row + 2, thisKpt.col + 2) > brighterthshod)
		brighter[6] = 1;
	if (thisMat.at<uchar>(thisKpt.row + 3, thisKpt.col + 1) > brighterthshod)
		brighter[7] = 1;
	if (thisMat.at<uchar>(thisKpt.row + 3, thisKpt.col) > brighterthshod)
		brighter[8] = 1;
	if (thisMat.at<uchar>(thisKpt.row + 3, thisKpt.col - 1) > brighterthshod)
		brighter[9] = 1;
	if (thisMat.at<uchar>(thisKpt.row + 2, thisKpt.col - 2) > brighterthshod)
		brighter[10] = 1;
	if (thisMat.at<uchar>(thisKpt.row + 1, thisKpt.col - 3) > brighterthshod)
		brighter[11] = 1;
	if (thisMat.at<uchar>(thisKpt.row, thisKpt.col - 3) > brighterthshod)
		brighter[12] = 1;
	if (thisMat.at<uchar>(thisKpt.row - 1, thisKpt.col - 3) > brighterthshod)
		brighter[13] = 1;
	if (thisMat.at<uchar>(thisKpt.row - 2, thisKpt.col - 2) > brighterthshod)
		brighter[14] = 1;
	if (thisMat.at<uchar>(thisKpt.row - 3, thisKpt.col - 1) > brighterthshod)
		brighter[15] = 1;

	//弄darker
	int darkererthshod = value - threshold;
	if (thisMat.at<uchar>(thisKpt.row - 3, thisKpt.col) < darkererthshod)
		darker[0] = 1;
	if (thisMat.at<uchar>(thisKpt.row - 3, thisKpt.col + 1) < darkererthshod)
		darker[1] = 1;
	if (thisMat.at<uchar>(thisKpt.row - 2, thisKpt.col + 2) < darkererthshod)
		darker[2] = 1;
	if (thisMat.at<uchar>(thisKpt.row - 1, thisKpt.col + 3) < darkererthshod)
		darker[3] = 1;
	if (thisMat.at<uchar>(thisKpt.row, thisKpt.col + 3) < darkererthshod)
		darker[4] = 1;
	if (thisMat.at<uchar>(thisKpt.row + 1, thisKpt.col + 3) < darkererthshod)
		darker[5] = 1;
	if (thisMat.at<uchar>(thisKpt.row + 2, thisKpt.col + 2) < darkererthshod)
		darker[6] = 1;
	if (thisMat.at<uchar>(thisKpt.row + 3, thisKpt.col + 1) < darkererthshod)
		darker[7] = 1;
	if (thisMat.at<uchar>(thisKpt.row + 3, thisKpt.col) < darkererthshod)
		darker[8] = 1;
	if (thisMat.at<uchar>(thisKpt.row + 3, thisKpt.col - 1) < darkererthshod)
		darker[9] = 1;
	if (thisMat.at<uchar>(thisKpt.row + 2, thisKpt.col - 2) < darkererthshod)
		darker[10] = 1;
	if (thisMat.at<uchar>(thisKpt.row + 1, thisKpt.col - 3) < darkererthshod)
		darker[11] = 1;
	if (thisMat.at<uchar>(thisKpt.row, thisKpt.col - 3) < darkererthshod)
		darker[12] = 1;
	if (thisMat.at<uchar>(thisKpt.row - 1, thisKpt.col - 3) < darkererthshod)
		darker[13] = 1;
	if (thisMat.at<uchar>(thisKpt.row - 2, thisKpt.col - 2) < darkererthshod)
		darker[14] = 1;
	if (thisMat.at<uchar>(thisKpt.row - 3, thisKpt.col - 1) < darkererthshod)
		darker[15] = 1;
}

void mySIFT::filterKeyPoints_FAST_16_Helper2(vector< int >& brighter, vector< int >& darker, int& nBrighter, int& nDarker)
{
	for (int i = 0; i < 8; ++i)
		brighter.push_back(brighter[i]);
	
	int accu1 = 0;
	int maxAccu1 = 0;
	for (int i = 0; i < 16; ++i){
		if (brighter[i]){
			++accu1;
			if (accu1 > maxAccu1)
				++maxAccu1;
		}
		else
			accu1 = 0;//歸0
	}
	nBrighter = maxAccu1;

	accu1 = 0;
	maxAccu1 = 0;
	for (int i = 8; i < 24; ++i){
		if (brighter[i]){
			++accu1;
			if (accu1 > maxAccu1)
				++maxAccu1;
		}
		else
			accu1 = 0;
	}
	if (maxAccu1 > nBrighter)
		nBrighter = maxAccu1;

	for (int i = 0; i < 8; ++i)
		darker.push_back(darker[i]);

	accu1 = 0;
	maxAccu1 = 0;
	for (int i = 0; i < 16; ++i){
		if (darker[i]){
			++accu1;
			if (accu1 > maxAccu1)
				++maxAccu1;
		}
		else
			accu1 = 0;//歸0
	}
	nDarker = maxAccu1;

	accu1 = 0;
	maxAccu1 = 0;
	for (int i = 8; i < 24; ++i){
		if (darker[i]){
			++accu1;
			if (accu1 > maxAccu1)
				++maxAccu1;
		}
		else
			accu1 = 0;
	}
	if (maxAccu1 > nDarker)
		nDarker = maxAccu1;
}

void mySIFT::filterKeyPoints_FAST_8()
{
	//cout << "Using FAST 8\n";
	for (int i = 0; i < keyPoints.size(); ++i){
		Key_Point& thisKpt = keyPoints[i];
		Mat& thisMat = blurredImgs[thisKpt.layer];
		//有n1個連續的pixel都比thisKpt深，有n2個連續的點都比thisKpt淺
		
		//在DoG上得到的keyPoint不會在邊界
		//if (thisKpt.row - 3 < 0 || thisKpt.row + 3 >= thisMat.rows || thisKpt.col - 3 < 0 || thisKpt.col + 3 >= thisMat.cols)//檢查範圍，不能夠畫整圈的就直接掰
		//	continue;
		vector< int > brighter(8, 0);
		vector< int > darker(8, 0);
		int value = thisMat.at<uchar>(thisKpt.row, thisKpt.col);
		int threshold = FAST_8_THRESHOLD;
		filterKeyPoints_FAST_8_Helper1(brighter, darker, thisMat, thisKpt, value, threshold);
		int nBrighter, nDarker;
		filterKeyPoints_FAST_8_Helper2(brighter, darker, nBrighter, nDarker);
		thisKpt.cornerValue = max(nBrighter, nDarker);
		if (thisKpt.cornerValue > 8 || thisKpt.cornerValue < 0)
			cout << "??\n";
	}

	//做sorting摟，大的排前面
	//for (int i = 1; i < keyPoints.size(); ++i)
	//	for (int j = i - 1; j >= 0; --j)
	//		if (keyPoints[j + 1].cornerValue > keyPoints[j].cornerValue)
	//			swap(keyPoints[j + 1], keyPoints[j]);
	//vector< Key_Point > temp(keyPoints.begin(), (keyPoints.size() < 500) ? keyPoints.end() : keyPoints.begin() + 500);
	//cout << keyPoints.size() << " ";
	vector< Key_Point > temp;
	int layer1 = 0;
	int layer2 = 0;
	for (int i = 0; i < keyPoints.size(); ++i){
		if (keyPoints[i].cornerValue >= 4){
			temp.push_back(keyPoints[i]);
			if (keyPoints[i].layer == 1)
				++layer1;
			else if (keyPoints[i].layer == 2)
				++layer2;
			else{
				if (this->blurredImgs.size() == 5)
					cout << "FUCK\n";
			}
		}
	}
	//cout << temp.size() << " " << layer1 << " " << layer2 << "\n";
	keyPoints = temp;
}

void mySIFT::filterKeyPoints_FAST_8_Helper1(vector< int >& brighter, vector< int >& darker, Mat& thisMat, Key_Point& thisKpt, int value, int threshold)
{
	//弄brighter
	int brighterthshod = value + threshold;
	if (thisMat.at<uchar>(thisKpt.row - 1, thisKpt.col - 1) > brighterthshod)
		brighter[0] = 1;
	if (thisMat.at<uchar>(thisKpt.row - 1, thisKpt.col) > brighterthshod)
		brighter[1] = 1;
	if (thisMat.at<uchar>(thisKpt.row - 1, thisKpt.col + 1) > brighterthshod)
		brighter[2] = 1;
	if (thisMat.at<uchar>(thisKpt.row, thisKpt.col + 1) > brighterthshod)
		brighter[3] = 1;
	if (thisMat.at<uchar>(thisKpt.row + 1, thisKpt.col + 1) > brighterthshod)
		brighter[4] = 1;
	if (thisMat.at<uchar>(thisKpt.row + 1, thisKpt.col) > brighterthshod)
		brighter[5] = 1;
	if (thisMat.at<uchar>(thisKpt.row + 1, thisKpt.col - 1) > brighterthshod)
		brighter[6] = 1;
	if (thisMat.at<uchar>(thisKpt.row, thisKpt.col - 1) > brighterthshod)
		brighter[7] = 1;

	//弄darker
	int darkerthshod = value - threshold;
	if (thisMat.at<uchar>(thisKpt.row - 1, thisKpt.col - 1) < darkerthshod)
		darker[0] = 1;
	if (thisMat.at<uchar>(thisKpt.row - 1, thisKpt.col) < darkerthshod)
		darker[1] = 1;
	if (thisMat.at<uchar>(thisKpt.row - 1, thisKpt.col + 1) < darkerthshod)
		darker[2] = 1;
	if (thisMat.at<uchar>(thisKpt.row, thisKpt.col + 1) < darkerthshod)
		darker[3] = 1;
	if (thisMat.at<uchar>(thisKpt.row + 1, thisKpt.col + 1) < darkerthshod)
		darker[4] = 1;
	if (thisMat.at<uchar>(thisKpt.row + 1, thisKpt.col) < darkerthshod)
		darker[5] = 1;
	if (thisMat.at<uchar>(thisKpt.row + 1, thisKpt.col - 1) < darkerthshod)
		darker[6] = 1;
	if (thisMat.at<uchar>(thisKpt.row, thisKpt.col - 1) < darkerthshod)
		darker[7] = 1;
}

void mySIFT::filterKeyPoints_FAST_8_Helper2(vector< int >& brighter, vector< int >& darker, int& nBrighter, int& nDarker)
{
	if (brighter.size() != 8 || darker.size() != 8)
		cout << "?????\n";

	for (int i = 0; i < 4; ++i)
		brighter.push_back(brighter[i]);

	int accu1 = 0;
	int maxAccu1 = 0;
	for (int i = 0; i < 8; ++i){
		if (brighter[i]){
			++accu1;
			if (accu1 > maxAccu1)
				++maxAccu1;
		}
		else
			accu1 = 0;//歸0
	}
	nBrighter = maxAccu1;

	accu1 = 0;
	maxAccu1 = 0;
	for (int i = 4; i < 12; ++i){
		if (brighter[i]){
			++accu1;
			if (accu1 > maxAccu1)
				++maxAccu1;
		}
		else
			accu1 = 0;
	}
	if (maxAccu1 > nBrighter)
		nBrighter = maxAccu1;

	for (int i = 0; i < 4; ++i)
		darker.push_back(darker[i]);

	accu1 = 0;
	maxAccu1 = 0;
	for (int i = 0; i < 8; ++i){
		if (darker[i]){
			++accu1;
			if (accu1 > maxAccu1)
				++maxAccu1;
		}
		else
			accu1 = 0;//歸0
	}
	nDarker = maxAccu1;

	accu1 = 0;
	maxAccu1 = 0;
	for (int i = 4; i < 12; ++i){
		if (darker[i]){
			++accu1;
			if (accu1 > maxAccu1)
				++maxAccu1;
		}
		else
			accu1 = 0;
	}
	if (maxAccu1 > nDarker)
		nDarker = maxAccu1;
}

void mySIFT::computeOrientationHist(vector< Key_Point >& keyPoints)// Computes a gradient orientation histogram at a specified pixel
{

	for (int i = 0; i < keyPoints.size(); ++i){//對每個keypoint算方向
		vector< double > bins(SIFT_ORI_HIST_BINS, 0);
		Key_Point& thiskpt = keyPoints[i];
		Mat& image = blurredImgs[thiskpt.layer];
		int kptrow = thiskpt.row;
		int kptcol = thiskpt.col;
		//int temp1 = (int)image.at<uchar>(kptrow, kptcol + 1) - (int)image.at<uchar>(kptrow, kptcol - 1);
		//int temp2 = (int)image.at<uchar>(kptrow + 1, kptcol) - (int)image.at<uchar>(kptrow - 1, kptcol);
		//double magnitude = sqrt(temp1 * temp1 + temp2 * temp2);
		//double theta = atan2(temp2, temp1) * 180 / PI;
		//cout << setprecision(2) << fixed << setw(5) << magnitude << " " << theta << "\n";
		//system("pause");
		//double weightSum = 0;
		//int row = kptrow - SIFT_ORI_RADIUS;
		for (int row = kptrow - SIFT_ORI_RADIUS; row <= kptrow + SIFT_ORI_RADIUS; ++row)
			for (int col = kptcol - SIFT_ORI_RADIUS; col <= kptcol + SIFT_ORI_RADIUS; ++col){
				if (row >= 1 && row < image.rows - 1 && col >= 1 && col < image.cols - 1 && (row - kptrow) * (row - kptrow) + (col - kptcol) * (col - kptcol) < SIFT_ORI_RADIUS * SIFT_ORI_RADIUS){
					//在範圍內
					int temp1 = (int)image.at<uchar>(row, col + 1) - (int)image.at<uchar>(row, col - 1);
					int temp2 = (int)image.at<uchar>(row + 1, col) - (int)image.at<uchar>(row - 1, col);//pixel值的差
					double magnitude = sqrt(temp1 * temp1 + temp2 * temp2);
					double theta = atan2(temp2, temp1) * 180 / PI;
					if (theta < 0)
						theta += 360;
					double weight = exp(-0.5 * ((row - kptrow) * (row - kptrow) + (col - kptcol) * (col - kptcol)) / (SIFT_ORI_SIG * SIFT_ORI_SIG));
					int offset = ((int)theta % 10 < 5)? 0 : 1;
					bins[((int)theta / 10 + offset) % 36] += magnitude * weight;
					//weightSum += weight;
				}
			}
		int index = 0;
		for (int i = 1; i < SIFT_ORI_HIST_BINS; ++i)
			if (bins[i] > bins[index])
				index = i;

		thiskpt.orientation = index * 10;
	}
}

void mySIFT::detectKeypoints()
{
	int size = blurredImgs.size() / nOctave - 1;//每一個octave，DoG的size，現在是4
	double sigmaTemp = sigma;

	for (int octave = 0; octave < nOctave; ++octave){	
		for (int layer = octave * nLayersPerOctave + 1; layer < octave * nLayersPerOctave + size - 1; ++layer){//DoG的layer
			//cout << octave << " " << layer << "\n";
			for (int row = 8; row <= DoGs[layer].rows - 9; ++row){//octave * nLayersPerOctave
				for (int col = 8; col <= DoGs[layer].cols - 9; ++col){//看DoGs[layer]那張的[row][col]那點是不是max min
					int value = DoGs[layer].at<int>(row, col);
					//cout << "value : " << value << " at " << row << ", " << col << "\n";
					bool isMax = true;
					bool isMin = true;
					
					for (int layerOffset = -1; layerOffset <= 1 && (isMin || isMax); ++layerOffset){
						for (int i = row - (DOG_DETECT_KPT_SIZE - 1) / 2; i <= row + (DOG_DETECT_KPT_SIZE - 1) / 2 && (isMin || isMax); ++i){
							for (int j = col - (DOG_DETECT_KPT_SIZE - 1) / 2; j <= col + (DOG_DETECT_KPT_SIZE - 1) / 2 && (isMin || isMax); ++j){
								if (i >= 0 && j >= 0 && i < DoGs[octave * nLayersPerOctave].rows && j < DoGs[octave * nLayersPerOctave].cols && abs(i - row) + abs(j - col) + abs(layerOffset) == 2){//原本最後兩項是這樣  !(layerOffset == 0 && i == row && j == col) i != row && j != col
									int tester = DoGs[layer + layerOffset].at<int>(i, j);
									if (tester - 0 >= value)
										isMax = false;
									if (tester + 0 <= value)
										isMin = false;
								}
							}
						}
					}
					if (isMax)
						keyPoints.push_back(Key_Point(sigmaTemp, row, col, 1, layer));
					if (isMin)
						keyPoints.push_back(Key_Point(sigmaTemp, row, col, 0, layer));
				}
			}
			sigmaTemp *= k;
		}
	}
	//cout << keyPoints.size() << "\n";
	
}

void mySIFT::createDoG()
{
	double tempSigma = sigma;

	for (int j = 0; j < nOctave; ++j){
		for (int i = 0; i < nLayersPerOctave - 1; ++i){//包含原圖的模糊圖，一個Octave5張圖，推4次，因為第一張是原圖
			blurredImgs.push_back(GaussianBlur(blurredImgs[j * nLayersPerOctave], tempSigma));
			tempSigma *= k;
			Mat DoG(blurredImgs[j * nLayersPerOctave + i + 1].rows, blurredImgs[j * nLayersPerOctave + i + 1].cols, CV_32SC1, Scalar(0));
			for (int row = 0; row < blurredImgs[j * nLayersPerOctave + i].rows; ++row)
				for (int col = 0; col < blurredImgs[j * nLayersPerOctave + i].cols; ++col){
					int diff = (int)blurredImgs[j * nLayersPerOctave + i + 1].at<uchar>(row, col) - (int)blurredImgs[j * nLayersPerOctave + i].at<uchar>(row, col);
					DoG.at<int>(row, col) = diff;
					//cout << diff << " " << DoG.at<int>(row, col) << "\n";
				}
			DoGs.push_back(DoG);
		}
		//做完一個octave
		if (j != nOctave - 1){
			tempSigma = tempSigma / k / k / k / k;
			Mat src = *(blurredImgs.end() - 4);//blurredImgs[blurredImgs.size() - 3];
			Mat firstMatInNewOctave;
			resize(src, firstMatInNewOctave, Size(src.cols / SCALING, src.rows / SCALING), 0, 0, INTER_NEAREST);
			//imshow("hey", firstMatInNewOctave);
			//waitKey(0);
			blurredImgs.push_back(firstMatInNewOctave);//推進下一層octave的第一張圖
			Mat empty;
			DoGs.push_back(empty);
		}
	}
}

double2D mySIFT::getGaussianKernel(double sigma)
{
	int kSize = cvRound(sigma * 1.5 + 1) | 1;//kSize為奇數，亂定的
	//cout << sigma << " " << kSize << " ";
	vector< double > kernel_1D;
	
	int shift = (kSize - 1) / 2;
	double sum = 0;
	
	for (int i = 0; i < kSize; ++i){
		int x = i - shift;
		double temp = exp(-0.5*(x / sigma)*(x / sigma));
		sum += temp;
		kernel_1D.push_back( temp );
	}
	
	for (int i = 0; i < kSize; ++i)
		kernel_1D[i] = kernel_1D[i] / sum;

	double2D kernel_2D;
	for (int i = 0; i < kSize; ++i){
		vector < double > a;
		kernel_2D.push_back(a);
		for (int j = 0; j < kSize; ++j)
			kernel_2D[i].push_back( kernel_1D[j] * kernel_1D[i]);
	}
	//for (int i = 0; i < kSize; ++i){
	//	for (int j = 0; j < kSize; ++j)
	//		cout << setw(6) << setprecision(2) << kernel_2D[i][j];
	//	cout << "\n";
	//}
	return kernel_2D;
}

Mat mySIFT::GaussianBlur(const Mat& src, double sigma)//input (圖片, sigma), output (模糊圖片)
{
	double2D G_Kernel = getGaussianKernel(sigma);
	int kSize = G_Kernel.size();
	//kernel罩上去，超出圖片的pixel補0
	int shift = (kSize - 1) / 2;

	Mat output = src.clone();

	for (int row = 0; row < src.rows; ++row)
		for (int col = 0; col < src.cols; ++col){//要對img[row][col]的pixel做模糊
			double sum = 0.0;
			double weightSum = 0.0;//0.0
			for (int i = row - shift; i <= row + shift; ++i)
				for (int j = col - shift; j <= col + shift; ++j){
					if (i >= 0 && j >= 0 && i < src.rows && j < src.cols){
						sum += src.at<uchar>(i, j) * G_Kernel[i - (row - shift)][j - (col - shift)];
						//weightSum += G_Kernel[i - (row - shift)][j - (col - shift)];
					}
				}
			output.at<uchar>(row, col) = sum;// / weightSum;
			//cout << "sum : " << sum << "   " << (int)output.at<uchar>(row, col) << "\n";
		}
	//cout << "Finish Blurring with sigma : " << setprecision(2) << sigma << "\n";
	return output;
}

void mySIFT::LoadImage(Mat imgOri)
{
	blurredImgs.push_back(imgOri);//原圖是放index0
}