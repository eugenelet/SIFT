#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string.h>
#include <string>

#define KERNEL_SIZE 7
#define KERNEL_VER  0 /*For 5x5 kernel 0 or 1*/
#define NUM_COL     640
#define NUM_PIPE    10
#define PIXEL_SIZE  8
#define BIT_WIDTH   32

int COL_PER_PIPE = NUM_COL/NUM_PIPE;

using namespace std;

int main()
{
    ofstream fout;
    char output_buf[80];
    strcpy(output_buf, "Gaussian_Blur_");
    char kSize_str[10];
    char kVersion[2];
    sprintf(kSize_str, "%d", KERNEL_SIZE);
    // string tmp = to_string(KERNEL_SIZE);
    strcat(output_buf, kSize_str);
    strcat(output_buf, "x");
    strcat(output_buf, kSize_str);
    if(KERNEL_SIZE==5){
        strcat(output_buf, "_");
        sprintf(kVersion, "%d", KERNEL_VER);
        strcat(output_buf, kVersion);
    }
    strcat(output_buf, ".v");
    char* output_file = output_buf; //"g_kernel_verilog_3x3.v";

    fout.open(output_file);

    char* layer[7] = {"layer0", "layer1", "layer2", "layer3", "layer4", "layer5", "layer6", };
    fout << "module Gaussian_Blur_" << KERNEL_SIZE << "x" << KERNEL_SIZE;
    if(KERNEL_SIZE==5)
        fout << "_" << KERNEL_VER;
    fout << "(" << endl;
    fout << "  clk," << endl; 
    fout << "  rst_n," << endl; 
    for(int i = 0; i < KERNEL_SIZE; i++)
        fout << "  buffer_data_" << i << "," << endl; 
    fout << "  current_state," << endl;
    fout << "  blur_din" << endl;
    fout << ");" << endl;

    fout << endl;
    fout << "input                 clk;" << endl; 
    fout << "input                 rst_n;" << endl; 
    fout << "input         [3:0]   current_state;" << endl; 
    for(int i = 0; i < KERNEL_SIZE; i++)
        fout << "input       [5119:0]   buffer_data_" << i << ";" << endl; 

    fout << "output reg  [5119:0]  blur_din;" << endl; 
    fout << endl;

    fout << "parameter ST_IDLE        = 0,"<< endl;
    fout << "          ST_READY       = 1,/*Idle 1 state for SRAM to get READY*/"<< endl;
    fout << "          ST_GAUSSIAN_0  = 2,"<< endl;
    fout << "          ST_GAUSSIAN_1  = 3,"<< endl;
    fout << "          ST_GAUSSIAN_2  = 4,"<< endl;
    fout << "          ST_GAUSSIAN_3  = 5,"<< endl;
    fout << "          ST_GAUSSIAN_4  = 6,"<< endl;
    fout << "          ST_GAUSSIAN_5  = 7,"<< endl;
    fout << "          ST_GAUSSIAN_6  = 8,"<< endl;
    fout << "          ST_GAUSSIAN_7  = 9,"<< endl;
    fout << "          ST_GAUSSIAN_8  = 10,"<< endl;
    fout << "          ST_GAUSSIAN_9  = 11;"<< endl;

    fout << endl;

    if(KERNEL_SIZE == 3){
        fout << "reg       [95:0]  G_Kernel_3x3  [0:1];" << endl;
        fout << "always @(posedge clk) begin" << endl;
        fout << "  if (!rst_n) begin" << endl;
        fout << "    G_Kernel_3x3[0][31:0]  <=  32'h17BC5428; //18'b00_0101_1110_1111_0001;//'d092717;" << endl;
        fout << "    G_Kernel_3x3[0][63:32] <=  32'h1E7ABFF3; //18'b00_0111_1001_1110_1010;//'d119061;" << endl;
        fout << "    G_Kernel_3x3[0][95:64] <=  32'h17BC5428; //18'b00_0101_1110_1111_0001;//'d092717;" << endl;
        fout << "    G_Kernel_3x3[1][31:0]  <=  32'h1E7ABFF3; //18'b00_0111_1001_1110_1010;//'d119061;" << endl;
        fout << "    G_Kernel_3x3[1][63:32] <=  32'h2723AF8E; //18'b00_1001_1100_1000_1110;//'d152888;" << endl;
        fout << "    G_Kernel_3x3[1][95:64] <=  32'h1E7ABFF3; //18'b00_0111_1001_1110_1011;//'d119061;" << endl;
        fout << "  end" << endl;
        fout << "end" << endl;
    }
    else if(KERNEL_SIZE==5 && KERNEL_VER==0){
        fout << "reg       [159:0]  G_Kernel_5x5[0:2];" << endl;
        fout << "always @(posedge clk) begin" << endl;
        fout << "  if (!rst_n) begin" << endl;
        fout << "    G_Kernel_5x5[0][31:0]  <= 32'h05F2F79A;  // 18'b000001011111001011;//'d023238;" << endl;
        fout << "    G_Kernel_5x5[0][63:32] <= 32'h08A86809;  // 18'b000010001010100001;//d033819;" << endl;
        fout << "    G_Kernel_5x5[0][95:64] <= 32'h09CFB50A;  // 18'b000010011100111110;//d038325;" << endl;
        fout << "    G_Kernel_5x5[0][127:96] <= 32'h08A86809;  // 18'b000010001010100001;//d033819;" << endl;
        fout << "    G_Kernel_5x5[0][159:128] <= 32'h05F2F79A;  // 18'b000001011111001011;//'d023238;" << endl;
        fout << "    G_Kernel_5x5[1][31:0]  <= 32'h08A86809;  // 18'b000010001010100001;//d033819;" << endl;
        fout << "    G_Kernel_5x5[1][63:32] <= 32'h0C999546;  // 18'b000011001001100110;//d049218;" << endl;
        fout << "    G_Kernel_5x5[1][95:64] <= 32'h0E475732;  // 18'b000011100100011101;//d055776;" << endl;
        fout << "    G_Kernel_5x5[1][127:96] <= 32'h0C999546;  // 18'b000011001001100110;//d049218;" << endl;
        fout << "    G_Kernel_5x5[1][159:128] <= 32'h08A86809;  // 18'b000010001010100001;//d033819;" << endl;
        fout << "    G_Kernel_5x5[2][31:0]  <= 32'h09CFB50A;  // 18'b000010011100111110;//d038325;" << endl;
        fout << "    G_Kernel_5x5[2][63:32] <= 32'h0E475732;  // 18'b000011100100011101;//d055776;" << endl;
        fout << "    G_Kernel_5x5[2][95:64] <= 32'h102E5B3F;  // 18'b000100000010111001;//d063207;" << endl;
        fout << "    G_Kernel_5x5[2][127:96] <= 32'h0E475732;  // 18'b000011100100011101;//d055776;" << endl;
        fout << "    G_Kernel_5x5[2][159:128] <= 32'h09CFB50A;  // 18'b000010011100111110;//d038325;" << endl;
        fout << "  end" << endl;
        fout << "end" << endl;
    }
    else if(KERNEL_SIZE==5 && KERNEL_VER==1){
        fout << "reg       [159:0]  G_Kernel_5x5[0:2];" << endl;
        fout << "always @(posedge clk) begin" << endl;
        fout << "  if (!rst_n) begin" << endl;
        fout << "    G_Kernel_5x5[0][31:0]  <= 32'h07E31AF6; //18'b000001111110001100;//'d030809;" << endl;
        fout << "    G_Kernel_5x5[0][63:32] <= 32'h0983EB80; //18'b000010011000001111;//'d037169;" << endl;
        fout << "    G_Kernel_5x5[0][95:64] <= 32'h0A212B91; //18'b000010100010000100;//'d039568;" << endl;
        fout << "    G_Kernel_5x5[0][127:96] <= 32'h0983EB80; //18'b000010011000001111;//'d037169;" << endl;
        fout << "    G_Kernel_5x5[0][159:128] <= 32'h07E31AF6; //18'b000001111110001100;//'d030809;" << endl;
        fout << "    G_Kernel_5x5[1][31:0]  <= 32'h0983EB80; //18'b000010011000001111;//'d037169;" << endl;
        fout << "    G_Kernel_5x5[1][63:32] <= 32'h0B7AC7A0; //18'b000010110111101011;//'d044842;" << endl;
        fout << "    G_Kernel_5x5[1][95:64] <= 32'h0C387DF7; //18'b000011000011100001;//'d047737;" << endl;
        fout << "    G_Kernel_5x5[1][127:96] <= 32'h0B7AC7A0; //18'b000010110111101011;//'d044842;" << endl;
        fout << "    G_Kernel_5x5[1][159:128] <= 32'h0983EB80; //18'b000010011000001111;//'d037169;" << endl;
        fout << "    G_Kernel_5x5[2][31:0]  <= 32'h0A212B91; //18'b000010100010000100;//'d039568;" << endl;
        fout << "    G_Kernel_5x5[2][63:32] <= 32'h0C387DF7; //18'b000011000011100001;//'d047737;" << endl;
        fout << "    G_Kernel_5x5[2][95:64] <= 32'h0D02737E; //18'b000011010000001001;//'d050818;" << endl;
        fout << "    G_Kernel_5x5[2][127:96] <= 32'h0C387DF7; //18'b000011000011100001;//'d047737;" << endl;
        fout << "    G_Kernel_5x5[2][159:128] <= 32'h0A212B91; //18'b000010100010000100;//'d039568;" << endl;
        fout << "  end" << endl;
        fout << "end" << endl;
    }
    else if(KERNEL_SIZE==7){
        fout << "reg       [223:0] G_Kernel_7x7  [0:3];" << endl;
        fout << "always @(posedge clk) begin" << endl;
        fout << "  if (!rst_n) begin" << endl;
        fout << "    G_Kernel_7x7[0][31:0]    <= 32'h03C6EBCB; //18'b000000111100011011;//'d014754;" << endl;
        fout << "    G_Kernel_7x7[0][63:32]   <= 32'h046AA8A8; //18'b000001000110101010;//'d017252;" << endl;
        fout << "    G_Kernel_7x7[0][95:64]   <= 32'h04D9ED31; //18'b000001001101100111;//'d018950;" << endl;
        fout << "    G_Kernel_7x7[0][127:96]   <= 32'h050165DE; //18'b000001010000000101;//'d019552;" << endl;
        fout << "    G_Kernel_7x7[0][159:128]   <= 32'h04D9ED31; //18'b000001001101100111;//'d018950;" << endl;
        fout << "    G_Kernel_7x7[0][191:160]  <= 32'h046AA8A8; //18'b000001000110101010;//'d017252;" << endl;
        fout << "    G_Kernel_7x7[0][223:192] <= 32'h03C6EBCB; //18'b000000111100011011;//'d014754;" << endl;
        fout << "    G_Kernel_7x7[1][31:0]    <= 32'h046AA8A8; //18'b000001000110101010;//'d017252;" << endl;
        fout << "    G_Kernel_7x7[1][63:32]   <= 32'h052A1FB1; //18'b000001010010101000;//'d020174;" << endl;
        fout << "    G_Kernel_7x7[1][95:64]   <= 32'h05AC3BC7; //18'b000001011010110000;//'d022159;" << endl;
        fout << "    G_Kernel_7x7[1][127:96]   <= 32'h05DA6392; //18'b000001011101101001;//'d022863;" << endl;
        fout << "    G_Kernel_7x7[1][159:128]   <= 32'h05AC3BC7; //18'b000001011010110000;//'d022159;" << endl;
        fout << "    G_Kernel_7x7[1][191:160]  <= 32'h052A1FB1; //18'b000001010010101000;//'d020174;" << endl;
        fout << "    G_Kernel_7x7[1][223:192] <= 32'h046AA8A8; //18'b000001000110101010;//'d017252;" << endl;
        fout << "    G_Kernel_7x7[2][31:0]    <= 32'h04D9ED31; //18'b000001001101100111;//'d018950;" << endl;
        fout << "    G_Kernel_7x7[2][63:32]   <= 32'h05AC3BC7; //18'b000001011010110000;//'d022159;" << endl;
        fout << "    G_Kernel_7x7[2][95:64]   <= 32'h063B25B2; //18'b000001100011101100;//'d024340;" << endl;
        fout << "    G_Kernel_7x7[2][127:96]   <= 32'h066DD847; //18'b000001100110110111;//'d025113;" << endl;
        fout << "    G_Kernel_7x7[2][159:128]   <= 32'h063B25B2; //18'b000001100011101100;//'d024340;" << endl;
        fout << "    G_Kernel_7x7[2][191:160]  <= 32'h05AC3BC7; //18'b000001011010110000;//'d022159;" << endl;
        fout << "    G_Kernel_7x7[2][223:192] <= 32'h04D9ED31; //18'b000001001101100111;//'d018950;" << endl;
        fout << "    G_Kernel_7x7[3][31:0]    <= 32'h050165DE; //18'b000001010000000101;//'d019552;" << endl;
        fout << "    G_Kernel_7x7[3][63:32]   <= 32'h05DA6392; //18'b000001011101101001;//'d022863;" << endl;
        fout << "    G_Kernel_7x7[3][95:64]   <= 32'h066DD847; //18'b000001100110110111;//'d025113;" << endl;
        fout << "    G_Kernel_7x7[3][127:96]   <= 32'h06A2275A; //18'b000001101010001000;//'d025911;" << endl;
        fout << "    G_Kernel_7x7[3][159:128]   <= 32'h066DD847; //18'b000001100110110111;//'d025113;" << endl;
        fout << "    G_Kernel_7x7[3][191:160]  <= 32'h05DA6392; //18'b000001011101101001;//'d022863;" << endl;
        fout << "    G_Kernel_7x7[3][223:192] <= 32'h050165DE; //18'b000001010000000101;//'d019552;" << endl;
        fout << "  end" << endl;
        fout << "end" << endl;
    }
    fout << endl;

    for(int i = 0; i < KERNEL_SIZE; i++)
        fout << "reg    [" << KERNEL_SIZE*PIXEL_SIZE - 1 << ":0]    " << layer[i] <<"[0:" << COL_PER_PIPE - 1 <<"]; //wire" << endl; 
    
    fout << "always @(*) begin" << endl;
    fout << "  case(current_state)" << endl;
    for(int pipe = 0; pipe < NUM_PIPE; pipe++){
        fout << "    ST_GAUSSIAN_" << pipe << ": begin" << endl;
        for(int col = 0; col < COL_PER_PIPE; col++){
            if(pipe*COL_PER_PIPE + col < KERNEL_SIZE/2){
                for(int kernel_y = 0; kernel_y < KERNEL_SIZE; kernel_y++)
                    for(int kernel_x = 0; kernel_x < KERNEL_SIZE; kernel_x++){
                        if( (kernel_x*PIXEL_SIZE + (pipe*COL_PER_PIPE + col - KERNEL_SIZE/2)*PIXEL_SIZE) < 0 ){
                            fout << "        " << layer[kernel_y] << "[" << col << "][" << kernel_x*PIXEL_SIZE + (PIXEL_SIZE - 1) << ":" << kernel_x*PIXEL_SIZE 
                            << "] = 0;" << endl; 
                        }
                        else{
                            fout << "        " << layer[kernel_y] << "[" << col << "][" << kernel_x*PIXEL_SIZE + (PIXEL_SIZE - 1) << ":" << kernel_x*PIXEL_SIZE 
                            << "] = buffer_data_"<< (KERNEL_SIZE - 1) - kernel_y 
                            <<"["<< kernel_x*PIXEL_SIZE + (PIXEL_SIZE - 1) + (pipe*COL_PER_PIPE + col - KERNEL_SIZE/2)*PIXEL_SIZE <<":"
                            << kernel_x*PIXEL_SIZE + (pipe*COL_PER_PIPE + col - KERNEL_SIZE/2)*PIXEL_SIZE <<"];" <<endl;
                        }
                    }
            }
            else if(pipe*COL_PER_PIPE + col > 639 - KERNEL_SIZE/2){
                for(int kernel_y = 0; kernel_y < KERNEL_SIZE; kernel_y++)
                    for(int kernel_x = 0; kernel_x < KERNEL_SIZE; kernel_x++){
                        if( pipe*COL_PER_PIPE + col + kernel_x - KERNEL_SIZE/2> 639 ){
                            fout << "        " << layer[kernel_y] << "[" << col << "][" << kernel_x*PIXEL_SIZE + (PIXEL_SIZE - 1) << ":" << kernel_x*PIXEL_SIZE 
                            << "] = 0;" << endl; 
                        }
                        else{
                            fout << "        " << layer[kernel_y] << "[" << col << "][" << kernel_x*PIXEL_SIZE + (PIXEL_SIZE - 1) << ":" << kernel_x*PIXEL_SIZE 
                            << "] = buffer_data_"<< (KERNEL_SIZE - 1) - kernel_y 
                            <<"["<< kernel_x*PIXEL_SIZE + (PIXEL_SIZE - 1) + (pipe*COL_PER_PIPE + col - KERNEL_SIZE/2)*PIXEL_SIZE <<":"
                            << kernel_x*PIXEL_SIZE + (pipe*COL_PER_PIPE + col - KERNEL_SIZE/2)*PIXEL_SIZE <<"];" <<endl;
                        }
                    }
            }
            else{
                for(int kernel_y = 0; kernel_y < KERNEL_SIZE; kernel_y++)
                    for(int kernel_x = 0; kernel_x < KERNEL_SIZE; kernel_x++){
                        fout << "        " << layer[kernel_y] << "[" << col << "][" << kernel_x*PIXEL_SIZE + (PIXEL_SIZE - 1) << ":" << kernel_x*PIXEL_SIZE 
                        << "] = buffer_data_"<< (KERNEL_SIZE - 1) - kernel_y 
                        <<"["<< kernel_x*PIXEL_SIZE + (PIXEL_SIZE - 1) + (pipe*COL_PER_PIPE + col - KERNEL_SIZE/2)*PIXEL_SIZE <<":"
                        << kernel_x*PIXEL_SIZE + (pipe*COL_PER_PIPE + col - KERNEL_SIZE/2)*PIXEL_SIZE <<"];" <<endl;
                    }

            }
        }
        fout << "    end" << endl;
    }
    fout << "  endcase" << endl;
    fout << "end" << endl;
    fout << endl;

    for(int i = 0; i < COL_PER_PIPE; i++){
        fout << "wire  [39:0]  kernel_img_mul_" << i << "[0:" << KERNEL_SIZE*KERNEL_SIZE -1 <<"];" << endl;
        for(int kernel_y = 0; kernel_y < KERNEL_SIZE; kernel_y++)
            for(int kernel_x = 0; kernel_x < KERNEL_SIZE; kernel_x++){
                fout << "assign kernel_img_mul_" << i << "[" << kernel_y*KERNEL_SIZE + kernel_x << "] = "
                << layer[kernel_y] << "[" << i << "][" 
                << kernel_x*PIXEL_SIZE + (PIXEL_SIZE - 1) << ":" << kernel_x*PIXEL_SIZE << "] *  G_Kernel_" 
                << kSize_str << "x" << kSize_str << "["
                << KERNEL_SIZE/2 - abs(kernel_y - KERNEL_SIZE/2) <<"]["<< kernel_x*BIT_WIDTH + (BIT_WIDTH - 1) <<":"
                << kernel_x*BIT_WIDTH <<"];" << endl;
            }

        fout << "wire  [39:0]  kernel_img_sum_"<< i <<" = ";
        int j;
        for(j = 0; j < KERNEL_SIZE*KERNEL_SIZE - 1; j++){
            fout << "kernel_img_mul_"<< i <<"["<< j <<"] + ";
            if((j+1)%3 == 0)
                fout << endl << "                ";
        }
        fout << "kernel_img_mul_"<< i <<"[" << j <<"];" << endl;

    }

    for(int i = 0; i < NUM_COL; i++){
        fout << "always @(posedge clk) begin" << endl;
        fout << "  if (!rst_n)" << endl;
        fout << "    blur_din["<< i*PIXEL_SIZE + (PIXEL_SIZE - 1) <<":"<< i*PIXEL_SIZE <<"] <= 'd0;" << endl;
        fout << "  else if (current_state==ST_GAUSSIAN_" << i/COL_PER_PIPE << ")" << endl;
        fout << "    blur_din["<< i*PIXEL_SIZE + (PIXEL_SIZE - 1) <<":"<< i*PIXEL_SIZE <<"] <= kernel_img_sum_"<< i%COL_PER_PIPE <<"[39:32];/*Q8.32 -> Q8.0*/" << endl;
        fout << "  else if (current_state==ST_IDLE)" << endl;
        fout << "    blur_din["<< i*PIXEL_SIZE + (PIXEL_SIZE - 1) <<":"<< i*PIXEL_SIZE <<"] <= 'd0;" << endl;
        fout << "end" << endl;
        fout << endl;   
    }
    fout << endl;
    fout << "endmodule";
    fout.close();
    return 0;
}

