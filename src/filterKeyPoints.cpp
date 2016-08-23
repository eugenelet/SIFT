#include "../include/sift.h"

void mySIFT::filterKeyPoints_Hessian_left(Mat img_scene, Mat img_color){
	cout << "Key_Point (BEFORE): " << keyPoints.size() << endl;
	// Mat image = img_scene;

	Mat *R = new Mat[DoGs.size()];
	for(int i = 0; i < DoGs.size(); i++){
		if(DoGs[i].rows != 0){
			Mat Dxx(DoGs[i].rows, DoGs[i].cols, CV_32FC1, Scalar(0));
			Mat Dxy(DoGs[i].rows, DoGs[i].cols, CV_32FC1, Scalar(0));
			Mat Dyy(DoGs[i].rows, DoGs[i].cols, CV_32FC1, Scalar(0));
			computeDxxDxyDyy(DoGs[i], Dxx, Dxy, Dyy);
			R[i] = Mat(DoGs[i].rows, DoGs[i].cols, CV_32FC1, Scalar(0));
			calculateR(R[i], Dxx, Dxy, Dyy);
		}
	}
/*
	Mat Dxx(image.rows, image.cols, CV_32FC1, Scalar(0));
	Mat Dxy(image.rows, image.cols, CV_32FC1, Scalar(0));
	Mat Dyy(image.rows, image.cols, CV_32FC1, Scalar(0));

	computeDxxDxyDyy(image, Dxx, Dxy, Dyy);

	Mat R(image.rows, image.cols, CV_32FC1, Scalar(0));
	calculateR(R, Dxx, Dxy, Dyy);
*/
	Mat imageColor = img_color;

	int count = 0;

	multiset< OMG > forSort;

	for (int i = 0; i < keyPoints.size(); i++){
		//cout << abs(R.at<float>(row, col)) << "\n";
		if (1){//(abs(R.at<float>(row, col)) < 0.0001){//
			Key_Point& kpt = keyPoints[i];
			//cout << R.at<float>(row, col) << " " << row << " " << col << "\n";
			//if (abs(R.at<float>(row, col)) == 0){
			//	//cout << R.at<float>(row, col) << "\n";
			//	circle(imageColor, Point(col, row), 3, Scalar(0, 0, 255), 1);
			//	zeros.insert(OMG(row, col, R.at<float>(row, col)));
			//}
			//if (row == 145 && col == 538)
			//	cout << setprecision(10) << R.at<float>(row, col) << " " << abs(R.at<float>(row, col)) << "\n";
			//++count;
			//circle(imageColor, Point(col, row), 3, Scalar(255, 0, 255), 1);
			if (R[kpt.layer].at<float>(kpt.row, kpt.col) > 0){
				forSort.insert(OMG(kpt.row, kpt.col, R[kpt.layer].at<float>(kpt.row, kpt.col), kpt));
			}
		}
	}
	int index = 0;
/*	cout << forSort.size() << "\n";
	for (multiset<OMG>::reverse_iterator p = forSort.rbegin(); p != forSort.rend(); ++p){
		//Mat tempImage = imageColor;
		if (p->Rvalue != INT_MAX && p->Rvalue != 4.0){
			circle(imageColor, Point(p->col, p->row), 3, Scalar(255, 255, 255), 1);
			imshow("??", imageColor);
			waitKey(1);
			printf("%f %d\n", p->Rvalue, index);
			++index;
			if (index == 100)
				break;
		}
	}

	index = 0;*/

	vector<Key_Point> tmp;
/*	for (multiset<OMG>::reverse_iterator p = forSort.rbegin(); p != forSort.rend(); ++p){
		//Mat tempImage = imageColor;
		if (p->Rvalue!=INT_MAX && p->Rvalue!=4.0 ){
			//circle(imageColor, Point(p->col, p->row), 3, Scalar(255, 255, 0), 1);
			//imshow("??", imageColor);
			//waitKey(1);
			//printf("%f %d\n", p->Rvalue, index);
			tmp.push_back(p->myKeyPoint);
			// cout << p->row << " " << p->col << endl;
			++index;
			if (index == 300)
				break;
		}
	}*/

	for (multiset<OMG>::reverse_iterator p = forSort.rbegin(); p != forSort.rend(); ++p){
		//Mat tempImage = imageColor;
		if(p->Rvalue!=INT_MAX && /*p->Rvalue!=4.0 &&*/ p->Rvalue<14){
			// circle(imageColor, Point(p->col, p->row), 3, Scalar(255, 255, 0), 1);
			//imshow("??", imageColor);
			//waitKey(1);
			//printf("%f %d\n", p->Rvalue, index);
			tmp.push_back(p->myKeyPoint);
			/*++index;
			if (index == 800)
				break;*/
		}
	}
		// cout << endl << "NEXT" << endl;

	keyPoints = tmp;
	/*index = 0;
	for (multiset<OMG>::iterator p = forSort.begin(); p != forSort.end(); ++p){
		//Mat tempImage = imageColor;
		if (p->Rvalue != INT_MAX && p->Rvalue != 4.0){
			circle(imageColor, Point(p->col, p->row), 3, Scalar(0, 0, 255), 1);
			imshow("??", imageColor);
			waitKey(1);
			printf("%f %d\n", p->Rvalue, index);
			++index;
			if (index == 100)
				break;
		}
	}*/
	//index = 0;
	//for (set<OMG>::reverse_iterator p = forSort.rbegin(); p != forSort.rend() && index < 300; ++p){
	//	//if (index >= 10000){
	//		circle(imageColor, Point(p->col, p->row), 3, Scalar(255, 0, 0), 1);
	//		cout << p->Rvalue << "\n";
	//	//}
	//	++index;
	//}
		
	//printf("%d\n", count);
	//imshow("??", imageColor);
	//waitKey(0);
	//imwrite("hahaha.jpg", imageColor);
}

