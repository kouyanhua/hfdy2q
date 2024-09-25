`timescale 1ns/1ps
/* ///////////////////////////////////////////////////////////////////////////// */
//Org         : raco
//Created by  : kou
//Time        : 2024-03-01
//Prj Name    :   
//Module Name : rf_tx
//Target Dev  : xcvu9p-flgb2104-2-i
//Tool ver    : vivado2018.3

//Description : This module sends information to uart;
//Modification Record
// |  author  | time        |      contents  
// ----------------------------------------------------------------------------               
// |  kou     | 2023-03-01  | 1.create the file              
// |  kou     | 2023-03-01  | 1.add create module logic
// |  xxx     | 2023-xx-xx  |       
module rf_tx (
    input           clk             ,
    input           rst_n           ,

    input  [7:0]    i_tx_data       ,
    input           i_tx_data_vld   ,
    input           i_fifo_empty    ,
    input           i_rf_ctrl_vld   ,

    input           i_init          ,
    input           i_stop          ,
    input           i_tx_ctrl_over  ,

    output          o_read_en       ,
    output          o_tx_busy       ,
    output          o_tx            ,
    output          o_tx_ctrl       
);

parameter           BAUD      = 1736    ,
                    HALF_BAUD = 868     ;

parameter           WAIT_TIME = 200_000,
                    HOLD_TIME = 200;

parameter           IDLE      = 7'b0000001 ,
                    START     = 7'b0000010 ,
                    DATA      = 7'b0000100 ,
                    STOP      = 7'b0001000 ,
                    WAIT      = 7'b0010000 ,
                    HOLD      = 7'b0100000 ,
                    CTRL_WAIT = 7'b1000000;

wire                idle2start          ;
wire                start2data          ;
wire                data2stop           ;
wire                stop2idle           ;
wire                stop2wait           ;
wire                stop2ctrl_wait      ;
wire                wait2hold           ;
wire                hold2idle           ;
wire                ctrl_wait2idle      ;

reg  [6:0]          state_c             ;
reg  [6:0]          state_n             ;

reg  [11:0]			cnt_baud			;
wire				add_cnt_baud		;
wire				end_cnt_baud		;
reg  [3:0]			cnt_bit			    ;
wire				add_cnt_bit		    ;
wire				end_cnt_bit		    ;
reg  [3:0]			cnt_byte			;
wire				add_cnt_byte		;
wire				end_cnt_byte		;
reg  [3:0]          cnt_byte_times      ;
reg  [19:0]			cnt_wait			;
wire				add_cnt_wait		;
wire				end_cnt_wait		;
reg  [11:0]			cnt_hold			;
wire				add_cnt_hold		;
wire				end_cnt_hold		;

reg  [7:0]          tx_data_r           ;
reg                 tx_r                ;
reg                 tx_ctrl_r           ;
reg                 read_en_r           ;
reg                 tx_busy_r           ;

reg                 ask_over_flag       ;

reg                 fpga_state          ;

reg                 end_cnt_byte_flag   ;
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
        IDLE  :begin
            if(idle2start) begin
                state_n = START;
            end
            else begin
                state_n = IDLE;
            end
        end
        START :begin
            if(start2data) begin
                state_n = DATA;
            end
            else begin
                state_n = START;
            end
        end
        DATA  :begin
            if(data2stop) begin
                state_n = STOP;
            end
            else begin
                state_n = DATA;
            end
        end
        STOP  :begin
            if(stop2wait) begin
                state_n = WAIT;
            end
            else if(stop2ctrl_wait)begin
                state_n = CTRL_WAIT;
            end
            else if(stop2idle) begin
                state_n = IDLE;
            end
            else begin
                state_n = STOP;
            end
        end
        WAIT  :begin
            if(wait2hold) begin
                state_n = HOLD;
            end
            else begin
                state_n = WAIT;
            end
        end
        HOLD  :begin
            if(hold2idle) begin
                state_n = IDLE;
            end
            else begin
                state_n = HOLD;
            end
        end
        CTRL_WAIT:begin
            if(ctrl_wait2idle)begin
                state_n = IDLE;
            end
            else begin
                state_n = CTRL_WAIT;
            end
        end
        default : state_n = IDLE;
    endcase
end

assign idle2start = (state_c == IDLE ) && (!i_fifo_empty);
assign start2data = (state_c == START) && (end_cnt_baud);
assign data2stop  = (state_c == DATA ) && (end_cnt_bit);
assign stop2idle  = (state_c == STOP ) && (end_cnt_baud);
assign stop2wait  = (state_c == STOP ) && (end_cnt_byte_flag) && (fpga_state == 1'b0)&& (end_cnt_baud);
assign stop2ctrl_wait = (state_c == STOP ) && (end_cnt_byte_flag) &&(fpga_state == 1'b1)&& (end_cnt_baud);
assign wait2hold  = (state_c == WAIT ) && (end_cnt_wait);
assign hold2idle  = (state_c == HOLD ) && (end_cnt_hold);
assign ctrl_wait2idle = (state_c == CTRL_WAIT) && (i_tx_ctrl_over);


always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        cnt_baud <= 0;
    end
    else if(add_cnt_baud)begin
        if(end_cnt_baud)begin
            cnt_baud <= 0;
        end
        else begin
            cnt_baud <= cnt_baud + 1;
        end
    end
    else begin
        cnt_baud <= cnt_baud ;
    end
end
assign add_cnt_baud = ((state_c == START)||(state_c == DATA)||(state_c == STOP)) ;
assign end_cnt_baud = add_cnt_baud && cnt_baud == BAUD - 1 ;

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        cnt_bit <= 0;
    end
    else if(add_cnt_bit)begin
        if(end_cnt_bit)begin
            cnt_bit <= 0;
        end
        else begin
            cnt_bit <= cnt_bit + 1;
        end
    end
    else begin
        cnt_bit <= cnt_bit ;
    end
end
assign add_cnt_bit = (state_c == DATA)&&(end_cnt_baud) ;
assign end_cnt_bit = add_cnt_bit && cnt_bit == 8 - 1 ;

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        cnt_byte <= 0;
    end
    else if(add_cnt_byte)begin
        if(end_cnt_byte)begin
            cnt_byte <= 0;
        end
        else begin
            cnt_byte <= cnt_byte + 1;
        end
    end
    else begin
        cnt_byte <= cnt_byte ;
    end
end
assign add_cnt_byte = data2stop ;
assign end_cnt_byte = add_cnt_byte && cnt_byte == cnt_byte_times - 1 ;

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        cnt_byte_times <= 4'd4;
    end
    else if(fpga_state)begin
        cnt_byte_times <= 4'd9;
    end
    else begin
        cnt_byte_times <= 4'd4;
    end
end

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        end_cnt_byte_flag <= 1'b0;
    end
    else if(end_cnt_byte)begin
        end_cnt_byte_flag <= 1'b1;
    end
    else if(stop2wait||stop2ctrl_wait)begin
        end_cnt_byte_flag <= 1'b0;
    end
    else begin
        end_cnt_byte_flag <= end_cnt_byte_flag ;
    end
end

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        cnt_wait <= 0;
    end
    else if(add_cnt_wait)begin
        if(end_cnt_wait)begin
            cnt_wait <= 0;
        end
        else begin
            cnt_wait <= cnt_wait + 1;
        end
    end
    else begin
        cnt_wait <= cnt_wait ;
    end
end
assign add_cnt_wait = (state_c == WAIT) ;
assign end_cnt_wait = add_cnt_wait && cnt_wait == WAIT_TIME - 1 ;

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        cnt_hold <= 0;
    end
    else if(add_cnt_hold)begin
        if(end_cnt_hold)begin
            cnt_hold <= 0;
        end
        else begin
            cnt_hold <= cnt_hold + 1;
        end
    end
    else begin
        cnt_hold <= cnt_hold ;
    end
end
assign add_cnt_hold = (state_c == HOLD) ;
assign end_cnt_hold = add_cnt_hold && cnt_hold == HOLD_TIME - 1 ;

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        ask_over_flag <= 1'b0;
    end
    else if(i_rf_ctrl_vld)begin
        ask_over_flag <= 1'b1;
    end
    else begin
        ask_over_flag <= ask_over_flag;
    end
end

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        tx_data_r <= 8'd0;
    end
    else if(i_tx_data_vld)begin
        tx_data_r <= i_tx_data;
    end
    else begin
        tx_data_r <= tx_data_r;
    end
end

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        tx_r <= 1'b1;
    end
    else if(state_c == START)begin
        tx_r <= 1'b0;
    end
    else if(state_c == DATA)begin
        tx_r <= tx_data_r[cnt_bit];
    end
    else begin
        tx_r <= 1'b1;
    end
end

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        read_en_r <= 1'b0;
    end
    else if(idle2start)begin
        read_en_r <= 1'b1;
    end
    else begin
        read_en_r <= 1'b0;
    end
end

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        tx_ctrl_r <= 1'b0;
    end
    else if(state_c == HOLD)begin
        tx_ctrl_r <= 1'b1;
    end
    else begin
        tx_ctrl_r <= 1'b0;
    end
end

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        tx_busy_r <= 1'b0;
    end
    else if(state_c != IDLE)begin
        tx_busy_r <= 1'b1;
    end
    else begin
        tx_busy_r <= 1'b0;
    end
end

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        fpga_state <= 1'b0;
    end
    else if(i_init)begin
        fpga_state <= 1'b1;
    end
    else if(i_stop)begin
        fpga_state <= 1'b0;
    end
    else begin
        fpga_state <= fpga_state ;
    end
end

assign o_read_en = read_en_r;
assign o_tx_busy = tx_busy_r;
assign o_tx      = tx_r;
assign o_tx_ctrl = tx_ctrl_r;

ila_rf_tx u_ila_rf_tx (
	.clk(clk), // input wire clk


	.probe0     (i_tx_data          ), // input wire [7:0]  probe0  
	.probe1     (i_tx_data_vld      ), // input wire [0:0]  probe1 
	.probe2     (i_fifo_empty       ), // input wire [0:0]  probe2 
	.probe3     (i_rf_ctrl_vld      ), // input wire [0:0]  probe3 
	.probe4     (i_init             ), // input wire [0:0]  probe4 
	.probe5     (i_stop             ), // input wire [0:0]  probe5 
	.probe6     (i_tx_ctrl_over     ), // input wire [0:0]  probe6 
	.probe7     (o_read_en          ), // input wire [0:0]  probe7 
	.probe8     (o_tx_busy          ), // input wire [0:0]  probe8 
	.probe9     (o_tx               ), // input wire [0:0]  probe9 
	.probe10    (o_tx_ctrl          ), // input wire [0:0]  probe10 
	.probe11    (state_c            ), // input wire [6:0]  probe11 
	.probe12    (cnt_baud           ), // input wire [11:0]  probe12 
	.probe13    (cnt_bit            ), // input wire [3:0]  probe13 
	.probe14    (cnt_byte           ), // input wire [3:0]  probe14 
	.probe15    (cnt_byte_times     ), // input wire [3:0]  probe15 
	.probe16    (cnt_wait           ), // input wire [19:0]  probe16 
	.probe17    (cnt_hold           ), // input wire [11:0]  probe17 
	.probe18    (tx_data_r          ) // input wire [7:0]  probe18
);

endmodule