#include "../include/sift.h"


int main(int argc, char* argv[])
{
	
	srand(time(NULL));
	bool time_on = false;
	bool multi_on = false;
	if (argc > 1)
		for(int i = 1; i < argc; i++){
			if(strcmp(argv[i], "time") == 0)
				time_on = true;	
			if(strcmp(argv[i], "multi") == 0)
				multi_on = true;	
			if(strcmp(argv[i], "-h") == 0){
				cout << "time: Measure time." << endl;
				cout << "multi: Multi target." << endl;
				return 0;
			}
		}
	clock_t start, end;
	if (1){
		mySIFT haha[2];
		haha[0] = mySIFT(1.414, 1.414, 3);//sigma k
		haha[1] = mySIFT(1.414, 1.414, 3);//sigma k
		string targetFile = "target.jpg";
		string targetFile2 = "target2.jpg";
		Mat target = imread(targetFile, CV_LOAD_IMAGE_GRAYSCALE);
		Mat target2 = imread(targetFile2, CV_LOAD_IMAGE_GRAYSCALE);
		computeSift(haha[0], target, imread(targetFile),time_on);
		computeSift(haha[1], target2, imread(targetFile),time_on);
		// haha.filterKeyPoints_Hessian(target, imread(targetFile));
		//haha.drawKeyPoints(targetFile);
		VideoCapture cap(1);

		while (1){

			clock_t s;
			s = clock();
			Mat img_scene;
			cap >> img_scene;
			clock_t start, end;

			mySIFT hoho(1.414, 1.414, 1);//sigma k
			//hoho.LoadImage(imageName2);
			Mat imgScene;
			cvtColor(img_scene, imgScene, CV_BGR2GRAY);
			computeSift(hoho, imgScene, img_scene, time_on);
			//start = clock();
			if(multi_on)
				match_multi(haha[0], haha[1], hoho, targetFile, targetFile2, img_scene);
			else{
				// dumpKeyPoints(haha[0], hoho);
				match(haha[0], hoho, targetFile, img_scene, s);
			}
			//end = clock();
			//cout << "Match : " << (double)(end - s) / CLOCKS_PER_SEC << "\n";

		}
		
	}
	else{//測試串接2張圖
		Mat i1 = imread("jijin.jpg");// , CV_LOsAD_IMAGE_GRAYSCALE);
		Mat i2 = imread("gg.jpg");// , CV_LOAD_IMAGE_GRAYSCALE);
		Mat i3 = concat2Img(i1, i2);
		Mat smalli1;
		resize(i1, smalli1, Size(i1.cols / 2, i1.rows / 2));
		imshow("??", smalli1);
		imshow("!!", i1);
		waitKey(0);
	}

	//system("pause");
	return 0;
}

void mySIFT::LoadImage(Mat imgOri)
{
	blurredImgs.push_back(imgOri);//原圖是放index0
}