void mySIFT::filterKeyPoints_Hessian(Mat img_scene, Mat img_color){
	cout << "Key_Point (BEFORE): " << keyPoints.size() << endl;
	// Mat image = img_scene;

	Mat *R = new Mat[DoGs.size()];
	cout << DoGs.size() << endl;
	for(int i = 0; i < DoGs.size(); i++){
		if(DoGs[i].rows != 0){
			Mat Dxx(DoGs[i].rows, DoGs[i].cols, CV_32FC1, Scalar(0));
			Mat Dxy(DoGs[i].rows, DoGs[i].cols, CV_32FC1, Scalar(0));
			Mat Dyy(DoGs[i].rows, DoGs[i].cols, CV_32FC1, Scalar(0));
			computeDxxDxyDyy(DoGs[i], Dxx, Dxy, Dyy);
			R[i] = Mat(DoGs[i].rows, DoGs[i].cols, CV_32FC1, Scalar(0));
			calculateR(R[i], Dxx, Dxy, Dyy);
		}
	}
/*
	Mat Dxx(image.rows, image.cols, CV_32FC1, Scalar(0));
	Mat Dxy(image.rows, image.cols, CV_32FC1, Scalar(0));
	Mat Dyy(image.rows, image.cols, CV_32FC1, Scalar(0));

	computeDxxDxyDyy(image, Dxx, Dxy, Dyy);

	Mat R(image.rows, image.cols, CV_32FC1, Scalar(0));
	calculateR(R, Dxx, Dxy, Dyy);
*/
	Mat imageColor = img_color;

	int count = 0;

	multiset< OMG > forSort;

	for (int i = 0; i < keyPoints.size(); i++){
		//cout << abs(R.at<float>(row, col)) << "\n";
		// if (1){//(abs(R.at<float>(row, col)) < 0.0001){//
		Key_Point& kpt = keyPoints[i];
		//cout << R[kpt.layer].at<float>(kpt.row, kpt.col) << " " << kpt.row << " " << kpt.col << "\n";
		//if (abs(R.at<float>(row, col)) == 0){
		//	//cout << R.at<float>(row, col) << "\n";
		//	circle(imageColor, Point(col, row), 3, Scalar(0, 0, 255), 1);
		//	zeros.insert(OMG(row, col, R.at<float>(row, col)));
		//}
		//if (row == 145 && col == 538)
		//	cout << setprecision(10) << R.at<float>(row, col) << " " << abs(R.at<float>(row, col)) << "\n";
		//++count;
		//circle(imageColor, Point(col, row), 3, Scalar(255, 0, 255), 1);
		// if (R[kpt.layer].at<float>(kpt.row, kpt.col) > 0){
			forSort.insert(OMG(kpt.row, kpt.col, R[kpt.layer].at<float>(kpt.row, kpt.col), kpt));
		// }
		// }
	}
	int index = 0;
/*	cout << forSort.size() << "\n";
	for (multiset<OMG>::reverse_iterator p = forSort.rbegin(); p != forSort.rend(); ++p){
		//Mat tempImage = imageColor;
		if (p->Rvalue != INT_MAX && p->Rvalue != 4.0){
			circle(imageColor, Point(p->col, p->row), 3, Scalar(255, 255, 255), 1);
			imshow("??", imageColor);
			waitKey(1);
			printf("%f %d\n", p->Rvalue, index);
			++index;
			if (index == 100)
				break;
		}
	}

	index = 0;*/

	vector<Key_Point> tmp;
/*	for (multiset<OMG>::reverse_iterator p = forSort.rbegin(); p != forSort.rend(); ++p){
		//Mat tempImage = imageColor;
		if (p->Rvalue!=INT_MAX && p->Rvalue!=4.0 ){
			//circle(imageColor, Point(p->col, p->row), 3, Scalar(255, 255, 0), 1);
			//imshow("??", imageColor);
			//waitKey(1);
			//printf("%f %d\n", p->Rvalue, index);
			tmp.push_back(p->myKeyPoint);
			// cout << p->row << " " << p->col << endl;
			++index;
			if (index == 300)
				break;
		}
	}*/

	for (multiset<OMG>::reverse_iterator p = forSort.rbegin(); p != forSort.rend(); ++p){
		// printf("%f %d\n", p->Rvalue, index);
		//Mat tempImage = imageColor;
		if(p->Rvalue!=INT_MAX && abs(p->Rvalue)>=4.0 && abs(p->Rvalue)<12){
			 // circle(imageColor, Point(p->col, p->row), 3, Scalar(255, 255, 0), 1);
			// imshow("??", imageColor);
			// waitKey(0);
			// printf("%f %d\n", p->Rvalue, index);
			tmp.push_back(p->myKeyPoint);
			/*++index;
			if (index == 1500)
				break;*/
		}
	}
		// cout << endl << "NEXT" << endl;

	keyPoints = tmp;
	/*index = 0;
	for (multiset<OMG>::iterator p = forSort.begin(); p != forSort.end(); ++p){
		//Mat tempImage = imageColor;
		if (p->Rvalue != INT_MAX && p->Rvalue != 4.0){
			circle(imageColor, Point(p->col, p->row), 3, Scalar(0, 0, 255), 1);
			imshow("??", imageColor);
			waitKey(1);
			printf("%f %d\n", p->Rvalue, index);
			++index;
			if (index == 100)
				break;
		}
	}*/
	//index = 0;
	//for (set<OMG>::reverse_iterator p = forSort.rbegin(); p != forSort.rend() && index < 300; ++p){
	//	//if (index >= 10000){
	//		circle(imageColor, Point(p->col, p->row), 3, Scalar(255, 0, 0), 1);
	//		cout << p->Rvalue << "\n";
	//	//}
	//	++index;
	//}
		
	//printf("%d\n", count);
	//imshow("??", imageColor);
	//waitKey(0);
	//imwrite("hahaha.jpg", imageColor);
}


