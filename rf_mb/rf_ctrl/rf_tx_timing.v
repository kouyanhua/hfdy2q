`timescale 1ns/1ps
/* ///////////////////////////////////////////////////////////////////////////// */
//Org         : raco
//Created by  : kou
//Time        : 2024-03-01
//Prj Name    :   
//Module Name : rf_pack
//Target Dev  : xcvu9p-flgb2104-2-i
//Tool ver    : vivado2018.3

//Description : This module create the timing to ctrl rf module;
//Modification Record
// |  author  | time        |      contents  
// ----------------------------------------------------------------------------               
// |  kou     | 2023-03-01  | 1.create the file              
// |  kou     | 2023-03-01  | 1.add timing module logic
// |  xxx     | 2023-xx-xx  |       
module rf_tx_timing (
    input           clk             ,
    input           rst_n           ,

    input           i_tx_ctrl       ,
    input           i_init          ,
    input           i_stop          ,
    input           i_pre_cpi       ,
    input           i_cpi           ,

    output          o_tx_ctrl       ,
    output          o_ctrl_over_flag
);

reg             fpga_state          ;
reg             tx_cpi_ctrl         ;

reg  [7:0]			cnt_1us			;
wire				add_cnt_1us		;
wire				end_cnt_1us		;
reg                 add_flag        ;

reg  [3:0]          pre_cpi_r       ;
wire                pedge_pre_cpi   ;

reg                 o_tx_ctrl_r     ;

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        pre_cpi_r <= 4'd0;
    end
    else begin
        pre_cpi_r[0] <= i_pre_cpi;
        pre_cpi_r[1] <= pre_cpi_r[0];
        pre_cpi_r[2] <= pre_cpi_r[1];
        pre_cpi_r[3] <= pre_cpi_r[2];
    end
end
assign pedge_pre_cpi = (!pre_cpi_r[3])&&(pre_cpi_r[2]);

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        add_flag <= 1'b0;
    end
    else if(pedge_pre_cpi)begin
        add_flag <= 1'b1;
    end
    else if(end_cnt_1us)begin
        add_flag <= 1'b0;
    end
    else begin
        add_flag <= add_flag;
    end
end

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        cnt_1us <= 0;
    end
    else if(add_cnt_1us)begin
        if(end_cnt_1us)begin
            cnt_1us <= 0;
        end
        else begin
            cnt_1us <= cnt_1us + 1;
        end
    end
    else begin
        cnt_1us <= cnt_1us ;
    end
end
assign add_cnt_1us = add_flag ;
assign end_cnt_1us = add_cnt_1us && cnt_1us == 200 - 1 ;

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        tx_cpi_ctrl <= 1'b0;
    end
    else if(add_flag)begin
        tx_cpi_ctrl <= 1'b1;
    end
    else begin
        tx_cpi_ctrl <= 1'b0;
    end
end

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        fpga_state <= 1'b0;
    end
    else if(i_stop)begin
        fpga_state <= 1'b0;
    end
    else if(i_init)begin
        fpga_state <= 1'b1;
    end
    else begin
        fpga_state <= fpga_state;
    end
end

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        o_tx_ctrl_r <= 1'b0;
    end
    else if(fpga_state)begin
        o_tx_ctrl_r <= tx_cpi_ctrl;
    end
    else begin
        o_tx_ctrl_r <= i_tx_ctrl;
    end
end

assign o_tx_ctrl = o_tx_ctrl_r;
assign o_ctrl_over_flag = end_cnt_1us;

ila_timing u_ila_timing (
	.clk(clk), // input wire clk


	.probe0     (i_tx_ctrl      ), // input wire [0:0]  probe0  
	.probe1     (i_init         ), // input wire [0:0]  probe1 
	.probe2     (i_stop         ), // input wire [0:0]  probe2 
	.probe3     (i_pre_cpi      ), // input wire [0:0]  probe3 
	.probe4     (i_cpi          ), // input wire [0:0]  probe4 
	.probe5     (o_tx_ctrl       ), // input wire [0:0]  probe5 
	.probe6     (o_ctrl_over_flag), // input wire [0:0]  probe6 
	.probe7     (cnt_1us        ), // input wire [7:0]  probe7 
	.probe8     (fpga_state     ), // input wire [0:0]  probe8 
	.probe9     (tx_cpi_ctrl    ) // input wire [0:0]  probe9
);

endmodule