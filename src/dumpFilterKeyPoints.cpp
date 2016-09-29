#include "../include/sift.h"

void dumpFilterKeyPoints(mySIFT& image)
{
	ofstream fout1, fout2, fout3;
	char* layer1_output_file = "keypoint_layer1.txt";
	char* layer2_output_file = "keypoint_layer2.txt";
	char* total_output_file = "keypoint.txt";
	fout1.open(layer1_output_file);
	fout2.open(layer2_output_file);
	fout3.open(total_output_file);

	vector< Key_Point >& kpt = image.keyPoints;
	
	for(int i = 0; i <kpt.size(); i++)
		fout3 << kpt[i].layer - 1 << ' ' << kpt[i].row << ' ' << kpt[i].col << endl;

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
	fout3.close();

}