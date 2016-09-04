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

    char* output_file = "prepare_filter.v";/*
    char* layer[3][3] = {"buffer_data_1", "buffer_data_0", "img_dout",
                         "buffer_data_3", "buffer_data_2", "blur3x3_dout",
                         "buffer_data_5","buffer_data_4", "blur5x5_1_dout"};*/
    char* layer[2][3] = {"buffer_data_3", "buffer_data_2", "blur3x3_dout",
                         "buffer_data_5", "buffer_data_4", "blur5x5_1_dout"};
    fout.open(output_file);
    fout << "module prepare_filter(" << endl;
    fout << "  clk," << endl;
    fout << "  rst_n," << endl;
    fout << "  current_state," << endl;
    fout << "  img_addr," << endl;
    fout << "  filter_input_0_0," << endl;
    fout << "  filter_input_0_1," << endl;
    fout << "  filter_input_0_2," << endl;
    fout << "  filter_input_1_0," << endl;
    fout << "  filter_input_1_1," << endl;
    fout << "  filter_input_1_2," << endl;
    fout << "  buffer_data_2," << endl;
    fout << "  buffer_data_3," << endl;
    fout << "  buffer_data_4," << endl;
    fout << "  buffer_data_5," << endl;
    fout << "  blur3x3_dout," << endl;
    fout << "  blur5x5_1_dout," << endl;
    fout << "  no_keypoint," << endl;
    fout << "  is_keypoint_0," << endl;
    fout << "  is_keypoint_1," << endl;
    fout << "  current_RowCol_0," << endl;
    fout << "  current_RowCol_1" << endl;
    fout << ");" << endl;

    fout << endl;

    fout << "input      clk," << endl;
    fout << "           rst_n;" << endl;
    fout << endl;
    fout << "input  [2:0]       current_state;" << endl;
    fout << "input  [8:0]       img_addr;" << endl;
    fout << "input  [5119:0]    buffer_data_2," << endl;
    fout << "                   buffer_data_3," << endl;
    fout << "                   buffer_data_4," << endl;
    fout << "                   buffer_data_5," << endl;
    fout << "                   blur3x3_dout," << endl;
    fout << "                   blur5x5_1_dout;" << endl;
    fout << "input  [637:0]     is_keypoint_0;" << endl;
    fout << "input  [637:0]     is_keypoint_1;" << endl;
    fout << "output reg [23:0]  filter_input_0_0; // wire" << endl;
    fout << "output reg [23:0]  filter_input_0_1; // wire" << endl;
    fout << "output reg [23:0]  filter_input_0_2; // wire" << endl;
    fout << "output reg [23:0]  filter_input_1_0; // wire" << endl;
    fout << "output reg [23:0]  filter_input_1_1; // wire" << endl;
    fout << "output reg [23:0]  filter_input_1_2; // wire" << endl;
    fout << "output [1:0]       no_keypoint;" << endl;
    fout << "output reg [18:0]  current_RowCol_0; // wire;" << endl;
    fout << "output reg [18:0]  current_RowCol_1; // wire;" << endl;

    fout << endl;

    fout << "/*Module FSM*/" << endl;
    fout << "parameter ST_IDLE   = 0," << endl;
    fout << "          ST_READY  = 1,/*Idle 1 state for SRAM to get READY*/" << endl;
    fout << "          ST_DETECT = 2," << endl;
    fout << "          ST_FILTER = 3," << endl;
    fout << "          ST_UPDATE = 4,/*Grants a cycle to update MEM addr*/" << endl;
    fout << "          ST_BUFFER = 5;/*Grants buffer a cycle to update*/" << endl;

    fout << endl;
    
    fout << "reg   [637:0] detected_keypoint[0:1];" << endl;
    for(int module = 0; module < 2; module++){
        fout << "always @(posedge clk) begin" << endl;
        fout << "  if (!rst_n)" << endl;
        fout << "    detected_keypoint[" << module << "] <= 'd0;" << endl;
        fout << "  else if (current_state==ST_DETECT)" << endl;
        fout << "    detected_keypoint[" << module << "] <= is_keypoint_" << module << ";" << endl;
        for(int i = 0; i < 638; i++){
            fout << "  else if (current_state==ST_FILTER && detected_keypoint[" << module << "][" << i << "])" << endl;
            fout << "    detected_keypoint[" << module << "][" << i << "] <= 1'b0;" << endl;
        }
        fout << "  else" << endl;
        fout << "    detected_keypoint[" << module << "] <= 'd0;" << endl;
        fout << "end" << endl;
        fout << endl;
    }

    for(int module = 0; module < 2; module++){
        fout << "assign no_keypoint[" << module << "] = (|detected_keypoint[" << module << "]) ? 0 : 1;" << endl;
    }
    fout << endl;



    for(int module = 0; module < 2; module++){
        // fout << "wire    [23:0]      filter_input_" << module << "[0:2];" << endl;
        fout << "always @(*) begin" << endl;
        for(int i = 0; i < 638; i++){
            if( i == 0){
                fout << "  if (detected_keypoint[" << module << "][" << i << "]) begin" << endl;
                for(int j = 0; j < 3; j++)
                    fout << "    filter_input_" << module << "_" << j << " = " << layer[module][j] << "[" << 23 + i*8 << ":" << i*8 << "];" << endl;
                    fout << "    current_RowCol_" << module <<" = {img_addr, "<< "10'd" << i + 1 << "};" << endl;
                fout << "  end" << endl;    
            }
            else{
                fout << "  else if (detected_keypoint[" << module << "][" << i << "]) begin" << endl;
                for(int j = 0; j < 3; j++)
                    fout << "    filter_input_" << module << "_" << j << " = " << layer[module][j] << "[" << 23 + i*8 << ":" << i*8 << "];" << endl;
                    fout << "    current_RowCol_" << module <<" = {img_addr, "<< "10'd" << i + 1 << "};" << endl;

                fout << "  end" << endl;    
            }
        }

            fout << "  else begin" << endl;
            for(int j = 0; j < 3; j++)
                fout << "    filter_input_" << module << "_" << j << " = " << "'d0;" << endl;
            fout << "    current_RowCol_" << module <<" = 'd0;" << endl;
            fout << "  end" << endl;  
            fout << "end" << endl;  
        fout << endl;
    }

    fout << "endmodule";




    fout.close();
    return 0;
}

