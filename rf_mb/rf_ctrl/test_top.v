module test_top(
    input           clk_100m_p          ,
    input           clk_100m_n          ,
    //input           rst_n               ,

    input           i_rx                ,
    output          o_tx                ,
    output          o_tx_ctrl           ,
    output [15:0]   o_rf_freq           ,
    output          o_rf_freq_ctrl      
);

wire                    clk_200m        ;
wire                    rst_n           ;

wire [31:0]             w_init          ;
wire [31:0]             w_stop          ;
wire                    w_pre_cpi       ;
wire                    w_cpi           ;

wire [31:0]             w_rf_freq       ;
wire [31:0]             w_rf_up_gain    ;
wire [31:0]             w_rf_down_gain  ;
wire                    w_rf_data_vld   ;
wire                    w_rf_ask_vld    ;

wire [31:0]             w_rf_send_num   ;
wire [31:0]             w_rf_ans_num    ;

wire [31:0]             w_rxd_1_tdata   ;
wire [3:0]              w_rxd_1_tdest   ;
wire                    w_rxd_1_tlast   ;
wire                    w_rxd_1_tready  ;
wire                    w_rxd_1_tvalid  ;
wire [31:0]             w_txd_1_tdata   ;
wire [3:0]              w_txd_1_tdest   ;
wire                    w_txd_1_tlast   ;
wire                    w_txd_1_tready  ;
wire                    w_txd_1_tvalid  ;
wire [31:0]             w_rxd_2_tdata   ;
wire [3:0]              w_rxd_2_tdest   ;
wire                    w_rxd_2_tlast   ;
wire                    w_rxd_2_tready  ;
wire                    w_rxd_2_tvalid  ;
wire [31:0]             w_txd_2_tdata   ;
wire [3:0]              w_txd_2_tdest   ;
wire                    w_txd_2_tlast   ;
wire                    w_txd_2_tready  ;
wire                    w_txd_2_tvalid  ;

wire [31:0]             gpio_1          ;
wire [31:0]             gpio_2          ;

clk_100m u_clk_100m
   (
    // Clock out ports
    .clk_out1(clk_200m),     // output clk_out1
    // Status and control signals
    .locked(rst_n ),       // output locked
   // Clock in ports
    .clk_in1_p(clk_100m_p),    // input clk_in1_p
    .clk_in1_n(clk_100m_n));    // input clk_in1_n

rf_ctrl_top u_rf_ctrl_top(
    /* input            */.clk                 (clk_200m        ),
    /* input            */.rst_n               (rst_n           ),

    /* input  [31:0]    */.i_init              (w_init          ),
    /* input  [31:0]    */.i_stop              (w_stop          ),
    /* input            */.i_pre_cpi           (w_pre_cpi       ),
    /* input            */.i_cpi               (w_cpi           ),

    /* input            */.i_rx                (i_rx            ),
    /* output           */.o_tx                (o_tx            ),
    /* output           */.o_tx_ctrl           (o_tx_ctrl       ),

    /* input  [31:0]    */.i_rf_data           (w_rf_freq       ),
    /* input  [31:0]    */.i_up_gain           (w_rf_up_gain    ),
    /* input  [31:0]    */.i_down_gain         (w_rf_down_gain  ),
    /* input            */.i_rf_data_vld       (w_rf_data_vld   ),
    /* input            */.i_rf_ask_flag       (w_rf_ask_vld    ),

    /* output [9:0]     */.o_rf_freq           (o_rf_freq       ),
    /* output           */.o_rf_freq_ctrl      (o_rf_freq_ctrl  ),
    /* output [31:0]    */.o_rf_send_num       (w_rf_send_num   ),
    /* output [31:0]    */.o_rf_ans_num        (w_rf_ans_num    )
);

test_wrapper u_test_wrapper(
    .AXI_STR_RXD_1_tdata        (w_rxd_1_tdata  ),
    .AXI_STR_RXD_1_tdest        (w_rxd_1_tdest  ),
    .AXI_STR_RXD_1_tlast        (w_rxd_1_tlast  ),
    .AXI_STR_RXD_1_tready       (w_rxd_1_tready ),
    .AXI_STR_RXD_1_tvalid       (w_rxd_1_tvalid ),
    .AXI_STR_TXD_1_tdata        (w_txd_1_tdata  ),
    .AXI_STR_TXD_1_tlast        (w_txd_1_tlast  ),
    .AXI_STR_TXD_1_tready       (w_txd_1_tready ),
    .AXI_STR_TXD_1_tvalid       (w_txd_1_tvalid ),
    .clk_in1_0                  (clk_200m       ),
    .gpio_rtl_3_tri_o           (gpio_1         ),
    .gpio_rtl_4_tri_o           (gpio_2         ),
    .init_tri_o                 (w_init         ),
    .pre_cpi_tri_o              (w_pre_cpi      ),
    .reset_rtl_0                (rst_n          ),
    .rf_ans_num_tri_i           (w_rf_ans_num   ),
    .rf_ask_flag_tri_o          (w_rf_ask_vld   ),
    .rf_ctrl_tri_o              (w_rf_data_vld  ),
    .rf_down_gain_tri_o         (w_rf_down_gain ),
    .rf_freq_tri_o              (w_rf_freq      ),
    .rf_send_num_tri_i          (w_rf_send_num  ),
    .rf_up_gain_tri_o           (w_rf_up_gain   ),
    .stop_tri_o                 (w_stop         ),
    .test_receive_data_tdata    (w_rxd_2_tdata  ),
    .test_receive_data_tdest    (w_rxd_2_tdest  ),
    .test_receive_data_tlast    (w_rxd_2_tlast  ),
    .test_receive_data_tready   (w_rxd_2_tready ),
    .test_receive_data_tvalid   (w_rxd_2_tvalid ),
    .test_send_data_tdata       (w_txd_2_tdata  ),
    .test_send_data_tdest       (w_txd_2_tdest  ),
    .test_send_data_tlast       (w_txd_2_tlast  ),
    .test_send_data_tready      (w_txd_2_tready ),
    .test_send_data_tvalid      (w_txd_2_tvalid )
);
endmodule