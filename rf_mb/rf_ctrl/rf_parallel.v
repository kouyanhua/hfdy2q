module rf_parallel (
    input           clk             ,
    input           rst_n           ,

    input  [15:0]   i_rf_freq       ,
    input           i_rf_freq_vld   ,
    input           i_init          ,
    input           i_stop          ,
    input           i_pre_cpi       ,
    input           i_cpi           ,
    input           i_tx_over_flag  ,

    output [15:0]   o_rf_freq       ,
    output          o_rf_freq_ctrl  
);

reg  [15:0]         rf_freq_r,rf_freq_r1           ;
reg  [15:0]         o_rf_freq_r         ;
reg                 o_rf_freq_ctrl_r    ;

reg  [3:0]          pre_cpi_r           ;
wire                pedge_pre_cpi       ;

reg  [7:0]			cnt_ctrl			;
wire				add_cnt_ctrl		;
wire				end_cnt_ctrl		;
reg                 add_cnt_flagcc        ;

reg                 first_data_flag     ;

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        rf_freq_r <= 16'd0;
    end
    else if(i_rf_freq_vld)begin
        rf_freq_r <= i_rf_freq;
    end
    else begin
        rf_freq_r <= rf_freq_r ;
    end
end

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        rf_freq_r1 <= 16'd0;
    end
    else if(i_tx_over_flag)begin
        rf_freq_r1 <= rf_freq_r;
    end
    else begin
        rf_freq_r1 <= rf_freq_r1 ;
    end
end

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
        cnt_ctrl <= 0;
    end
    else if(add_cnt_ctrl)begin
        if(end_cnt_ctrl)begin
            cnt_ctrl <= 0;
        end
        else begin
            cnt_ctrl <= cnt_ctrl + 1;
        end
    end
    else begin
        cnt_ctrl <= cnt_ctrl ;
    end
end
assign add_cnt_ctrl = add_cnt_flag ;
assign end_cnt_ctrl = add_cnt_ctrl && cnt_ctrl == 200 - 1 ;

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        add_cnt_flag <= 1'b0;
    end
    else if(pedge_pre_cpi)begin
        add_cnt_flag <= 1'b1;
    end
    else if(end_cnt_ctrl)begin
        add_cnt_flag <= 1'b0;
    end
    else begin
        add_cnt_flag <= add_cnt_flag ;
    end
end

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        o_rf_freq_ctrl_r <= 1'b1;
    end
    else if(pedge_pre_cpi)begin
        o_rf_freq_ctrl_r <= 1'b0;
    end
    else if(end_cnt_ctrl)begin
        o_rf_freq_ctrl_r <= 1'b1;
    end
    else begin
        o_rf_freq_ctrl_r <= o_rf_freq_ctrl_r;
    end
end

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        first_data_flag <= 1'b0;
    end
    else if(i_rf_freq_vld)begin
        first_data_flag <= 1'b1;
    end
    else begin
        first_data_flag <= first_data_flag ;
    end
end

always@(posedge clk or negedge rst_n) begin
    if(!rst_n)begin
        o_rf_freq_r <= 16'd0;
    end
    else if(first_data_flag == 1'b0)begin
        o_rf_freq_r <= rf_freq_r;
    end
    else if(i_tx_over_flag)begin
        o_rf_freq_r <= rf_freq_r;
    end
    else begin
        o_rf_freq_r <= o_rf_freq_r ;
    end
end

assign o_rf_freq = rf_freq_r;
assign o_rf_freq_ctrl = add_cnt_flag;

ila_parallel u_ila_parallel (
	.clk(clk), // input wire clk


	.probe0     (i_rf_freq      ), // input wire [9:0]  probe0  
	.probe1     (i_rf_freq_vld  ), // input wire [0:0]  probe1 
	.probe2     (i_init         ), // input wire [0:0]  probe2 
	.probe3     (i_stop         ), // input wire [0:0]  probe3 
	.probe4     (i_pre_cpi      ), // input wire [0:0]  probe4 
	.probe5     (i_cpi          ), // input wire [0:0]  probe5 
	.probe6     (o_rf_freq      ), // input wire [9:0]  probe6 
	.probe7     (o_rf_freq_ctrl ), // input wire [0:0]  probe7 
	.probe8     (rf_freq_r      ), // input wire [9:0]  probe8 
	.probe9     (o_rf_freq_r    ), // input wire [9:0]  probe9 
	.probe10    (cnt_ctrl       ) // input wire [7:0]  probe10
);
    
endmodule