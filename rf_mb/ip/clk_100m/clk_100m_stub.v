// Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2018.3 (win64) Build 2405991 Thu Dec  6 23:38:27 MST 2018
// Date        : Tue Mar  5 16:22:28 2024
// Host        : ITIANKONG running 64-bit major release  (build 9200)
// Command     : write_verilog -force -mode synth_stub
//               D:/1Y14S/1y14s_top/vu9p_rfctrl_mb/vu9p_rfctrl_mb/vu9p_rfctrl_mb.srcs/sources_1/ip/clk_100m/clk_100m_stub.v
// Design      : clk_100m
// Purpose     : Stub declaration of top-level module interface
// Device      : xcvu9p-flgb2104-2-i
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
module clk_100m(clk_out1, locked, clk_in1_p, clk_in1_n)
/* synthesis syn_black_box black_box_pad_pin="clk_out1,locked,clk_in1_p,clk_in1_n" */;
  output clk_out1;
  output locked;
  input clk_in1_p;
  input clk_in1_n;
endmodule
