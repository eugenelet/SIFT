#include "../include/sift.h"

void dumpFilterKeyPoints(mySIFT& image)
{
	ofstream fout;
	char* layer1_output_file = "keypoint_layer1.txt";
	char* layer2_output_file = "keypoint_layer2.txt";
	fout1.open(layer1_output_file);
	fout2.open(layer2_output_file);

	vector< Key_Point >& kpt = image.keyPoints;
	
	for(int i = 0; i < kpt.size(); i++){
		if(kpt.layer == 1)
			fout1 << kpt.row << ' ' << kpt.col << endl;
		else if(kpt.layer == 2)
			fout2 << kpt.col << ' ' << kpt.col << endl;
		else
			cout << "????" << endl;
	}


	fout1.close();
	fout2.close();

}