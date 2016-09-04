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

    char* output_file = "filter_keypoint.v";/*
    char* layer[3][3] = {"buffer_data_1", "buffer_data_0", "img_dout",
                         "buffer_data_3", "buffer_data_2", "blur3x3_dout",
                         "buffer_data_5","buffer_data_4", "blur5x5_1_dout"};*/
    fout.open(output_file);

    char* layer[3] = {"top", "mid", "btm"};
    char* rows[3] = {"top_row", "mid_row", "btm_row"};

    fout << "always@(*) begin" << endl;
    fout << "  case(current_col)" << endl;
    for (int k = 1; k < 639; k++){
        fout << "    'd" << k << ": begin" << endl;
        for (int layer_num = 0; layer_num < 3; layer_num++)
            for (int col = 0; col < 3; col++){
                fout << "      " << layer[layer_num] << "[" << col << "] = " 
                << rows[layer_num] << "[" << k*8 + (col - 1)*8 + 7 << ":" << k*8 + (col - 1)*8 << "];" << endl;
            }
        fout << "    end" << endl;
    }
    fout << "    default: begin" << endl;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++){
            fout << "      " << layer[i] << "[" << j << "] = 'd0;" << endl;  
            }

    fout << "    end" << endl;
    fout << "  endcase" << endl;
    fout << "end" << endl;
    fout << endl;

    
    fout.close();
    return 0;
}

