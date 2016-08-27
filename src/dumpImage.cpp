#include "../include/sift.h"

void dumpKeyPoints(mySIFT& left, mySIFT& right)
{
	ofstream fout;
	char* output_file = "keypoints.txt";
	fout.open(output_file);

	vector< Key_Point >& a = left.keyPoints;
	vector< Key_Point >& b = right.keyPoints;
	


	for (int i = 0; i < a.size(); ++i){//¹ï¨C¤@­Ó¥ªÃäªºKey_Point:a[i]
		int aScaleNum = a[i].layer / left.nLayersPerOctave;// == 0) ? 1 : 1.6;
		double aScaling = 1;
		for (int k = 0; k < aScaleNum; ++k){
			aScaling *= SCALE;
		}
		fout << a[i].col * aScaling << " " << a[i].row * aScaling << endl;
		for(int j = 0; j < 32; j++)
			fout << a[i].descriptor[j] << " ";
		fout << endl;
	}

	fout << endl;

	for (int i = 0; i < b.size(); ++i){//¹ï¨C¤@­Ó¥ªÃäªºKey_Point:a[i]
		int bScaleNum = b[i].layer / left.nLayersPerOctave;// == 0) ? 1 : 1.6;
		double bScaling = 1;
		for (int k = 0; k < bScaleNum; ++k){
			bScaling *= SCALE;
		}
		fout << b[i].col * bScaling << " " << b[i].row * bScaling << endl;
		for(int j = 0; j < 32; j++)
			fout << b[i].descriptor[j] << " ";
		fout << endl;
	}

	fout.close();

	char* answer_file = "answer.txt";
	fout.open(answer_file);

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

			obj.push_back(Point2f(a[i].col * aScaling, a[i].row * aScaling));
			scene.push_back(Point2f(b[index].col * bScaling, b[index].row * bScaling));
		}
	}

	for(int i = 0; i < obj.size(); i++)
		fout << obj[i].x << " " << obj[i].y << endl;

	fout << endl;

	for(int i = 0; i < scene.size(); i++)
		fout << scene[i].x << " " << scene[i].y << endl;

	fout.close();

}