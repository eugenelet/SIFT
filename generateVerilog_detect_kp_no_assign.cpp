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
    fout << "  layer_0_0," << endl; 
    fout << "  layer_0_1," << endl; 
    fout << "  layer_0_2," << endl; 
    fout << "  layer_1_0," << endl; 
    fout << "  layer_1_1," << endl; 
    fout << "  layer_1_2," << endl; 
    fout << "  layer_2_0," << endl; 
    fout << "  layer_2_1," << endl; 
    fout << "  layer_2_2," << endl; 
    fout << "  layer_3_0," << endl; 
    fout << "  layer_3_1," << endl; 
    fout << "  layer_3_2," << endl; 
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
    fout << "output reg  [637:0]    is_keypoint; // wire" << endl;

    fout << endl;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++){
            fout << "reg signed [8:0]  ";
            fout << layer[i][j] << "[0:639]; // wire" << endl;
        }
    fout << endl;

    fout << "always@(*) begin" << endl;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++){
            for (int k = 0; k < 640; k++){
                fout << "  " << layer[i][j] << "[" << k << "]" << " = " 
                << "{1'b0," << inputs[i+1][j] << "[" << k*8 + 7 << ":" << k*8 << "]}" << " - {1'b0, " << inputs[i][j] << "[" << k*8 + 7 << ":" << k*8 << "]};" << endl;
            }
        }
    fout << "end" << endl;
    fout << endl;

    fout << "reg    [25:0]      detect_max[0:637]; //wire" << endl;
    int neighbours_tmp;
    for(int col = 0; col < 638; col++){
        fout << "always@(*) begin" << endl;
        for(int neighbours = 0; neighbours<27; neighbours++){
            if(neighbours < 13)
                neighbours_tmp = neighbours;
            else
                neighbours_tmp = neighbours - 1;
            if(neighbours == 13)
                continue;
            fout << "  if(" << layer[1][1] << "[" << col <<"] > " << layer[neighbours/9][(neighbours%9)/3] << "[" <<
             (col + 1) + ((neighbours%3) - 1) << "])" << endl;
            fout << "    detect_max[" << col << "][" << neighbours_tmp << "] = 1;" << endl;
            fout << "  else" << endl;
            fout << "    detect_max[" << col << "][" << neighbours_tmp << "] = 0;" << endl;
        }
        fout << "end" << endl;
        fout << endl;
    }

    fout << "reg [637:0] is_max;" << endl;
    fout << "always@(*) begin" << endl;
    for(int i = 0; i < 638; i++){
        fout << "  if(&detect_max[" << i << "])" << endl;
        fout << "    is_max[" << i << "] = 1;" << endl;
        fout << "  else" << endl;
        fout << "    is_max[" << i << "] = 0;" << endl;
    }
    fout << "end" << endl;
    fout << endl;

    fout << "reg    [25:0]      detect_min[0:637]; //wire" << endl;
    for(int col = 0; col < 638; col++){
        fout << "always@(*) begin" << endl;
        for(int neighbours = 0; neighbours<27; neighbours++){
            if(neighbours < 13)
                neighbours_tmp = neighbours;
            else
                neighbours_tmp = neighbours - 1;
            if(neighbours == 13)
                continue;
            fout << "  if(" << layer[1][1] << "[" << col <<"] > " << layer[neighbours/9][(neighbours%9)/3] << "[" <<
             (col + 1) + ((neighbours%3) - 1) << "])" << endl;
            fout << "    detect_min[" << col << "][" << neighbours_tmp << "] = 1;" << endl;
            fout << "  else" << endl;
            fout << "    detect_min[" << col << "][" << neighbours_tmp << "] = 0;" << endl;
        }
        fout << "end" << endl;
        fout << endl;
    }

    fout << "reg [637:0] is_min;" << endl;
    fout << "always@(*) begin" << endl;
    for(int i = 0; i < 638; i++){
        fout << "  if(&detect_min[" << i << "])" << endl;
        fout << "    is_min[" << i << "] = 1;" << endl;
        fout << "  else" << endl;
        fout << "    is_min[" << i << "] = 0;" << endl;
    }
    fout << "end" << endl;
    fout << endl;



    fout << "always@(*) begin" << endl;
    for(int i = 0; i < 638; i++)
        fout << "  is_keypoint[" << i << "] = is_max[" << i << "] | is_min[" << i << "];" << endl;
    fout << "end" << endl;
    fout << endl;

    
    fout << "endmodule" ;

    fout.close();
    return 0;
}

