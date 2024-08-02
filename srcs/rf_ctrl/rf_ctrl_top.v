module rf_ctrl_top (
    input           clk                 ,
    input           rst_n               ,

    input  [31:0]   i_init              ,
    input  [31:0]   i_stop              ,
    input           i_pre_cpi           ,
    input           i_cpi               ,

    input           i_rx                ,
    output          o_tx                ,
    output          o_tx_ctrl           ,

    input  [31:0]   i_rf_data           ,
    input  [31:0]   i_up_gain           ,
    input  [31:0]   i_down_gain         ,
    input           i_rf_data_vld       ,
    input  [31:0]   i_rf_ask_flag       ,

    output [15:0]   o_rf_freq           ,
    output          o_rf_freq_ctrl      ,
    output [31:0]   o_rf_send_num       ,
    output [31:0]   o_rf_ans_num        
);

reg                 i_init_r1,i_init_r2;
reg                 i_stop_r1,i_stop_r2;
reg  				i_ask_flag_r1,i_ask_flag_r2;
wire [9:0]          rf_freq             ;
wire                w_tx_over_flag      ;

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        i_init_r1 <= 1'b0;
        i_init_r2 <= 1'b0;
    end
    else begin
        i_init_r1 <= i_init[0];
        i_init_r2 <= i_init_r1;
    end
end

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        i_stop_r1 <= 1'b0;
        i_stop_r2 <= 1'b0;
    end
    else begin
        i_stop_r1 <= i_stop[0];
        i_stop_r2 <= i_stop_r1;
    end
end

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        i_ask_flag_r1 <= 1'b0;
        i_ask_flag_r2 <= 1'b0;
    end
    else begin
        i_ask_flag_r1 <= i_rf_ask_flag[0];
        i_ask_flag_r2 <= i_ask_flag_r1;
    end
end

assign rf_freq = i_rf_data[15:0]/10;

uart_top u_uart_top(
    /* input            */.clk                 (clk             ),
    /* input            */.rst_n               (rst_n           ),

    /* input            */.i_rx                (i_rx            ),
    /* output           */.o_tx                (o_tx            ),
    /* output           */.o_tx_ctrl           (o_tx_ctrl       ),

    /* input            */.i_init              (i_init_r2       ),
    /* input            */.i_stop              (i_stop_r2       ),
    /* input            */.i_pre_cpi           (i_pre_cpi       ),
    /* input            */.i_cpi               (i_cpi           ),

    /* input            */.i_rf_ask_flag       (i_ask_flag_r2   ),
    /* input            */.i_rf_ctrl_vld       (i_rf_data_vld   ),
    /* input  [15:0]    */.i_rfc_agin          ({i_up_gain[15:0],i_down_gain[15:0]}),
    /* input  [15:0]    */.i_rf_freq           (i_rf_data[15:0] ),
    /* output           */.o_tx_over_flag      (w_tx_over_flag  ),

    /* output [31:0]    */.o_rf_send_num       (o_rf_send_num   ),
    /* output [31:0]    */.o_rf_ans_num        (o_rf_ans_num    )
);

rf_parallel u_rf_parallel(
    /* input            */.clk                 (clk             ),
    /* input            */.rst_n               (rst_n           ),

    /* input  [9:0]     */.i_rf_freq           (i_rf_data[15:0] ),
    /* input            */.i_rf_freq_vld       (i_rf_data_vld   ),
    /* input            */.i_init              (i_init_r2       ),
    /* input            */.i_stop              (i_stop_r2       ),
    /* input            */.i_pre_cpi           (i_pre_cpi       ),
    /* input            */.i_cpi               (i_cpi           ),
    /* input            */.i_tx_over_flag      (w_tx_over_flag  ),

    /* output [9:0]     */.o_rf_freq           (o_rf_freq       ),
    /* output           */.o_rf_freq_ctrl      (o_rf_freq_ctrl  )
);
    
endmodule