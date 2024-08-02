`timescale 1ns/1ps
/* ///////////////////////////////////////////////////////////////////////////// */
//Org         : raco
//Created by  : kou
//Time        : 2024-03-01
//Prj Name    :   
//Module Name : rf_rx_chk
//Target Dev  : xcvu9p-flgb2104-2-i
//Tool ver    : vivado2018.3

//Description : This module chacks uart data;
//Modification Record
// |  author  | time        |      contents  
// ----------------------------------------------------------------------------               
// |  kou     | 2023-03-01  | 1.create the file              
// |  kou     | 2023-03-01  | 1.add chack module logic
// |  xxx     | 2023-xx-xx  |       
module rf_rx_chk (
    input           clk             ,
    input           rst_n           ,

    input           i_fifo_empty    ,
    input           i_fifo_prog_empty    ,
    input  [7:0]    rx_data         ,
    input           rx_data_vld     ,
    output          rx_fifo_rd_en   ,

    output [31:0]   rf_ans_state    
);

parameter           IDLE = 3'b001,
                    DATA = 3'b010,
                    CHK  = 3'b100;

wire                idle2data           ;
wire                data2chk            ;
wire                chk2idle            ;

reg  [2:0]          state_c             ;
reg  [2:0]          state_n             ;

reg  [15:0]			cnt_data			;
wire				add_cnt_data		;
wire				end_cnt_data		;

reg  [7:0]          chk_r               ;
reg  [15:0]         pack_total_num      ;
reg  [15:0]         pack_err_num        ;


always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        state_c <= IDLE;
    end
    else begin
        state_c <= state_n;
    end
end
always@(*) begin//组合逻辑
    case (state_c)
        IDLE : begin
            if(idle2data) begin
                state_n = DATA;
            end
            else begin
                state_n = IDLE;
            end
        end
        DATA : begin
            if(data2chk) begin
                state_n = CHK;
            end
            else begin
                state_n = DATA;
            end
        end
        CHK  : begin
            if(chk2idle) begin
                state_n = IDLE;
            end
            else begin
                state_n = CHK;
            end
        end
        default : state_n = IDLE;
    endcase
end

assign idle2data = (state_c == IDLE) && (!i_fifo_prog_empty);
assign data2chk  = (state_c == DATA) && (end_cnt_data);
assign chk2idle  = (state_c == CHK ) && (rx_data_vld);

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        cnt_data <= 0;
    end
    else if(add_cnt_data)begin
        if(end_cnt_data)begin
            cnt_data <= 0;
        end
        else begin
            cnt_data <= cnt_data + 1;
        end
    end
    else begin
        cnt_data <= cnt_data ;
    end
end
assign add_cnt_data = (state_c == DATA)&&rx_data_vld ;
assign end_cnt_data = add_cnt_data && cnt_data == 12 - 1 ;

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        chk_r <= 8'd0;
    end
    else if((state_c == DATA)&&rx_data_vld)begin
        chk_r <= chk_r ^ rx_data;
    end
    else if(chk2idle)begin
        chk_r <= 8'd0;
    end
    else begin
        chk_r <= chk_r;
    end
end

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        pack_total_num <= 16'd0;
    end
    else if(chk2idle)begin
        pack_total_num <= pack_total_num +1;
    end
    else begin
        pack_total_num <= pack_total_num;
    end
end

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        pack_err_num <= 16'd0;
    end
    else if((state_c == CHK)&&rx_data_vld&&(chk_r != rx_data))begin
        pack_err_num <= pack_err_num +1;
    end
    else begin
        pack_err_num <= pack_err_num;
    end
end

assign rf_ans_state = {pack_total_num,pack_err_num};
assign rx_fifo_rd_en = (((state_c == DATA)||(state_c == CHK))&&(!i_fifo_empty));

ila_rx_chk u_ila_rx_chk (
	.clk(clk), // input wire clk


	.probe0     (i_fifo_empty       ), // input wire [0:0]  probe0  
	.probe1     (rx_data            ), // input wire [7:0]  probe1 
	.probe2     (rx_data_vld        ), // input wire [0:0]  probe2 
	.probe3     (rx_fifo_rd_en      ), // input wire [0:0]  probe3 
	.probe4     (rf_ans_state       ), // input wire [31:0]  probe4 
	.probe5     (state_c            ), // input wire [2:0]  probe5 
	.probe6     (cnt_data           ), // input wire [2:0]  probe6 
	.probe7     (chk_r              ) // input wire [7:0]  probe7
);

endmodule