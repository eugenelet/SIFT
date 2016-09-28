#include "../include/sift.h"

void countKeypointPerRow(mySIFT& image);
void sameKeypoint(mySIFT& image);

void computeSift(mySIFT &sift_obj, Mat img_scene, Mat img_color, bool time_on){
	if(time_on){
		clock_t start, end;
		sift_obj.LoadImage(img_scene);

		start = clock();
		sift_obj.createDoG();
		end = clock();
		cout << "Blurred & Create DoGs : " << (double)(end - start) / CLOCKS_PER_SEC << "\n";
		
		start = clock();
		sift_obj.detectKeypoints();
		end = clock();
		cout << "Detect Keypoints : " << (double)(end - start) / CLOCKS_PER_SEC << "\n";
		
		start = clock();
		sift_obj.filterKeyPoints();
		end = clock();
		cout << "Filter Keypoints(FAST) : " << (double)(end - start) / CLOCKS_PER_SEC << "\n";
		
		start = clock();
		sift_obj.computeDescriptor();
		end = clock();
		cout << "Compute Descriptors : " << (double)(end - start) / CLOCKS_PER_SEC << "\n";

		cout << endl << endl;
		
		//start = clock();
		//match(haha, hoho, targetFile, img_scene, s);
		//end = clock();
		//cout << "Match : " << (double)(end - s) / CLOCKS_PER_SEC << "\n";
	}
	else{
		sift_obj.LoadImage(img_scene);
		sift_obj.createDoG();
		dumpImage(sift_obj);
		dumpDoG(sift_obj);
		sift_obj.detectKeypoints();
		dumpDetectKeyPoints(sift_obj);
		countKeypointPerRow(sift_obj);
		sift_obj.filterKeyPoints();
		dumpFilterKeyPoints(sift_obj);
		sameKeypoint(sift_obj);

		// sift_obj.filterKeyPoints_Hessian(img_scene, img_color);
		sift_obj.computeDescriptor();
	}
}

void countKeypointPerRow(mySIFT& image){
	vector< Key_Point >& kpt = image.keyPoints;
	int row[480];
	int kptPer64Pixel[480][10];
	for(int i = 0; i < 480; i++)
		row[i] = 0;

	for(int i = 0; i < kpt.size(); i++)
		for(int j = 0; j < 480; j++)
			if(kpt[i].row == j)
				row[j]++;

	for(int i = 0; i < 480; i++)
		for(int j = 0; j < 10; j++)
			kptPer64Pixel[i][j] = 0;

	for(int i = 0; i < 480; i++)
		for(int j = 0; j < 640; j++)
			for(int kpt_num = 0; kpt_num < kpt.size(); kpt_num++)
				if(kpt[kpt_num].row==i && kpt[kpt_num].col==j)
					kptPer64Pixel[i][j/64]++;

	vector<int>totalKeypointPerRow;
	for(int i = 0; i < 480; i ++)
		if(row[i] != 0)
			totalKeypointPerRow.push_back(row[i]);

	int accuKptPerRow = 0;
	for(int i = 0; i < totalKeypointPerRow.size(); i++)
		accuKptPerRow += totalKeypointPerRow[i];

	cout << "Average Keypoint Per Row: " << accuKptPerRow/totalKeypointPerRow.size() << endl;

	int max = 0;
	int min = INT_MAX;
	int total = 0;
	int keyPointsStats[32];
	for(int i = 0; i < 32; i++)
		keyPointsStats[i] = 0;

	for(int i = 0; i < 480; i++)
		for(int j = 0; j < 10; j++){
			total += kptPer64Pixel[i][j];
			if(kptPer64Pixel[i][j] > max)
				max = kptPer64Pixel[i][j];
			if(kptPer64Pixel[i][j] < min)
				min = kptPer64Pixel[i][j];
			keyPointsStats[kptPer64Pixel[i][j]]++;
		}

	for(int i = 0; i < 32; i++)
	cout << "	" << i << "\t" << keyPointsStats[i] << endl;
	cout << "	Max: " << max << endl;
	cout << "	Min: " << min << endl;	

}

void sameKeypoint(mySIFT& image){
	vector<Key_Point> layer_1, layer_2;
	vector< Key_Point >& kpt = image.keyPoints;

	for(int i = 0; i < kpt.size(); i++)
		if(kpt[i].layer == 1)
			layer_1.push_back(kpt[i]);
		else
			layer_2.push_back(kpt[i]);

	int sameKpt = 0;
	for(int i = 0; i < layer_1.size(); i++)
		for(int j = 0; j < layer_2.size(); j++)
			if(layer_1[i].col==layer_2[j].col && layer_1[i].row==layer_2[j].row)
				sameKpt++;

	cout << "Same Keypoint Count: " << sameKpt << endl;
}