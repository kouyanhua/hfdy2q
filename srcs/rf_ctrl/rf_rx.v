`timescale 1ns/1ps
/* ///////////////////////////////////////////////////////////////////////////// */
//Org         : raco
//Created by  : kou
//Time        : 2024-03-01
//Prj Name    :   
//Module Name : rf_rx
//Target Dev  : xcvu9p-flgb2104-2-i
//Tool ver    : vivado2018.3

//Description : This module receive information from uart;
//Modification Record
// |  author  | time        |      contents  
// ----------------------------------------------------------------------------               
// |  kou     | 2023-03-01  | 1.create the file              
// |  kou     | 2023-03-01  | 1.add receive module logic
// |  xxx     | 2023-xx-xx  |       
module rf_rx (
    input           clk                 ,
    input           rst_n               ,
    input           i_rx                ,
    output [7:0]    o_rx_data           ,
    output          o_rx_data_vld       ,

    output [7:0]    o_rx_cnt            ,
    output [7:0]    o_err_cnt           
);

parameter           IDLE = 4'b0001          ,
                    START= 4'b0010          ,
                    DATA = 4'b0100          ,
                    STOP = 4'b1000          ;

parameter           BAUD        = 1736      ,//系统时钟200m，波特率115200，都是定死的，所以不需要外部输入控制
                    HALF_BAUD   = 868       ;

reg  [3:0]          state_c         ;//状态机现态
reg  [3:0]          state_n         ;//状态机次态

wire                idle2start      ;//状态转移条件
wire                start2data      ;
wire                data2stop       ;
wire                stop2idle       ;
wire                start2idle      ;

reg                 rx_d1           ;//打两拍，防止输入亚稳态
reg                 rx_d2           ;
reg                 rx_d3           ;//对输入打拍，用以检测下降沿
reg                 rx_d4           ;
reg                 start_flag      ;//下降沿信号，高电平有效

reg  [10:0]			cnt_bit		        ;
wire				add_cnt_bit		    ;
wire				end_cnt_bit		    ;

reg  [2:0]			cnt_byte			;
wire				add_cnt_byte		;
wire				end_cnt_byte		;

reg  [7:0]          rx_data_d           ;
reg                 rx_data_vld_d       ;

reg                 stop_data           ;//为计数错误设立
reg                 start_data          ;

reg  [7:0]          rx_cnt_r            ;
reg  [7:0]          err_cnt_r           ;


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
        IDLE :begin
            if(idle2start) begin
                state_n = START;
            end
            else begin
                state_n = IDLE;
            end
        end
        START:begin
            if(start2data) begin
                state_n = DATA;
            end
            else if(start2idle) begin
                state_n = IDLE;
            end
            else begin
                state_n = START;
            end
        end
        DATA :begin
            if(data2stop) begin
                state_n = STOP;
            end
            else begin
                state_n = DATA;
            end
        end
        STOP :begin
            if(stop2idle) begin
                state_n = IDLE;
            end
            else begin
                state_n = STOP;
            end
        end
        default : state_n = IDLE;
    endcase
end

assign idle2start = (state_c == IDLE ) && (start_flag);
assign start2data = (state_c == START) && (end_cnt_bit) && (start_data == 0);
assign data2stop  = (state_c == DATA ) && (end_cnt_byte);
assign stop2idle  = (state_c == STOP ) && (end_cnt_bit);
assign start2idle = (state_c == START) && (end_cnt_bit) && (start_data == 1);

always@(posedge clk or negedge rst_n) begin//打拍，下降沿检测
    if(!rst_n)begin
        rx_d1 <= 1'b1;
        rx_d2 <= 1'b1;
        rx_d3 <= 1'b1;
        rx_d4 <= 1'b1;
    end
    else begin
        rx_d1 <= i_rx ;
        rx_d2 <= rx_d1;
        rx_d3 <= rx_d2;
        rx_d4 <= rx_d3;
    end
end

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        start_flag <= 1'b0;
    end
    else begin
        start_flag <= (!rx_d3) && rx_d4;
    end
end

always@(posedge clk or negedge rst_n) begin//波特率计数
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
        cnt_bit <= cnt_bit;
    end
end
assign add_cnt_bit = state_c != IDLE ;
assign end_cnt_bit = add_cnt_bit && cnt_bit == (state_c == STOP?HALF_BAUD:BAUD) - 1 ;

always@(posedge clk or negedge rst_n) begin//有效数据字节计数
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
        cnt_byte <= cnt_byte;
    end
end
assign add_cnt_byte = end_cnt_bit && (state_c == DATA) ;
assign end_cnt_byte = add_cnt_byte && (&cnt_byte);

always@(posedge clk or negedge rst_n) begin//数据输出
    if(!rst_n)begin
        rx_data_d <= 8'd0;
    end
    else if((state_c == DATA) && (cnt_bit == HALF_BAUD))begin
        rx_data_d <= {rx_d2,rx_data_d[7:1]};
    end
    else begin
        rx_data_d <= rx_data_d;
    end
end

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        stop_data <= 1'b1;
    end
    else if((state_c == STOP) && (cnt_bit == HALF_BAUD))begin
        stop_data <= rx_d2;
    end
    else begin
        stop_data <= stop_data;
    end
end

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        start_data <= 1'b0;
    end
    else if((state_c == START) && (cnt_bit == HALF_BAUD))begin
        start_data <= rx_d2;
    end
    else begin
        start_data <= start_data;
    end
end

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        rx_cnt_r <= 8'd0;
    end
    else if(stop2idle && (stop_data))begin
        rx_cnt_r <= rx_cnt_r +1;
    end
    else begin
        rx_cnt_r <= rx_cnt_r;
    end
end

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        err_cnt_r <= 8'd0;
    end
    else if((start2idle)|(stop2idle && (!stop_data)))begin
        err_cnt_r <= err_cnt_r +1;
    end
    else begin
        err_cnt_r <= err_cnt_r;
    end
end

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        rx_data_vld_d <= 1'b0;
    end
    else if(stop2idle)begin
        rx_data_vld_d <= 1'b1;
    end
    else begin
        rx_data_vld_d <= 1'b0;
    end
end

assign o_rx_data = rx_data_d;
assign o_rx_data_vld = rx_data_vld_d;
assign o_rx_cnt = rx_cnt_r;
assign o_err_cnt = err_cnt_r;

endmodule