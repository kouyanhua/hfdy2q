`timescale 1ns/1ps 
module rx_chk_tb (); 
    reg           clk             ;
    reg           rst_n           ;

    reg           i_fifo_empty    ;
    reg  [7:0]    rx_data         ;
    reg           rx_data_vld     ;
    wire          rx_fifo_rd_en   ;

    wire [31:0]   rf_ans_state    ;

//模块例化
rf_rx_chk u_rf_rx_chk(
    /* input            */.clk             (clk           ),
    /* input            */.rst_n           (rst_n         ),

    /* input            */.i_fifo_empty    (i_fifo_empty  ),
    /* input  [7:0]     */.rx_data         (rx_data       ),
    /* input            */.rx_data_vld     (rx_fifo_rd_en ),
    /* output           */.rx_fifo_rd_en   (rx_fifo_rd_en ),

    /* output [31:0]    */.rf_ans_state    (rf_ans_state  )
);


localparam CLK_PERIOD = 5;
initial clk = 1'b0 ;
always #2.5 clk = ~clk;

initial begin
    rst_n = 1'b0 ;
    #(CLK_PERIOD*3);
    rst_n = 1'b1 ;
end

initial begin
    i_fifo_empty = 1'b1;
    rx_data      = 8'd0;
    #(CLK_PERIOD*5);
    i_fifo_empty = 1'b0;
    rx_data      = 8'd0;
    rx_data      = 8'd0;
    i_fifo_empty = 1'b1;
end

endmodule