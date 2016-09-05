#include "../include/sift.h"

void dumpDoG(mySIFT& image)
{
	ofstream fout;
	char answer_buf[80];
	char* answer_file = "DoG_";
	char* answer_end  = ".txt";
	for(int i = 0; i < 4; i++){
		strcpy(answer_buf, answer_file);
		string tmp = to_string(i);
		strcat(answer_buf, tmp.c_str());
		strcat(answer_buf, answer_end);
		fout.open(answer_buf);

		for(int row = 0; row < image.DoGs[i].rows; row++){
			for(int col = 0; col < image.DoGs[i].cols; col++)
				fout << image.DoGs[i].at<int>(row, col) << ' ' ;
			fout << endl;
		}
		fout << endl;
		fout.close();

	}


}