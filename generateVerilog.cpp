#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

int main()
{
	ofstream fout;
	char* output_file = "g_kernel_verilog_3x3.v";
	fout.open(output_file);
	for(int i = 1; i < 639; i++){
		fout << "wire  [25:0]  kernel_img_mul_" << i << "[0:8]" << endl;
		for(int kernel_y = 0; kernel_y < 3; kernel_y++)
			for(int kernel_x = 0; kernel_x < 3; kernel_x++){
				fout << "assign kernel_img_mul_" << i << "[" << kernel_y*3 + kernel_x << "] = buffer_data_"<<
				2 - kernel_y <<"["<< kernel_x*8 + 7 + (i-1)*8 <<":"
				<< kernel_x*8 + (i-1)*8 <<"] * G_Kernel_3x3["
				 << kernel_y%2 <<"]["<< kernel_x*18 + 17 <<":"<< kernel_x*18 <<"];" << endl;
			}

		fout << "wire  [29:0]  kernel_img_sum_"<< i <<" = ";
		int j;
		for(j = 0; j < 8; j++){
			fout << "kernel_img_mul_"<< i <<"["<< j <<"] + ";
			if((j+1)%3 == 0)
				fout << endl << "                ";
		}
		fout << "kernel_img_mul_"<< i <<"[" << j <<"];" << endl;

		fout << "always @(posedge clk) begin" << endl;
		fout << "  if (!rst_n)" << endl;
		fout << "    blur_din["<< i*8 + 7 <<":"<< i*8 <<"] <= 'd0;" << endl;
		fout << "  else if (current_state==ST_START || (current_state==ST_IDLE)&&buffer_valid)" << endl;
		fout << "    blur_din["<< i*8 + 7 <<":"<< i*8 <<"] <= kernel_img_sum_"<< i <<"[25:18];/*Q12.18 -> Q8.0*/" << endl;
		fout << "  else if (current_state==ST_IDLE)" << endl;
		fout << "    blur_din["<< i*8 + 7 <<":"<< i*8 <<"] <= 'd0;" << endl;
		fout << "end" << endl;
		fout << endl;
	}
	fout.close();
	return 0;
}

