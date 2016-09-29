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

    char* output_file = "detect_module.v";
    fout.open(output_file);
    for(int mod = 0; mod < NUM_MODULES ; mod++){
        for(int pix = 0; pix < NUM_PIXELS; pix++){
            if(mod == 0){
                fout << "detect_keypoint u_detect_keypoint_" << mod << "_" << pix << "(" << endl;
                fout << "  .layer_0_0        (buffer_data_1)," << endl;
                fout << "  .layer_0_1        (buffer_data_0)," << endl;
                fout << "  .layer_0_2        (img_dout)," << endl;
                fout << "  .layer_1_0        (buffer_data_3)," << endl;
                fout << "  .layer_1_1        (buffer_data_2)," << endl;
                fout << "  .layer_1_2        (blur3x3_dout)," << endl;
                fout << "  .layer_2_0        (buffer_data_5)," << endl;
                fout << "  .layer_2_1        (buffer_data_4)," << endl;
                fout << "  .layer_2_2        (blur5x5_1_dout)," << endl;
                fout << "  .layer_3_0        (buffer_data_7)," << endl;
                fout << "  .layer_3_1        (buffer_data_6)," << endl;
                fout << "  .layer_3_2        (blur5x5_2_dout)," << endl;
                fout << "  .current_col      (current_col + " << pix << ")," << endl;
                fout << "  .is_keypoint      (is_keypoint_" << mod << "[" << pix << "])" << endl;
                fout << ");" << endl;
            }
            else{
                fout << "detect_keypoint u_detect_keypoint_" << mod << "_" << pix << "(" << endl;
                fout << "  .layer_0_0        (buffer_data_3)," << endl;
                fout << "  .layer_0_1        (buffer_data_2)," << endl;
                fout << "  .layer_0_2        (blur3x3_dout)," << endl;
                fout << "  .layer_1_0        (buffer_data_5)," << endl;
                fout << "  .layer_1_1        (buffer_data_4)," << endl;
                fout << "  .layer_1_2        (blur5x5_1_dout)," << endl;
                fout << "  .layer_2_0        (buffer_data_7)," << endl;
                fout << "  .layer_2_1        (buffer_data_6)," << endl;
                fout << "  .layer_2_2        (blur5x5_2_dout)," << endl;
                fout << "  .layer_3_0        (buffer_data_9)," << endl;
                fout << "  .layer_3_1        (buffer_data_8)," << endl;
                fout << "  .layer_3_2        (blur7x7_dout)," << endl;
                fout << "  .current_col      (current_col + " << pix << ")," << endl;
                fout << "  .is_keypoint      (is_keypoint_" << mod << "[" << pix << "])" << endl;
                fout << ");" << endl;
            }
        }
        fout << endl;
    }

    fout.close();
    return 0;
}

