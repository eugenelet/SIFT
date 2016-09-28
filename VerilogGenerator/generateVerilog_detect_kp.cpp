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

    char* output_file = "detect_keypoint.v";/*
    char* layer[3][3] = {"buffer_data_1", "buffer_data_0", "img_dout",
                         "buffer_data_3", "buffer_data_2", "blur3x3_dout",
                         "buffer_data_5","buffer_data_4", "blur5x5_1_dout"};*/
    char* layer[3][3] = {"top_0", "top_1", "top_2",
                         "mid_0", "mid_1", "mid_2",
                         "btm_0", "btm_1", "btm_2"};

    char* inputs[4][3] = {"layer_0_0", "layer_0_1", "layer_0_2", 
                         "layer_1_0", "layer_1_1", "layer_1_2",
                         "layer_2_0", "layer_2_1", "layer_2_2",
                         "layer_3_0", "layer_3_1", "layer_3_2"};
    fout.open(output_file);
    fout << "module detect_keypoint(" << endl;
    fout << "  layer_0_0, /*img*/" << endl; 
    fout << "  layer_0_1," << endl; 
    fout << "  layer_0_2," << endl; 
    fout << "  layer_1_0, /*Blur 0*/" << endl; 
    fout << "  layer_1_1," << endl; 
    fout << "  layer_1_2," << endl; 
    fout << "  layer_2_0, /*Blur 1*/" << endl; 
    fout << "  layer_2_1," << endl; 
    fout << "  layer_2_2," << endl; 
    fout << "  layer_3_0, /*Blur 2*/" << endl; 
    fout << "  layer_3_1," << endl; 
    fout << "  layer_3_2," << endl; 
    fout << "  current_col," << endl; 
    fout << "  is_keypoint" << endl;
    fout << ");" << endl;

    fout << endl;

    fout << "input       [5119:0]   layer_0_0," << endl; 
    fout << "                       layer_0_1," << endl; 
    fout << "                       layer_0_2," << endl; 
    fout << "                       layer_1_0," << endl; 
    fout << "                       layer_1_1," << endl; 
    fout << "                       layer_1_2," << endl; 
    fout << "                       layer_2_0," << endl; 
    fout << "                       layer_2_1," << endl; 
    fout << "                       layer_2_2," << endl; 
    fout << "                       layer_3_0," << endl; 
    fout << "                       layer_3_1," << endl; 
    fout << "                       layer_3_2;" << endl; 
    fout << "input       [9:0]      current_col;" << endl; 
    fout << "output                 is_keypoint;" << endl;

    fout << endl;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++){
            fout << "reg signed [8:0]  ";
            fout << layer[i][j] << "[0:2]; // wire" << endl;
        }
    fout << endl;

    fout << "always@(*) begin" << endl;
    fout << "  case(current_col)" << endl;
    for (int k = 1; k < 639; k++){
        fout << "    'd" << k << ": begin" << endl;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++){
                for (int col = 0; col < 3; col++){
                    fout << "      " << layer[i][j] << "[" << col << "] = " 
                    << "{1'b0," << inputs[i+1][j] << "[" << k*8 + 7  + (col - 1)*8 << ":" << k*8 + (col - 1)*8 << "]}" 
                    << " - {1'b0, " << inputs[i][j] << "[" << k*8 + 7 + (col - 1)*8 << ":" << k*8 + (col - 1)*8 << "]};" << endl;
                }
            }
        fout << "    end" << endl;
    }
    fout << "    default: begin" << endl;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++){
            for(int k = 0; k < 3; k++){
                fout << "      " << layer[i][j] << "[" << k << "] = " 
                << "'d0;" << endl;  
            }
        }
    fout << "    end" << endl;
    fout << "  endcase" << endl;
    fout << "end" << endl;
    fout << endl;

    fout << "reg    [11:0]      detect_max; //wire" << endl;
    fout << "always@(*) begin" << endl;
    int count = 0;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++){
            for(int k = 0; k < 3; k++){
                if((i==1 && j!=1 && k!=1) || (i!=1 && (j==1 || k==1) && !(j==1 && k==1))){
                    fout << "  if(" << layer[1][1] << "[1] >= " << layer[i][j] << "[" << k << "])" << endl;
                    fout << "    detect_max[" << count << "] = 1;" << endl;
                    fout << "  else" << endl;
                    fout << "    detect_max[" << count << "] = 0;" << endl;
                    count++;
                }
            }
        }
    fout << "end" << endl;
    fout << endl;

    fout << "reg    [11:0]      detect_min; //wire" << endl;
    fout << "always@(*) begin" << endl;
    count = 0;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++){
            for(int k = 0; k < 3; k++){
                if((i==1 && j!=1 && k!=1) || (i!=1 && (j==1 || k==1) && !(j==1 && k==1))){
                    fout << "  if(" << layer[1][1] << "[1] =< " << layer[i][j] << "[" << k << "])" << endl;
                    fout << "    detect_min[" << count << "] = 1;" << endl;
                    fout << "  else" << endl;
                    fout << "    detect_min[" << count << "] = 0;" << endl; 
                    count++;
                }
            }
        }
    fout << "end" << endl;
    fout << endl;

    fout << "wire is_max = (&detect_max) ? 1:0;" << endl;  
    fout << "wire is_min = (&detect_min) ? 1:0;" << endl;  
    fout << "assign is_keypoint = is_max | is_min;" << endl;
    fout << endl;
    fout << "endmodule" ;

    fout.close();
    return 0;
}

