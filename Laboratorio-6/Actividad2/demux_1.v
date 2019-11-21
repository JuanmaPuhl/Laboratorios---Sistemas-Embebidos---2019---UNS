module demux_1(
	Data_in,
	sel,
	Data_out_0,
	Data_out_1,
	Data_out_2,
	Data_out_3,
	Data_out_4,
	Data_out_5,
	Data_out_6,
	Data_out_7
);
input Data_in;
input [2:0] sel;
output Data_out_0, Data_out_1, Data_out_2, Data_out_3, 
					Data_out_4, Data_out_5, Data_out_6, Data_out_7;

reg Data_out_0, Data_out_1, Data_out_2, Data_out_3, 
				 Data_out_4, Data_out_5, Data_out_6, Data_out_7;				 
				 
				 
always @(Data_in or sel)
begin
	case(sel)
		'b000: begin
						Data_out_0 = Data_in;
						Data_out_1 = 0;
						Data_out_2 = 0;
						Data_out_3 = 0;
						Data_out_4 = 0;
						Data_out_5 = 0;
						Data_out_6 = 0;
						Data_out_7 = 0;
					end
		'b001: begin
						Data_out_0 = 0;
						Data_out_1 = Data_in;
						Data_out_2 = 0;
						Data_out_3 = 0;
						Data_out_4 = 0;
						Data_out_5 = 0;
						Data_out_6 = 0;
						Data_out_7 = 0;
					end
		'b010: begin
						Data_out_0 = 0;
						Data_out_1 = 0;
						Data_out_2 = Data_in;
						Data_out_3 = 0;
						Data_out_4 = 0;
						Data_out_5 = 0;
						Data_out_6 = 0;
						Data_out_7 = 0;
					end
		'b011: begin
						Data_out_0 = 0;
						Data_out_1 = 0;
						Data_out_2 = 0;
						Data_out_3 = Data_in;
						Data_out_4 = 0;
						Data_out_5 = 0;
						Data_out_6 = 0;
						Data_out_7 = 0;
					end
		'b100: begin
						Data_out_0 = 0;
						Data_out_1 = 0;
						Data_out_2 = 0;
						Data_out_3 = 0;
						Data_out_4 = Data_in;
						Data_out_5 = 0;
						Data_out_6 = 0;
						Data_out_7 = 0;
					end
		'b101: begin
						Data_out_0 = 0;
						Data_out_1 = 0;
						Data_out_2 = 0;
						Data_out_3 = 0;
						Data_out_4 = 0;
						Data_out_5 = Data_in;
						Data_out_6 = 0;
						Data_out_7 = 0;
					end
		'b110: begin
						Data_out_0 = 0;
						Data_out_1 = 0;
						Data_out_2 = 0;
						Data_out_3 = 0;
						Data_out_4 = 0;
						Data_out_5 = 0;
						Data_out_6 = Data_in;
						Data_out_7 = 0;
					end
		'b111: begin
						Data_out_0 = 0;
						Data_out_1 = 0;
						Data_out_2 = 0;
						Data_out_3 = 0;
						Data_out_4 = 0;
						Data_out_5 = 0;
						Data_out_6 = 0;
						Data_out_7 = Data_in;
					end
	 endcase
end				 
				 
endmodule