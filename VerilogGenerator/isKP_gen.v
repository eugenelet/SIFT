  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[1:0]) && is_keypoint_reg_0[2]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hfffffffffffffffb;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[2:0]) && is_keypoint_reg_0[3]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hfffffffffffffff7;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[3:0]) && is_keypoint_reg_0[4]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hffffffffffffffef;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[4:0]) && is_keypoint_reg_0[5]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hffffffffffffffdf;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[5:0]) && is_keypoint_reg_0[6]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hffffffffffffffbf;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[6:0]) && is_keypoint_reg_0[7]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hffffffffffffff7f;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[7:0]) && is_keypoint_reg_0[8]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hfffffffffffffeff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[8:0]) && is_keypoint_reg_0[9]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hfffffffffffffdff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[9:0]) && is_keypoint_reg_0[10]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hfffffffffffffbff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[10:0]) && is_keypoint_reg_0[11]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hfffffffffffff7ff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[11:0]) && is_keypoint_reg_0[12]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hffffffffffffefff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[12:0]) && is_keypoint_reg_0[13]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hffffffffffffdfff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[13:0]) && is_keypoint_reg_0[14]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hffffffffffffbfff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[14:0]) && is_keypoint_reg_0[15]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hffffffffffff7fff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[15:0]) && is_keypoint_reg_0[16]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hfffffffffffeffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[16:0]) && is_keypoint_reg_0[17]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hfffffffffffdffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[17:0]) && is_keypoint_reg_0[18]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hfffffffffffbffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[18:0]) && is_keypoint_reg_0[19]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hfffffffffff7ffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[19:0]) && is_keypoint_reg_0[20]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hffffffffffefffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[20:0]) && is_keypoint_reg_0[21]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hffffffffffdfffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[21:0]) && is_keypoint_reg_0[22]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hffffffffffbfffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[22:0]) && is_keypoint_reg_0[23]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hffffffffff7fffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[23:0]) && is_keypoint_reg_0[24]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hfffffffffeffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[24:0]) && is_keypoint_reg_0[25]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hfffffffffdffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[25:0]) && is_keypoint_reg_0[26]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hfffffffffbffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[26:0]) && is_keypoint_reg_0[27]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hfffffffff7ffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[27:0]) && is_keypoint_reg_0[28]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hffffffffefffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[28:0]) && is_keypoint_reg_0[29]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hffffffffdfffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[29:0]) && is_keypoint_reg_0[30]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hffffffffbfffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[30:0]) && is_keypoint_reg_0[31]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hffffffff7fffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[31:0]) && is_keypoint_reg_0[32]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hfffffffeffffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[32:0]) && is_keypoint_reg_0[33]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hfffffffdffffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[33:0]) && is_keypoint_reg_0[34]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hfffffffbffffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[34:0]) && is_keypoint_reg_0[35]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hfffffff7ffffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[35:0]) && is_keypoint_reg_0[36]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hffffffefffffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[36:0]) && is_keypoint_reg_0[37]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hffffffdfffffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[37:0]) && is_keypoint_reg_0[38]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hffffffbfffffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[38:0]) && is_keypoint_reg_0[39]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hffffff7fffffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[39:0]) && is_keypoint_reg_0[40]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hfffffeffffffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[40:0]) && is_keypoint_reg_0[41]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hfffffdffffffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[41:0]) && is_keypoint_reg_0[42]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hfffffbffffffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[42:0]) && is_keypoint_reg_0[43]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hfffff7ffffffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[43:0]) && is_keypoint_reg_0[44]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hffffefffffffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[44:0]) && is_keypoint_reg_0[45]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hffffdfffffffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[45:0]) && is_keypoint_reg_0[46]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hffffbfffffffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[46:0]) && is_keypoint_reg_0[47]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hffff7fffffffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[47:0]) && is_keypoint_reg_0[48]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hfffeffffffffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[48:0]) && is_keypoint_reg_0[49]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hfffdffffffffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[49:0]) && is_keypoint_reg_0[50]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hfffbffffffffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[50:0]) && is_keypoint_reg_0[51]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hfff7ffffffffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[51:0]) && is_keypoint_reg_0[52]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hffefffffffffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[52:0]) && is_keypoint_reg_0[53]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hffdfffffffffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[53:0]) && is_keypoint_reg_0[54]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hffbfffffffffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[54:0]) && is_keypoint_reg_0[55]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hff7fffffffffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[55:0]) && is_keypoint_reg_0[56]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hfeffffffffffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[56:0]) && is_keypoint_reg_0[57]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hfdffffffffffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[57:0]) && is_keypoint_reg_0[58]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hfbffffffffffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[58:0]) && is_keypoint_reg_0[59]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hf7ffffffffffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[59:0]) && is_keypoint_reg_0[60]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hefffffffffffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[60:0]) && is_keypoint_reg_0[61]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hdfffffffffffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[61:0]) && is_keypoint_reg_0[62]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'hbfffffffffffffff;
  else if ( (current_state==ST_FILTER || current_state==ST_NO_FILTER) && !filter_layer && (!(|is_keypoint_reg_0[62:0]) && is_keypoint_reg_0[63]) )
    is_keypoint_reg_0 <= is_keypoint_reg_0 & 64'h7fffffffffffffff;
