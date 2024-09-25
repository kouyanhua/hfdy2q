// Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2018.3 (win64) Build 2405991 Thu Dec  6 23:38:27 MST 2018
// Date        : Tue Jun 25 13:58:50 2024
// Host        : ITIANKONG running 64-bit major release  (build 9200)
// Command     : write_verilog -force -mode synth_stub D:/1Y14S/test/srcs/rf_mb/ip/ila_rf_pack/ila_rf_pack_stub.v
// Design      : ila_rf_pack
// Purpose     : Stub declaration of top-level module interface
// Device      : xcvu9p-flgb2104-2-i
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* X_CORE_INFO = "ila,Vivado 2018.3" *)
module ila_rf_pack(clk, probe0, probe1, probe2, probe3, probe4, probe5, 
  probe6, probe7, probe8, probe9, probe10, probe11, probe12)
/* synthesis syn_black_box black_box_pad_pin="clk,probe0[0:0],probe1[0:0],probe2[15:0],probe3[15:0],probe4[0:0],probe5[0:0],probe6[7:0],probe7[0:0],probe8[31:0],probe9[2:0],probe10[2:0],probe11[2:0],probe12[7:0]" */;
  input clk;
  input [0:0]probe0;
  input [0:0]probe1;
  input [15:0]probe2;
  input [15:0]probe3;
  input [0:0]probe4;
  input [0:0]probe5;
  input [7:0]probe6;
  input [0:0]probe7;
  input [31:0]probe8;
  input [2:0]probe9;
  input [2:0]probe10;
  input [2:0]probe11;
  input [7:0]probe12;
endmodule
