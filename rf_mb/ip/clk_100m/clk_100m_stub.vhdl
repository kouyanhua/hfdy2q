-- Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2018.3 (win64) Build 2405991 Thu Dec  6 23:38:27 MST 2018
-- Date        : Tue Mar  5 16:22:28 2024
-- Host        : ITIANKONG running 64-bit major release  (build 9200)
-- Command     : write_vhdl -force -mode synth_stub
--               D:/1Y14S/1y14s_top/vu9p_rfctrl_mb/vu9p_rfctrl_mb/vu9p_rfctrl_mb.srcs/sources_1/ip/clk_100m/clk_100m_stub.vhdl
-- Design      : clk_100m
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xcvu9p-flgb2104-2-i
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity clk_100m is
  Port ( 
    clk_out1 : out STD_LOGIC;
    locked : out STD_LOGIC;
    clk_in1_p : in STD_LOGIC;
    clk_in1_n : in STD_LOGIC
  );

end clk_100m;

architecture stub of clk_100m is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "clk_out1,locked,clk_in1_p,clk_in1_n";
begin
end;
