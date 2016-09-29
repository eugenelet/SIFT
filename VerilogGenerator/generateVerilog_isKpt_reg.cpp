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

    char* output_file = "isKP_gen.v";
    fout.open(output_file);
    unsigned long long int base = 4;//0x8000000000000000;
    
    for(int i = 1; i < 63; i++){
        fout << "  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[" 
        	<< dec << i << ":0]) && is_keypoint_reg_0[" << i+1 <<"]) )" << endl;
        fout << "    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'h" << hex << 0xFFFFFFFFFFFFFFFF - base << ";" << endl;
        base *= 2;
        // cout << hex << base << endl;
    }
/*
    for(int i = 1; i < 63; i++){
        fout << "  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && filter_layer && (!(|is_keypoint_reg_1[" 
        	<< dec << i << ":0]) && is_keypoint_reg_1[" << i+1 <<"]) )" << endl;
        fout << "    is_keypoint_reg_1 <= is_keypoint_reg_1 & 64'h" << hex << 0xFFFFFFFFFFFFFFFF - base << ";" << endl;
        base *= 2;
        // cout << hex << base << endl;
    }*/
    fout.close();
    return 0;
}

