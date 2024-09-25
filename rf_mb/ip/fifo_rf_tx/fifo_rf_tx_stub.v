// Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2018.3 (win64) Build 2405991 Thu Dec  6 23:38:27 MST 2018
// Date        : Tue Jun 25 14:13:51 2024
// Host        : ITIANKONG running 64-bit major release  (build 9200)
// Command     : write_verilog -force -mode synth_stub D:/1Y14S/test/srcs/rf_mb/ip/fifo_rf_tx/fifo_rf_tx_stub.v
// Design      : fifo_rf_tx
// Purpose     : Stub declaration of top-level module interface
// Device      : xcvu9p-flgb2104-2-i
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* x_core_info = "fifo_generator_v13_2_3,Vivado 2018.3" *)
module fifo_rf_tx(clk, rst, din, wr_en, rd_en, dout, full, empty, 
  prog_full, prog_empty)
/* synthesis syn_black_box black_box_pad_pin="clk,rst,din[7:0],wr_en,rd_en,dout[7:0],full,empty,prog_full,prog_empty" */;
  input clk;
  input rst;
  input [7:0]din;
  input wr_en;
  input rd_en;
  output [7:0]dout;
  output full;
  output empty;
  output prog_full;
  output prog_empty;
endmodule
