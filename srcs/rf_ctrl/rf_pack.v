`timescale 1ns/1ps
/* ///////////////////////////////////////////////////////////////////////////// */
//Org         : raco
//Created by  : kou
//Time        : 2024-03-01
//Prj Name    :   
//Module Name : rf_pack
//Target Dev  : xcvu9p-flgb2104-2-i
//Tool ver    : vivado2018.3

//Description : This module creates two packs:rf_ask,rf_ctrl;
//Modification Record
// |  author  | time        |      contents  
// ----------------------------------------------------------------------------               
// |  kou     | 2023-03-01  | 1.create the file              
// |  kou     | 2023-03-01  | 1.add create module logic
// |  xxx     | 2023-xx-xx  |       
module rf_pack (
    input           clk                 ,
    input           rst_n               ,

    input           i_rf_ask_flag       ,
    input           i_rf_ctrl_vld       ,
    input   [31:0]  i_rfc_agin          ,
    input   [15:0]  i_rf_freq           ,
    input   [7:0]   i_chk_mode          ,

    input           i_init              ,
    input           i_stop              ,

    output  [7:0]   o_rf_data           ,
    output          o_rf_vld            ,
    output  [31:0]  o_rf_send_num       
);

parameter               M_IDLE = 3'b001,
                        M_INIT = 3'b010,
                        M_STOP = 3'b100;

parameter               S_IDLE = 3'b001,
                        S_DATA = 3'b010,
                        S_CHK  = 3'b100;

wire                m_idle2m_init           ;
wire                m_init2m_stop           ;

wire                s_idle2s_data           ;
wire                s_data2s_chk            ;
wire                s_chk2s_idle            ;

reg  [2:0]          m_state_c               ;
reg  [2:0]          m_state_n               ;
reg  [2:0]          s_state_c               ;
reg  [2:0]          s_state_n               ;

reg                 s_idle2s_data_flag      ;

reg  [3:0]			cnt_data			    ;
wire				add_cnt_data		    ;
wire				end_cnt_data		    ;
reg  [3:0]          cnt_data_times          ;

reg  [31:0]         rfc_agin_r              ;
reg  [15:0]         rf_freq_r               ;
reg  [7:0]          chk_mode_r              ;

reg  [3:0]          rf_ask_flag_r           ;
wire                pedge_ask_flag          ;
reg  [3:0]          rf_ctrl_vld_r           ;
wire                pedge_ctrl_vld          ;

reg  [7:0]          chk_r                   ;

reg  [7:0]          o_rf_data_r             ;
reg                 o_rf_vld_r              ;

reg  [15:0]         rf_ask_num              ;
reg  [15:0]         rf_ctrl_num             ;

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        m_state_c <= M_IDLE;
    end
    else begin
        m_state_c <= m_state_n;
    end
end
always@(*) begin//组合逻辑
    case (m_state_c)
        M_IDLE :begin
            if(m_idle2m_init) begin
                m_state_n = M_INIT;
            end
            else begin
                m_state_n = M_IDLE;
            end
        end
        M_INIT :begin
            if(m_init2m_stop) begin
                m_state_n = M_STOP;
            end
            else begin
                m_state_n = M_INIT;
            end
        end
        M_STOP :begin
            m_state_n = M_STOP;
        end
        default : m_state_n = M_IDLE;
    endcase
end

assign m_idle2m_init = (m_state_c == M_IDLE) && (i_init);
assign m_init2m_stop = (m_state_c == M_INIT) && (i_stop);

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        s_state_c <= S_IDLE;
    end
    else begin
        s_state_c <= s_state_n;
    end
end
always@(*) begin//组合逻辑
    case (s_state_c)
        S_IDLE : begin
            if(s_idle2s_data) begin
                s_state_n = S_DATA;
            end
            else begin
                s_state_n = S_IDLE;
            end
        end
        S_DATA : begin
            if(s_data2s_chk) begin
                s_state_n = S_CHK;
            end
            else begin
                s_state_n = S_DATA;
            end
        end
        S_CHK  : begin
            if(s_chk2s_idle) begin
                s_state_n = S_IDLE;
            end
            else begin
                s_state_n = S_CHK;
            end
        end
        default : s_state_n = S_IDLE;
    endcase
end

assign s_idle2s_data = (s_state_c == S_IDLE) && (s_idle2s_data_flag);
assign s_data2s_chk  = (s_state_c == S_DATA) && (end_cnt_data);
assign s_chk2s_idle  = (s_state_c == S_CHK) && (1'b1);

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        rf_ask_flag_r <= 4'd0;
    end
    else begin
        rf_ask_flag_r[0] <= i_rf_ask_flag;
        rf_ask_flag_r[1] <= rf_ask_flag_r[0];
        rf_ask_flag_r[2] <= rf_ask_flag_r[1];
        rf_ask_flag_r[3] <= rf_ask_flag_r[2];
    end
end
assign pedge_ask_flag = (!rf_ask_flag_r[3])&&(rf_ask_flag_r[2]);

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        rf_ctrl_vld_r <= 4'd0;
    end
    else begin
        rf_ctrl_vld_r[0] <= i_rf_ctrl_vld;
        rf_ctrl_vld_r[1] <= rf_ctrl_vld_r[0];
        rf_ctrl_vld_r[2] <= rf_ctrl_vld_r[1];
        rf_ctrl_vld_r[3] <= rf_ctrl_vld_r[2];
    end
end
assign pedge_ctrl_vld = (!rf_ctrl_vld_r[3])&&(rf_ctrl_vld_r[2]);

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        s_idle2s_data_flag <= 1'b0;
    end
    else if(((m_state_c == M_IDLE)||(m_state_c == M_STOP))&&(pedge_ask_flag))begin
        s_idle2s_data_flag <= 1'b1;
    end
    else if((m_state_c == M_INIT)&&(pedge_ctrl_vld))begin
        s_idle2s_data_flag <= 1'b1;
    end
    else begin
        s_idle2s_data_flag <= 1'b0;
    end
end

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
assign add_cnt_data = (s_state_c == S_DATA) ;
assign end_cnt_data = add_cnt_data && cnt_data == cnt_data_times - 1 ;

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        cnt_data_times <= 4'd3;
    end
    else if(m_state_c == M_INIT)begin
        cnt_data_times <= 4'd8;
    end
    else begin
        cnt_data_times <= 4'd3;
    end
end

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        rfc_agin_r<= 32'd0;
        rf_freq_r <= 16'd0;
    end
    else if(i_rf_ctrl_vld)begin
        rfc_agin_r<= i_rfc_agin;
        rf_freq_r <= i_rf_freq ;
    end
    else begin
        rfc_agin_r<= rfc_agin_r;
        rf_freq_r <= rf_freq_r ;
    end
end

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        chk_r <= 8'd0;
    end
    else if((m_state_c == M_INIT)&&(s_state_c == S_DATA))begin
        case (cnt_data)
            0 : chk_r <= chk_r ^ 8'hff;
            1 : chk_r <= chk_r ^ rf_freq_r[15:8]  ;
            2 : chk_r <= chk_r ^ rf_freq_r[7:0]   ;
            3 : chk_r <= chk_r ^ rfc_agin_r[31:24];
            4 : chk_r <= chk_r ^ rfc_agin_r[23:16];
            5 : chk_r <= chk_r ^ rfc_agin_r[15:8] ;
            6 : chk_r <= chk_r ^ rfc_agin_r[7:0]  ;
            7 : chk_r <= chk_r ^ 8'h02       ;
            default: chk_r <= chk_r;
        endcase
    end
    else if(s_idle2s_data)begin
        chk_r <= 8'd0;
    end
    else begin
        chk_r <= chk_r;
    end
end

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        o_rf_data_r <= 8'd0;
    end
    else begin
        case (m_state_c)
            M_IDLE : begin
                if(s_state_c == S_DATA)begin
                    case (cnt_data)
                        0 : o_rf_data_r <= 8'hfa;
                        1 : o_rf_data_r <= 8'h01;
                        2 : o_rf_data_r <= 8'h00;
                        default: o_rf_data_r <= 8'd0;
                    endcase
                end
                else if(s_state_c == S_CHK)begin
                    o_rf_data_r <= 8'hfb;
                end
                else begin
                    o_rf_data_r <= 8'd0;
                end
            end
            M_INIT : begin
                if(s_state_c == S_DATA)begin
                    case (cnt_data)
                        0 : o_rf_data_r <= 8'hff            ;
                        1 : o_rf_data_r <= rf_freq_r[15:8]  ;
                        2 : o_rf_data_r <= rf_freq_r[7:0]   ;
                        3 : o_rf_data_r <= rfc_agin_r[31:24];
                        4 : o_rf_data_r <= rfc_agin_r[23:16];
                        5 : o_rf_data_r <= rfc_agin_r[15:8] ;
                        6 : o_rf_data_r <= rfc_agin_r[7:0]  ;
                        7 : o_rf_data_r <= 8'h02       ;
                        default: o_rf_data_r <= 8'd0;
                    endcase
                end
                else if(s_state_c == S_CHK)begin
                    o_rf_data_r <= chk_r;
                end
                else begin
                    o_rf_data_r <= 8'd0;
                end
            end
            M_STOP : begin
                if(s_state_c == S_DATA)begin
                    case (cnt_data)
                        0 : o_rf_data_r <= 8'hff;
                        1 : o_rf_data_r <= 8'h01;
                        2 : o_rf_data_r <= 8'h00; 
                        default: o_rf_data_r <= 8'd0;
                    endcase
                end
                else if(s_state_c == S_CHK)begin
                    o_rf_data_r <= 8'hfe;
                end
                else begin
                    o_rf_data_r <= 8'd0;
                end
            end
            default: o_rf_data_r <= 8'd0;
        endcase
    end
end

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        o_rf_vld_r <= 1'b0;
    end
    else if(s_state_c != S_IDLE)begin
        o_rf_vld_r <= 1'b1;
    end
    else begin
        o_rf_vld_r <= 1'b0;
    end
end

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        rf_ask_num <= 16'd0;
    end
    else if(((m_state_c == M_IDLE)||(m_state_c == M_STOP))&&s_chk2s_idle)begin
        rf_ask_num <= rf_ask_num +1;
    end
    else begin
        rf_ask_num <= rf_ask_num ;
    end
end

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        rf_ctrl_num <= 16'd0;
    end
    else if((m_state_c == M_INIT)&&s_chk2s_idle)begin
        rf_ctrl_num <= rf_ctrl_num +1;
    end
    else begin
        rf_ctrl_num <= rf_ctrl_num ;
    end
end

assign o_rf_data      = o_rf_data_r;
assign o_rf_vld       = o_rf_vld_r;
assign o_rf_send_num  = {rf_ask_num,rf_ctrl_num};

ila_rf_pack u_ila_rf_pack (
	.clk(clk), // input wire clk


	.probe0     (i_rf_ask_flag  ), // input wire [0:0]  probe0  
	.probe1     (i_rf_ctrl_vld  ), // input wire [0:0]  probe1 
	.probe2     (i_rfc_agin     ), // input wire [15:0]  probe2 
	.probe3     (i_rf_freq      ), // input wire [15:0]  probe3 
	.probe4     (i_init         ), // input wire [0:0]  probe4 
	.probe5     (i_stop         ), // input wire [0:0]  probe5 
	.probe6     (o_rf_data      ), // input wire [7:0]  probe6 
	.probe7     (o_rf_vld       ), // input wire [0:0]  probe7 
	.probe8     (o_rf_send_num  ), // input wire [31:0]  probe8 
	.probe9     (m_state_c      ), // input wire [2:0]  probe9 
	.probe10    (s_state_c      ), // input wire [2:0]  probe10 
	.probe11    (cnt_data       ), // input wire [2:0]  probe11 
	.probe12    (chk_r          ) // input wire [7:0]  probe12
);

endmodule