#include "../include/sift.h"

void match_multi(mySIFT& left1, mySIFT& left2, mySIFT& right, string targetFile1, string targetFile2, Mat img_scene)
{
	vector< Key_Point >& a1 = left1.keyPoints;
    vector< Key_Point >& a2 = left2.keyPoints;
	vector< Key_Point >& b  = right.keyPoints;
	
    Mat target1 = imread(targetFile1);//§Ú­n±m¦âªº
    Mat target2 = imread(targetFile2);//§Ú­n±m¦âªº
	Mat find = img_scene;
	Mat result = concatMultiImg(target1, target2, find);

	vector< Point2f > obj1, obj2;
	vector< Point2f > scene, scene2;

	for (int i = 0; i < a1.size(); ++i){// match target 1
		int index = -1;//index of minimum distance;
		double min = INT_MAX;
		int indexMin2 = -1;
		double min2 = INT_MAX;
		
		for (int j = 0; j < b.size(); ++j){//¼É¤O¥h±½¨C¤@­Ó¥kÃäªºKey_Point:b[j]
			double dist = 0;
			for (int k = 0; k < 32; ++k)
				dist += (a1[i].descriptor[k] - b[j].descriptor[k]) * (a1[i].descriptor[k] - b[j].descriptor[k]);
			if (dist < min){//³Ì¤pªº­n³Q¨ú¥N¤F
				min2 = min;
				indexMin2 = index;
				min = dist;
				index = j;//¥ªÃäi match¨ì¥kÃäindex
			}
		}

		int B = rand() % 256;
		int G = rand() % 256;
		int R = rand() % 256;

		if (min < 0.5 * min2){//good matches
			int aScaleNum = a1[i].layer / left1.nLayersPerOctave;// == 0) ? 1 : 1.6;
			double aScaling = 1;
			for (int k = 0; k < aScaleNum; ++k){
				aScaling *= SCALE;
			}
			int bScaleNum = b[index].layer / right.nLayersPerOctave;// == 0) ? 1 : 1.6;//ÁY¤p´X­¿ªº¡A­n©ñ¤j¦^¨Ó
			double bScaling = 1;
			for (int k = 0; k < bScaleNum; ++k){
				bScaling *= SCALE;
			}
			//cout << aScaling << " " << bScaling << "\n";
			// circle(result, Point(a1[i].col * aScaling, a1[i].row * aScaling), 3, Scalar(255, 0, 0), 1);
			// circle(result, Point(max(target1.cols, target2.cols) + b[index].col * bScaling, b[index].row * bScaling), 3, Scalar(0, 255, 0), 1);

			line(result, Point(a1[i].col * aScaling, a1[i].row * aScaling), Point(max(target1.cols, target2.cols) + b[index].col * bScaling, b[index].row * bScaling), Scalar(B, G, R));
			obj1.push_back(Point2f(a1[i].col * aScaling, a1[i].row * aScaling));
			scene.push_back(Point2f(b[index].col * bScaling, b[index].row * bScaling));
        }
	}
	//µ²§ômatch

    for (int i = 0; i < a2.size(); ++i){// match target 2
        int index = -1;//index of minimum distance;
        double min = INT_MAX;
        int indexMin2 = -1;
        double min2 = INT_MAX;
        
        for (int j = 0; j < b.size(); ++j){//¼É¤O¥h±½¨C¤@­Ó¥kÃäªºKey_Point:b[j]
            double dist = 0;
            for (int k = 0; k < 32; ++k)
                dist += (a2[i].descriptor[k] - b[j].descriptor[k]) * (a2[i].descriptor[k] - b[j].descriptor[k]);
            if (dist < min){//³Ì¤pªº­n³Q¨ú¥N¤F
                min2 = min;
                indexMin2 = index;
                min = dist;
                index = j;//¥ªÃäi match¨ì¥kÃäindex
            }
        }

        int B = rand() % 256;
        int G = rand() % 256;
        int R = rand() % 256;

        if (min < 0.5 * min2){//good matches
			int aScaleNum = a2[i].layer / left2.nLayersPerOctave;// == 0) ? 1 : 1.6;
			double aScaling = 1;
			for (int k = 0; k < aScaleNum; ++k){
				aScaling *= SCALE;
			}
			int bScaleNum = b[index].layer / right.nLayersPerOctave;// == 0) ? 1 : 1.6;//ÁY¤p´X­¿ªº¡A­n©ñ¤j¦^¨Ó
			double bScaling = 1;
			for (int k = 0; k < bScaleNum; ++k){
				bScaling *= SCALE;
			}
            //cout << aScaling << " " << bScaling << "\n";
            line(result, Point(a2[i].col * aScaling, target1.rows + a2[i].row * aScaling), Point(max(target1.cols, target2.cols) + b[index].col * bScaling, b[index].row * bScaling), Scalar(B, G, R));
            obj2.push_back(Point2f(a2[i].col * aScaling, a2[i].row * aScaling));
            scene2.push_back(Point2f(b[index].col * bScaling, b[index].row * bScaling));
        }
    }


	if (obj1.empty() || scene.empty() || obj2.empty())
		return;
		//system("pause");

	Mat H = findHomography(obj1, scene, CV_RANSAC);
    Mat H2 = findHomography(obj2, scene2, CV_RANSAC);

	vector< Point2f > obj_corners(4);
	obj_corners[0] = cvPoint(0, 0);
	obj_corners[1] = cvPoint(left1.blurredImgs[0].cols, 0);
	obj_corners[2] = cvPoint(left1.blurredImgs[0].cols, left1.blurredImgs[0].rows);
	obj_corners[3] = cvPoint(0, left1.blurredImgs[0].rows);
	
    vector< Point2f > obj_corners2(4);
    obj_corners2[0] = cvPoint(0, 0);
    obj_corners2[1] = cvPoint(left2.blurredImgs[0].cols, 0);
    obj_corners2[2] = cvPoint(left2.blurredImgs[0].cols, left2.blurredImgs[0].rows);
    obj_corners2[3] = cvPoint(0, left2.blurredImgs[0].rows);
    
	vector< Point2f > computed_corners(4);
	for (int i = 0; i < 4; ++i)
		computed_corners[i] = MatMulti(H, obj_corners[i]);//¦Û¤vºâ

    vector< Point2f > computed_corners2(4);
    for (int i = 0; i < 4; ++i)
        computed_corners2[i] = MatMulti(H2, obj_corners2[i]);//¦Û¤vºâ

    //int target_cols = max(target1.cols, target2.cols);
    Point2f target_offset = cvPoint(max(target1.cols, target2.cols), 0);
    line(result, computed_corners[0] + target_offset, computed_corners[1] + target_offset, Scalar(255, 0, 255), 4);
    line(result, computed_corners[1] + target_offset, computed_corners[2] + target_offset, Scalar(255, 0, 255), 4);
    line(result, computed_corners[2] + target_offset, computed_corners[3] + target_offset, Scalar(255, 0, 255), 4);
    line(result, computed_corners[3] + target_offset, computed_corners[0] + target_offset, Scalar(255, 0, 255), 4);


    line(result, computed_corners2[0] + target_offset, computed_corners2[1] + target_offset, Scalar(0, 255, 255), 4);
    line(result, computed_corners2[1] + target_offset, computed_corners2[2] + target_offset, Scalar(0, 255, 255), 4);
    line(result, computed_corners2[2] + target_offset, computed_corners2[3] + target_offset, Scalar(0, 255, 255), 4);
    line(result, computed_corners2[3] + target_offset, computed_corners2[0] + target_offset, Scalar(0, 255, 255), 4);

    imshow("haha", result);
	// imwrite("result.jpg", result);
	waitKey(1);
}



