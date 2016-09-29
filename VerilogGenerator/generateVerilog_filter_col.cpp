#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string.h>
#include <string>

#define NUM_MODULES 2
#define NUM_PIXELS 64

using namespace std;

int main()
{
    ofstream fout;

    char* output_file = "filter_gen.v";
    fout.open(output_file);
    for(int i = 1; i < 63; i++){
        fout << "  else if (!keypoint_layer_1_empty&&(!(|is_keypoint_reg_0[" << i << ":0]) && is_keypoint_reg_0[" 
            << i+1 << "] ) ||" << endl << "     keypoint_layer_1_empty&&(!(|is_keypoint_reg_1[" << i << ":0]) && is_keypoint_reg_1[" << i + 1 
            << "]) )" << endl;
        fout << "    filter_col = current_col + " << i + 1 << ";" << endl;
    }

    fout.close();
    return 0;
}

