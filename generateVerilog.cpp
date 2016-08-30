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
	char output_buf[80];
	strcpy(output_buf, "g_kernel_verilog_");
	char kSize_str[10];
	sprintf(kSize_str, "%d", KERNEL_SIZE);
	// string tmp = to_string(KERNEL_SIZE);
	strcat(output_buf, kSize_str);
	strcat(output_buf, "x");
	strcat(output_buf, kSize_str);
	strcat(output_buf, ".v");

	char* output_file = output_buf; //"g_kernel_verilog_3x3.v";

	fout.open(output_file);
	for(int i = 0; i < 640; i++){
		if(i < KERNEL_SIZE/2){
			fout << "wire  [25:0]  kernel_img_mul_" << i << "[0:" << KERNEL_SIZE*KERNEL_SIZE -1 <<"];" << endl;
			for(int kernel_y = 0; kernel_y < KERNEL_SIZE; kernel_y++)
				for(int kernel_x = 0; kernel_x < KERNEL_SIZE - KERNEL_SIZE/2 + i; kernel_x++){
					fout << "assign kernel_img_mul_" << i << "[" << kernel_y*KERNEL_SIZE + kernel_x << "] = buffer_data_"<<
					(KERNEL_SIZE - 1) - kernel_y <<"["<< kernel_x*8 + 7 + ((KERNEL_SIZE/2) - i)*8 + (i-KERNEL_SIZE/2)*8 <<":"
					<< kernel_x*8 + ((KERNEL_SIZE/2) - i)*8 + (i-KERNEL_SIZE/2)*8 <<"] * G_Kernel_" << kSize_str << "x" << kSize_str << "["
					 << KERNEL_SIZE/2 - abs(kernel_y - KERNEL_SIZE/2) <<"]["<< (KERNEL_SIZE/2)*18 - i*18 + kernel_x*18 + 17 
					 <<":"<< (KERNEL_SIZE/2)*18 - i*18 + kernel_x*18 <<"];" << endl;
				}

			fout << "wire  [29:0]  kernel_img_sum_"<< i <<" = ";
			int count = 0;
			for(int k_y = 0; k_y < KERNEL_SIZE; k_y++)
				for(int k_x = 0; k_x < KERNEL_SIZE - KERNEL_SIZE/2 + i; k_x++){
					fout << "kernel_img_mul_"<< i <<"["<< k_y*KERNEL_SIZE + k_x <<"] + ";
					count++;
					if(count%3 == 0)
						fout << endl << "                ";
				}
			fout << "'d0;" << endl;

			fout << "always @(posedge clk) begin" << endl;
			fout << "  if (!rst_n)" << endl;
			fout << "    blur_din["<< i*8 + 7 <<":"<< i*8 <<"] <= 'd0;" << endl;
			fout << "  else if (current_state==ST_START)" << endl;// || (current_state==ST_IDLE)&&buffer_valid
			fout << "    blur_din["<< i*8 + 7 <<":"<< i*8 <<"] <= kernel_img_sum_"<< i <<"[25:18];/*Q12.18 -> Q8.0*/" << endl;
			fout << "  else if (current_state==ST_IDLE)" << endl;
			fout << "    blur_din["<< i*8 + 7 <<":"<< i*8 <<"] <= 'd0;" << endl;
			fout << "end" << endl;
			fout << endl;	
		}
		else if(i > 639 - KERNEL_SIZE/2){
			fout << "wire  [25:0]  kernel_img_mul_" << i << "[0:" << KERNEL_SIZE*KERNEL_SIZE -1 <<"];" << endl;
			for(int kernel_y = 0; kernel_y < KERNEL_SIZE; kernel_y++)
				for(int kernel_x = 0; kernel_x < KERNEL_SIZE - KERNEL_SIZE/2 + (639 - i); kernel_x++){
					fout << "assign kernel_img_mul_" << i << "[" << kernel_y*KERNEL_SIZE + kernel_x << "] = buffer_data_"<<
					(KERNEL_SIZE - 1) - kernel_y <<"["<< kernel_x*8 + 7 + (i-KERNEL_SIZE/2)*8 <<":"
					<< kernel_x*8 + (i-KERNEL_SIZE/2)*8 <<"] * G_Kernel_" << kSize_str << "x" << kSize_str << "["
					 << KERNEL_SIZE/2 - abs(kernel_y - KERNEL_SIZE/2) <<"]["<< kernel_x*18 + 17 
					 <<":"<< kernel_x*18 <<"];" << endl;
				}

			fout << "wire  [29:0]  kernel_img_sum_"<< i <<" = ";
			int count = 0;
			for(int k_y = 0; k_y < KERNEL_SIZE; k_y++)
				for(int k_x = 0; k_x < KERNEL_SIZE - KERNEL_SIZE/2 + (639 - i); k_x++){
					fout << "kernel_img_mul_"<< i <<"["<< k_y*KERNEL_SIZE + k_x <<"] + ";
					count++;
					if(count%3 == 0)
						fout << endl << "                ";
				}
			fout << "'d0;" << endl;

			fout << "always @(posedge clk) begin" << endl;
			fout << "  if (!rst_n)" << endl;
			fout << "    blur_din["<< i*8 + 7 <<":"<< i*8 <<"] <= 'd0;" << endl;
			fout << "  else if (current_state==ST_START)" << endl;
			fout << "    blur_din["<< i*8 + 7 <<":"<< i*8 <<"] <= kernel_img_sum_"<< i <<"[25:18];/*Q12.18 -> Q8.0*/" << endl;
			fout << "  else if (current_state==ST_IDLE)" << endl;
			fout << "    blur_din["<< i*8 + 7 <<":"<< i*8 <<"] <= 'd0;" << endl;
			fout << "end" << endl;
			fout << endl;

		}
		else{
			fout << "wire  [25:0]  kernel_img_mul_" << i << "[0:" << KERNEL_SIZE*KERNEL_SIZE -1 <<"];" << endl;
			for(int kernel_y = 0; kernel_y < KERNEL_SIZE; kernel_y++)
				for(int kernel_x = 0; kernel_x < KERNEL_SIZE; kernel_x++){
					fout << "assign kernel_img_mul_" << i << "[" << kernel_y*KERNEL_SIZE + kernel_x << "] = buffer_data_"<<
					(KERNEL_SIZE - 1) - kernel_y <<"["<< kernel_x*8 + 7 + (i-KERNEL_SIZE/2)*8 <<":"
					<< kernel_x*8 + (i-KERNEL_SIZE/2)*8 <<"] * G_Kernel_" << kSize_str << "x" << kSize_str << "["
					 << KERNEL_SIZE/2 - abs(kernel_y - KERNEL_SIZE/2) <<"]["<< kernel_x*18 + 17 <<":"<< kernel_x*18 <<"];" << endl;
				}

			fout << "wire  [29:0]  kernel_img_sum_"<< i <<" = ";
			int j;
			for(j = 0; j < KERNEL_SIZE*KERNEL_SIZE - 1; j++){
				fout << "kernel_img_mul_"<< i <<"["<< j <<"] + ";
				if((j+1)%3 == 0)
					fout << endl << "                ";
			}
			fout << "kernel_img_mul_"<< i <<"[" << j <<"];" << endl;

			fout << "always @(posedge clk) begin" << endl;
			fout << "  if (!rst_n)" << endl;
			fout << "    blur_din["<< i*8 + 7 <<":"<< i*8 <<"] <= 'd0;" << endl;
			fout << "  else if (current_state==ST_START)" << endl;
			fout << "    blur_din["<< i*8 + 7 <<":"<< i*8 <<"] <= kernel_img_sum_"<< i <<"[25:18];/*Q12.18 -> Q8.0*/" << endl;
			fout << "  else if (current_state==ST_IDLE)" << endl;
			fout << "    blur_din["<< i*8 + 7 <<":"<< i*8 <<"] <= 'd0;" << endl;
			fout << "end" << endl;
			fout << endl;	
		}
	}
	fout.close();
	return 0;
}

