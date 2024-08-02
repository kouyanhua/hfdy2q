`timescale 1ns/1ps
/* ///////////////////////////////////////////////////////////////////////////// */
//Org         : raco
//Created by  : kou
//Time        : 2024-03-01
//Prj Name    :   
//Module Name : uart_top
//Target Dev  : xcvu9p-flgb2104-2-i
//Tool ver    : vivado2018.3

//Description : This module is the top for uart;
//Modification Record
// |  author  | time        |      contents  
// ----------------------------------------------------------------------------               
// |  kou     | 2023-03-01  | 1.create the file              
// |  xxx     | 2023-xx-xx  |       
module uart_top (
    input           clk                 ,
    input           rst_n               ,

    input           i_rx                ,
    output          o_tx                ,
    output          o_tx_ctrl           ,

    input           i_init              ,
    input           i_stop              ,
    input           i_pre_cpi           ,
    input           i_cpi               ,

    input           i_rf_ask_flag       ,
    input           i_rf_ctrl_vld       ,
    input  [31:0]   i_rfc_agin          ,
    input  [15:0]   i_rf_freq           ,
    output          o_tx_over_flag      ,

    output [31:0]   o_rf_send_num       ,
    output [31:0]   o_rf_ans_num        
);

wire  [7:0]         rx_data2fifo        ;
wire                rx_data2fifo_vld    ;
wire  [7:0]         rx_fifo2data_chk    ;
wire                fifo_rx_rd_en       ;

wire  [7:0]         tx_data2fifo        ;
wire                tx_data2fifo_vld    ;
wire  [7:0]         tx_fifo2data        ;
wire                fifo_tx_rd_en       ;

wire                fifo_rx_full        ;
wire                fifo_rx_empty       ;
wire                fifo_rx_prog_full   ;
wire                fifo_rx_prog_empty  ;

wire                fifo_tx_full        ;
wire                fifo_tx_empty       ;
wire                fifo_tx_prog_full   ;
wire                fifo_tx_prog_empty  ;

wire                w_tx_ctrl_over      ;
wire                w_tx_ask_ctrl       ;
wire                tx_busy             ;

rf_rx u_rf_rx(
    /* input            */.clk                 (clk             ),
    /* input            */.rst_n               (rst_n           ),
    /* input            */.i_rx                (i_rx            ),
    /* output [7:0]     */.o_rx_data           (rx_data2fifo    ),
    /* output           */.o_rx_data_vld       (rx_data2fifo_vld),

    /* output [7:0]     */.o_rx_cnt            (),
    /* output [7:0]     */.o_err_cnt           ()
);

fifo_rf_rx u_fifo_rf_rx (
  .clk                  (clk                ),// input wire clk
  .rst                  (!rst_n             ),// input wire rst
  .din                  (rx_data2fifo       ),// input wire [7 : 0] din
  .wr_en                (rx_data2fifo_vld   ),// input wire wr_en
  .rd_en                (fifo_rx_rd_en      ),// input wire rd_en
  .dout                 (rx_fifo2data_chk   ),// output wire [7 : 0] dout
  .full                 (fifo_rx_full       ),// output wire full
  .empty                (fifo_rx_empty      ),// output wire empty
  .prog_full            (fifo_rx_prog_full  ),// output wire prog_full
  .prog_empty           (fifo_rx_prog_empty )// output wire prog_empty
);

rf_rx_chk u_rf_rx_chk(
    /* input            */.clk                 (clk                 ),
    /* input            */.rst_n               (rst_n               ),

    /* input            */.i_fifo_empty        (fifo_rx_empty  ),
    /* input            */.i_fifo_prog_empty   (fifo_rx_prog_empty  ),
    /* input  [7:0]     */.rx_data             (rx_fifo2data_chk    ),
    /* input            */.rx_data_vld         (fifo_rx_rd_en       ),
    /* output           */.rx_fifo_rd_en       (fifo_rx_rd_en       ),

    /* output [31:0]    */.rf_ans_state        (o_rf_ans_num        )
);

rf_pack u_rf_pack(
    /* input            */.clk                 (clk             ),
    /* input            */.rst_n               (rst_n           ),

    /* input            */.i_rf_ask_flag       (i_rf_ask_flag   ),
    /* input            */.i_rf_ctrl_vld       (i_rf_ctrl_vld   ),
    /* input   [15:0]   */.i_rfc_agin          (i_rfc_agin      ),
    /* input   [15:0]   */.i_rf_freq           (i_rf_freq       ),

    /* input            */.i_init              (i_init          ),
    /* input            */.i_stop              (i_stop          ),

    /* output  [7:0]    */.o_rf_data           (tx_data2fifo    ),
    /* output           */.o_rf_vld            (tx_data2fifo_vld),
    /* output  [31:0]   */.o_rf_send_num       (o_rf_send_num   )
);

fifo_rf_tx u_fifo_rf_tx (
  .clk              (clk                ),// input wire clk
  .rst              (!rst_n              ),// input wire rst
  .din              (tx_data2fifo       ),// input wire [7 : 0] din
  .wr_en            (tx_data2fifo_vld   ),// input wire wr_en
  .rd_en            (fifo_tx_rd_en      ),// input wire rd_en
  .dout             (tx_fifo2data       ),// output wire [7 : 0] dout
  .full             (fifo_tx_full       ),// output wire full
  .empty            (fifo_tx_empty      ),// output wire empty
  .prog_full        (fifo_tx_prog_full  ),// output wire prog_full
  .prog_empty       (fifo_tx_prog_empty )// output wire prog_empty
);

rf_tx u_rf_tx(
    /* input            */.clk                 (clk             ),
    /* input            */.rst_n               (rst_n           ),
    
    /* input  [7:0]     */.i_tx_data           (tx_fifo2data    ),
    /* input            */.i_tx_data_vld       (fifo_tx_rd_en   ),
    /* input            */.i_fifo_empty        (fifo_tx_empty   ),
    /* input            */.i_rf_ctrl_vld       (i_rf_ctrl_vld   ),
    
    /* input            */.i_init              (i_init          ),
    /* input            */.i_stop              (i_stop          ),
    /* input            */.i_tx_ctrl_over      (w_tx_ctrl_over  ),
    
    /* output           */.o_read_en           (fifo_tx_rd_en   ),
    /* output           */.o_tx_busy           (tx_busy         ),
    /* output           */.o_tx                (o_tx            ),
    /* output           */.o_tx_ctrl           (w_tx_ask_ctrl   )
);

rf_tx_timing u_rf_tx_timing(
    /* input            */.clk                 (clk             ),
    /* input            */.rst_n               (rst_n           ),
    
    /* input            */.i_tx_ctrl           (w_tx_ask_ctrl   ),
    /* input            */.i_init              (i_init          ),
    /* input            */.i_stop              (i_stop          ),
    /* input            */.i_pre_cpi           (i_pre_cpi       ),
    /* input            */.i_cpi               (i_cpi           ),
    
    /* output           */.o_tx_ctrl           (o_tx_ctrl       ),
    /* output           */.o_ctrl_over_flag    (w_tx_ctrl_over  )
);

assign o_tx_over_flag = w_tx_ctrl_over;

endmodule