void calculateR(Mat& R, Mat& Dxx, Mat& Dxy, Mat& Dyy)
{
	int count = 0;
	for (int row = 0; row < R.rows; ++row){
		for (int col = 0; col < R.cols; ++col){
			//cout << Dxx.at<float>(row, col) << " " << Dyy.at<float>(row, col) << " " << Dxy.at<float>(row, col) << "\n";
			float denominator = (Dxx.at<float>(row, col) * Dyy.at<float>(row, col) - Dxy.at<float>(row, col) * Dxy.at<float>(row, col));
			float numerator = (Dxx.at<float>(row, col) + Dyy.at<float>(row, col)) * (Dxx.at<float>(row, col) + Dyy.at<float>(row, col));
			//cout << denominator << " " << numerator << "\n";
			//if (Dxy.at<float>(row, col) != 0)
			//	system("pause");
			if (denominator != 0){
				R.at<float>(row, col) = numerator / denominator;
				++count;
			}
			else{
				R.at<float>(row, col) = INT_MAX;
				//cout << "Hey\n";
			}
		}
	}
	// cout << "¤À¥À!=0 : " << count << "\n";
}

void computeDxxDxyDyy(Mat& src, Mat& Dxx, Mat& Dxy, Mat& Dyy)
{
	//Dxx
	for (int row = 0; row < src.rows; ++row)
		for (int col = 2; col < src.cols - 2; ++col)
			Dxx.at<float>(row, col) = 0.25*src.at<uchar>(row, col - 2) + 0.25*src.at<uchar>(row, col + 2) - 0.5*src.at<uchar>(row, col);

	for (int row = 0; row < src.rows; ++row)//²Ä 0 column
		Dxx.at<float>(row, 0) = src.at<uchar>(row, 1) - src.at<uchar>(row, 0);

	for (int row = 0; row < src.rows; ++row)//³Ì«á column
		Dxx.at<float>(row, src.cols - 1) = src.at<uchar>(row, src.cols - 2) - src.at<uchar>(row, src.cols - 1);

	//Dyy
	for (int row = 2; row < src.rows - 2; ++row)
		for (int col = 0; col < src.cols; ++col)
			Dyy.at<float>(row, col) = 0.25*src.at<uchar>(row - 2, col) + 0.25*src.at<uchar>(row + 2, col) - 0.5*src.at<uchar>(row, col);

	for (int col = 0; col < src.cols; ++col)//²Ä 0 row
		Dyy.at<float>(0, col) = src.at<uchar>(1, col) - src.at<uchar>(0, col);

	for (int col = 0; col < src.cols; ++col)//³Ì«á row
		Dyy.at<float>(src.rows - 1, col) = src.at<uchar>(src.rows - 2, col) - src.at<uchar>(src.rows - 1, col);

	//Dxy
	for (int row = 1; row < src.rows - 1; ++row)
		for (int col = 1; col < src.cols - 1; ++col){
			Dxy.at<float>(row, col) = (float)(src.at<uchar>(row - 1, col + 1) + src.at<uchar>(row + 1, col - 1) - src.at<uchar>(row - 1, col - 1) - src.at<uchar>(row + 1, col + 1)) / 4.0;
			//cout << Dxy.at<float>(row, col) << "\n";
		}
	
	for (int col = 1; col < src.cols - 1; ++col){
		//²Ä 0 row
		Dxy.at<float>(0, col) = (float)(src.at<uchar>(0, col + 1) + src.at<uchar>(1, col - 1) - src.at<uchar>(0, col - 1) - src.at<uchar>(1, col + 1)) / 4.0;
		//³Ì«árow
		Dxy.at<float>(src.rows - 1, col) = (float)(src.at<uchar>(src.rows - 2, col + 1) + src.at<uchar>(src.rows - 1, col - 1) - src.at<uchar>(src.rows - 2, col - 1) - src.at<uchar>(src.rows - 1, col + 1)) / 4.0;
	}
	
	for (int row = 1; row < src.rows - 1; ++row){
		//²Ä 0 column
		Dxy.at<float>(row, 0) = (float)(src.at<uchar>(row - 1, 1) + src.at<uchar>(row + 1, 0) - src.at<uchar>(row - 1, 0) - src.at<uchar>(row + 1, 1)) / 4.0;
		//³Ì«ácolumn
		Dxy.at<float>(row, src.cols - 1) = (float)(src.at<uchar>(row - 1, src.cols - 1) + src.at<uchar>(row + 1, src.cols - 2) - src.at<uchar>(row - 1, src.cols - 2) - src.at<uchar>(row + 1, src.cols - 1)) / 4.0;
	}

	Dxy.at<float>(0, 0) = (float)(src.at<uchar>(0, 1) + src.at<uchar>(1, 0) - src.at<uchar>(0, 0) - src.at<uchar>(1, 1)) / 4.0;
	Dxy.at<float>(0, src.cols - 1) = (float)(src.at<uchar>(0, src.cols - 1) + src.at<uchar>(1, src.cols - 2) - src.at<uchar>(0, src.cols - 2) - src.at<uchar>(1, src.cols - 1)) / 4.0;
	Dxy.at<float>(src.rows - 1, 0) = (float)(src.at<uchar>(src.rows - 2, 1) + src.at<uchar>(src.rows - 1, 0) - src.at<uchar>(src.rows - 2, 0) - src.at<uchar>(src.rows - 1, 1)) / 4.0;
	Dxy.at<float>(src.rows - 1, src.cols - 1) = (float)(src.at<uchar>(src.rows - 2, src.cols - 1) + src.at<uchar>(src.rows - 1, src.cols - 2) - src.at<uchar>(src.rows - 2, src.cols - 2) - src.at<uchar>(src.rows - 1, src.cols - 1)) / 4.0;
}