Mat concatMultiImg(Mat& target1, Mat& target2, Mat& scene)
{   
    //cout << i1.type();
    Mat concat_out(max(target1.rows + target2.rows, scene.rows), max(target1.cols, target2.cols) + scene.cols, target1.type(), Scalar(0, 0, 0));

    for (int row = 0; row < concat_out.rows; ++row)
        for (int col = 0; col < concat_out.cols; ++col){
            if(col < max(target1.cols, target2.cols)){
                if(row<target1.rows && col<target1.cols) // target 1
                    concat_out.at<Vec3b>(row, col) = target1.at<Vec3b>(row, col);    
                else if(row<(target1.rows + target2.rows) && col<target2.cols ) // target 2
                    concat_out.at<Vec3b>(row, col) = target2.at<Vec3b>(row - target1.rows, col);    

            }
            else if(row<scene.rows)
                concat_out.at<Vec3b>(row, col) = scene.at<Vec3b>(row, col - max(target1.cols, target2.cols));
        }
    return concat_out;
}


void match(mySIFT& left, mySIFT& right, string targetFile, Mat img_scene, clock_t s)
{
	cout << "keyPoints (AFTER): " << right.keyPoints.size() << endl << endl;
	// vector< Key_Point >& a = left.keyPoints;
	// vector< Key_Point >& b = right.keyPoints;
	vector< Key_Point >& b = left.keyPoints;

	vector< Key_Point >& a = right.keyPoints;
	
	Mat target = imread(targetFile);//§Ú­n±m¦âªº
	Mat find = img_scene;
	Mat result = concat2Img(target, find);

	vector< Point2f > obj;
	vector< Point2f > scene;

	for (int i = 0; i < a.size(); ++i){//¹ï¨C¤@­Ó¥ªÃäªºKey_Point:a[i]
		//Mat garha = target.clone();
		//Mat garho = find.clone();
		int index = -1;//index of minimum distance;
		double min = INT_MAX;
		int indexMin2 = -1;
		double min2 = INT_MAX;
		
		for (int j = 0; j < b.size(); ++j){//¼É¤O¥h±½¨C¤@­Ó¥kÃäªºKey_Point:b[j]
			double dist = 0;
			for (int k = 0; k < 32; ++k)
				dist += (a[i].descriptor[k] - b[j].descriptor[k]) * (a[i].descriptor[k] - b[j].descriptor[k]);
			if (dist < min){//³Ì¤pªº­n³Q¨ú¥N¤F
				min2 = min;
				indexMin2 = index;
				min = dist;
				index = j;//¥ªÃäi match¨ì¥kÃäindex
			}
		}
		//circle(garha, Point(a[i].col, a[i].row), 3, Scalar(0, 0, 255));
		//circle(garho, Point(b[index].col, b[index].row), 3, Scalar(0, 0, 255));
		//imshow("target", garha);
		//imshow("find", garho);
		//while (waitKey(0) != 'e')
		//	continue;

		int B = rand() % 256;
		int G = rand() % 256;
		int R = rand() % 256;

		if (min < 0.5 * min2){//good matches
			int aScaleNum = a[i].layer / left.nLayersPerOctave;// == 0) ? 1 : 1.6;
			double aScaling = 1;
			for (int k = 0; k < aScaleNum; ++k){
				aScaling *= SCALE;
			}
			int bScaleNum = b[index].layer / right.nLayersPerOctave;// == 0) ? 1 : 1.6;//ÁY¤p´X­¿ªº¡A­n©ñ¤j¦^¨Ó
			double bScaling = 1;
			for (int k = 0; k < bScaleNum; ++k){
				bScaling *= SCALE;
			}
			//cout << aScaling << " " << bScaling << "\n";
			// circle(result, Point(a[i].col * aScaling, a[i].row * aScaling), 3, Scalar(255, 0, 0), 1);
			// circle(result, Point(target.cols + b[index].col * bScaling, b[index].row * bScaling), 3, Scalar(0, 255, 0), 1);
			circle(result, Point(target.cols + a[i].col * aScaling, a[i].row * aScaling), 3, Scalar(255, 0, 0), 1);
			circle(result, Point(b[index].col * bScaling, b[index].row * bScaling), 3, Scalar(0, 255, 0), 1);

			// line(result, Point(a[i].col * aScaling, a[i].row * aScaling), Point(target.cols + b[index].col * bScaling, b[index].row * bScaling), Scalar(B, G, R));
			line(result, Point(target.cols + a[i].col * aScaling, a[i].row * aScaling), Point(b[index].col * bScaling, b[index].row * bScaling), Scalar(B, G, R));
			// obj.push_back(Point2f(a[i].col * aScaling, a[i].row * aScaling));
			// scene.push_back(Point2f(b[index].col * bScaling, b[index].row * bScaling));
			scene.push_back(Point2f(a[i].col * aScaling, a[i].row * aScaling));
			obj.push_back(Point2f(b[index].col * bScaling, b[index].row * bScaling));

		}
	}
	//µ²§ômatch

	if (obj.empty() || scene.empty())
		return;

	clock_t end = clock();
	//cout << "«e­±¥þ³¡ : " << (double)(end - s) / CLOCKS_PER_SEC << "\n";

	clock_t start = clock();
	
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
		computed_corners[i] = MatMulti(H, obj_corners[i]);//¦Û¤vºâ

	//end = clock();
	//cout << "Find Homography Use " << (double)(end - start) / CLOCKS_PER_SEC << "\n\n\n";

	//perspectiveTransform(obj_corners, scene_corners, H);
	//³o¬O¥ÎopenCVªºfunctionºâªº
	//line(result, scene_corners[0] + obj_corners[1], scene_corners[1] + obj_corners[1], Scalar(0, 255, 0), 4);
	//line(result, scene_corners[1] + obj_corners[1], scene_corners[2] + obj_corners[1], Scalar(0, 255, 0), 4);
	//line(result, scene_corners[2] + obj_corners[1], scene_corners[3] + obj_corners[1], Scalar(0, 255, 0), 4);
	//line(result, scene_corners[3] + obj_corners[1], scene_corners[0] + obj_corners[1], Scalar(0, 255, 0), 4);

	//³o¬O¦Û¤vºâªº
	line(result, computed_corners[0] + obj_corners[1], computed_corners[1] + obj_corners[1], Scalar(255, 0, 255), 4);
	line(result, computed_corners[1] + obj_corners[1], computed_corners[2] + obj_corners[1], Scalar(255, 0, 255), 4);
	line(result, computed_corners[2] + obj_corners[1], computed_corners[3] + obj_corners[1], Scalar(255, 0, 255), 4);
	line(result, computed_corners[3] + obj_corners[1], computed_corners[0] + obj_corners[1], Scalar(255, 0, 255), 4);

	imshow("haha", result);
	//imwrite("result.jpg", result);
	waitKey(1);
}

Point2f MatMulti(Mat matrix, Point2f point)//¬Ý¬Ý¸òsource code¤@¤£¤@¼Ë
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

Mat concat2Img(Mat& i1, Mat& i2)
{	
	//cout << i1.type();
	Mat i3(max(i1.rows, i2.rows), i1.cols + i2.cols, i1.type(), Scalar(0, 0, 0));

	for (int row = 0; row < i3.rows; ++row)
		for (int col = 0; col < i3.cols; ++col){
			if (col < i1.cols && row < i1.rows)//¥Î¥ªÃäªº½Æ»s
				i3.at<Vec3b>(row, col) = i1.at<Vec3b>(row, col);
			else if (col >= i1.cols)
				i3.at<Vec3b>(row, col) = i2.at<Vec3b>(row, col - i1.cols);
		}
	return i3;
}
