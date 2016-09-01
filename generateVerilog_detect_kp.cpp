#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string.h>
#include <string>

#define KERNEL_SIZE 7

using namespace std;

int main()
{
	ofstream fout;

	char* output_file = "detect_kp.v";/*
	char* layer[3][3] = {"buffer_data_1", "buffer_data_0", "img_dout",
						 "buffer_data_3", "buffer_data_2", "blur3x3_dout",
						 "buffer_data_5","buffer_data_4", "blur5x5_1_dout"};*/
	char* layer[3][3] = {"top_0", "top_1", "top_2",
						 "mid_0", "mid_1", "mid_2",
						 "btm_0", "btm_1", "btm_2"};
	fout.open(output_file);
	fout << "module detect_keypoint(" << endl;
  	fout << "  top_0," << endl; 
  	fout << "  top_1," << endl; 
  	fout << "  top_2," << endl; 
  	fout << "  mid_0," << endl; 
  	fout << "  mid_1," << endl; 
  	fout << "  mid_2," << endl; 
  	fout << "  btm_0," << endl; 
  	fout << "  btm_1," << endl; 
  	fout << "  btm_2," << endl; 
  	fout << "  is_keypoint" << endl;
	fout << ");" << endl;

	fout << endl;

  	fout << "input       [5119:0]	top_0," << endl; 
  	fout << "						top_1," << endl; 
  	fout << "						top_2," << endl; 
  	fout << "						mid_0," << endl; 
  	fout << "						mid_1," << endl; 
  	fout << "						mid_2," << endl; 
  	fout << "						btm_0," << endl; 
  	fout << "						btm_1," << endl; 
  	fout << "						btm_2;" << endl; 
  	fout << "output 	 [637:0]	is_keypoint;" << endl;

  	fout << endl;

	fout << "wire	[25:0]		detect_max[0:637];" << endl;
	int neighbours_tmp;
	for(int col = 0; col < 638; col++){
		for(int neighbours = 0; neighbours<27; neighbours++){
			if(neighbours < 13)
				neighbours_tmp = neighbours;
			else
				neighbours_tmp = neighbours - 1;
			if(neighbours == 13)
				continue;
			fout << "assign detect_max[" << col << "][" << neighbours_tmp <<
			 "] = (" << layer[1][1] << "[" << (col+1)*8 + 7 << ":" << (col+1)*8 <<"] > " << layer[neighbours/9][(neighbours%9)/3] << "[" <<
			 (neighbours%3)*8 + 7 + (col)*8 << ":" << (neighbours%3)*8 + (col)*8 << "]) ? 1 : 0;";
			 fout << endl;
		}
		fout << endl;
	}

	fout << "wire [637:0] is_max;" << endl;
	for(int i = 0; i < 638; i++)
		fout << "assign is_max[" << i << "] = (&detect_max[" << i << "]) ? 1:0;" << endl;
	fout << endl;

	fout << "wire	[25:0]		detect_min[0:637];" << endl;
	for(int col = 0; col < 638; col++){
		for(int neighbours = 0; neighbours<27; neighbours++){
			if(neighbours < 13)
				neighbours_tmp = neighbours;
			else
				neighbours_tmp = neighbours - 1;
			if(neighbours == 13)
				continue;
			fout << "assign detect_min[" << col << "][" << neighbours_tmp <<
			 "] = (" << layer[1][1] << "[" << (col+1)*8 + 7 << ":" << (col+1)*8 <<"] < " << layer[neighbours/9][(neighbours%9)/3] << "[" <<
			 (neighbours%3)*8 + 7 + (col)*8 << ":" << (neighbours%3)*8 + (col)*8 << "]) ? 1 : 0;";
			 fout << endl;
		}
		fout << endl;
	}

	fout << "wire [637:0] is_min;" << endl;
	for(int i = 0; i < 638; i++)
		fout << "assign is_min[" << i << "] = (&detect_min[" << i << "]) ? 1:0;" << endl;
	fout << endl;

	for(int i = 0; i < 638; i++)
		fout << "assign is_keypoint[" << i << "] = is_max[" << i << "] | is_min[" << i << "];" << endl;
	fout << endl;

	
	fout << "endmodule" ;

	fout.close();
	return 0;
}

