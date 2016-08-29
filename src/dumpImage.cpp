#include "../include/sift.h"

void dumpImage(mySIFT& image)
{
	ofstream fout;
	char* output_file = "originalImage.txt";
	fout.open(output_file);

	for(int row = 0; row < image.blurredImgs[0].rows; row++){
		for(int col = 0; col < image.blurredImgs[0].cols; col++)
			fout << (unsigned int)image.blurredImgs[0].at<uchar>(row, col) << ' ' ;
		fout << endl;
	}


	fout.close();
	char answer_buf[80];
	char* answer_file = "blurredImgs";
	char* answer_end  = ".txt";
	for(int i = 1; i < 5; i++){
		strcpy(answer_buf, answer_file);
		string tmp = to_string(i);
		strcat(answer_buf, tmp.c_str());
		strcat(answer_buf, answer_end);
		fout.open(answer_buf);

		for(int row = 0; row < image.blurredImgs[i].rows; row++){
			for(int col = 0; col < image.blurredImgs[i].cols; col++)
				fout << (unsigned int)image.blurredImgs[i].at<uchar>(row, col) << ' ' ;
			fout << endl;
		}
		fout << endl;
		fout.close();

	}


}