void mySIFT::filterKeyPoints()
{
	for (int i = 0; i < keyPoints.size(); ++i){
		Key_Point& thisKpt = keyPoints[i];
		Mat& thisMat = blurredImgs[thisKpt.layer];
		//¦³n1­Ó³sÄòªºpixel³£¤ñthisKpt²`¡A¦³n2­Ó³sÄòªºÂI³£¤ñthisKpt²L
		
		//¦bDoG¤W±o¨ìªºkeyPoint¤£·|¦bÃä¬É
		//if (thisKpt.row - 3 < 0 || thisKpt.row + 3 >= thisMat.rows || thisKpt.col - 3 < 0 || thisKpt.col + 3 >= thisMat.cols)//ÀË¬d½d³ò¡A¤£¯à°÷µe¾ã°éªº´Nª½±µÙT
		//	continue;
		vector< int > brighter(8, 0);
		vector< int > darker(8, 0);
		int value = thisMat.at<uchar>(thisKpt.row, thisKpt.col);
		int threshold = 7;
		filterKeyPointsHelper1(brighter, darker, thisMat, thisKpt, value, threshold);
		int nBrighter, nDarker;
		filterKeyPointsHelper2(brighter, darker, nBrighter, nDarker);
		thisKpt.cornerValue = max(nBrighter, nDarker);
		if (thisKpt.cornerValue > 8 || thisKpt.cornerValue < 0)
			cout << "??\n";
	}

	//°µsortingºO¡A¤jªº±Æ«e­±
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
	// cout << temp.size() << " " << layer1 << " " << layer2 << "\n";
	keyPoints = temp;
}

