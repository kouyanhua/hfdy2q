-- Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2018.3 (win64) Build 2405991 Thu Dec  6 23:38:27 MST 2018
-- Date        : Tue Jun 25 14:18:15 2024
-- Host        : ITIANKONG running 64-bit major release  (build 9200)
-- Command     : write_vhdl -force -mode synth_stub D:/1Y14S/test/srcs/rf_mb/ip/ila_rx_chk/ila_rx_chk_stub.vhdl
-- Design      : ila_rx_chk
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xcvu9p-flgb2104-2-i
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity ila_rx_chk is
  Port ( 
    clk : in STD_LOGIC;
    probe0 : in STD_LOGIC_VECTOR ( 0 to 0 );
    probe1 : in STD_LOGIC_VECTOR ( 7 downto 0 );
    probe2 : in STD_LOGIC_VECTOR ( 0 to 0 );
    probe3 : in STD_LOGIC_VECTOR ( 0 to 0 );
    probe4 : in STD_LOGIC_VECTOR ( 31 downto 0 );
    probe5 : in STD_LOGIC_VECTOR ( 2 downto 0 );
    probe6 : in STD_LOGIC_VECTOR ( 2 downto 0 );
    probe7 : in STD_LOGIC_VECTOR ( 7 downto 0 )
  );

end ila_rx_chk;

architecture stub of ila_rx_chk is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "clk,probe0[0:0],probe1[7:0],probe2[0:0],probe3[0:0],probe4[31:0],probe5[2:0],probe6[2:0],probe7[7:0]";
attribute X_CORE_INFO : string;
attribute X_CORE_INFO of stub : architecture is "ila,Vivado 2018.3";
begin
end;