void mySIFT::filterKeyPointsHelper1(vector< int >& brighter, vector< int >& darker, Mat& thisMat, Key_Point& thisKpt, int value, int threshold)
{
	//§Ëbrighter
	int brighterthshod = value + threshold;
	if (thisMat.at<uchar>(thisKpt.row - 1, thisKpt.col - 1) > brighterthshod)
		brighter[0] = 1;
	if (thisMat.at<uchar>(thisKpt.row - 1, thisKpt.col) > brighterthshod)
		brighter[1] = 1;
	if (thisMat.at<uchar>(thisKpt.row - 1, thisKpt.col + 1) > brighterthshod)
		brighter[2] = 1;
	if (thisMat.at<uchar>(thisKpt.row, thisKpt.col - 1) > brighterthshod)
		brighter[3] = 1;
	if (thisMat.at<uchar>(thisKpt.row, thisKpt.col + 1) > brighterthshod)
		brighter[4] = 1;
	if (thisMat.at<uchar>(thisKpt.row + 1, thisKpt.col - 1) > brighterthshod)
		brighter[5] = 1;
	if (thisMat.at<uchar>(thisKpt.row + 1, thisKpt.col) > brighterthshod)
		brighter[6] = 1;
	if (thisMat.at<uchar>(thisKpt.row + 1, thisKpt.col + 1) > brighterthshod)
		brighter[7] = 1;

	//§Ëdarker
	int darkerthshod = value - threshold;
	if (thisMat.at<uchar>(thisKpt.row - 1, thisKpt.col - 1) < darkerthshod)
		darker[0] = 1;
	if (thisMat.at<uchar>(thisKpt.row - 1, thisKpt.col) < darkerthshod)
		darker[1] = 1;
	if (thisMat.at<uchar>(thisKpt.row - 1, thisKpt.col + 1) < darkerthshod)
		darker[2] = 1;
	if (thisMat.at<uchar>(thisKpt.row, thisKpt.col - 1) < darkerthshod)
		darker[3] = 1;
	if (thisMat.at<uchar>(thisKpt.row, thisKpt.col + 1) < darkerthshod)
		darker[4] = 1;
	if (thisMat.at<uchar>(thisKpt.row + 1, thisKpt.col - 1) < darkerthshod)
		darker[5] = 1;
	if (thisMat.at<uchar>(thisKpt.row + 1, thisKpt.col) < darkerthshod)
		darker[6] = 1;
	if (thisMat.at<uchar>(thisKpt.row + 1, thisKpt.col + 1) < darkerthshod)
		darker[7] = 1;
}

void mySIFT::filterKeyPointsHelper2(vector< int >& brighter, vector< int >& darker, int& nBrighter, int& nDarker)
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
			accu1 = 0;//Âk0
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
			accu1 = 0;//Âk0
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