module demux(
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
input [7:0]Data_in;
input [2:0] sel;
output [7:0] Data_out_0, Data_out_1, Data_out_2, Data_out_3, 
					Data_out_4, Data_out_5, Data_out_6, Data_out_7;

reg [7:0] Data_out_0, Data_out_1, Data_out_2, Data_out_3, 
				 Data_out_4, Data_out_5, Data_out_6, Data_out_7;				 
				 
				 
always @(Data_in or sel)
begin
	case(sel)
		3'b000: begin
						Data_out_0 = Data_in;
						Data_out_1 = 3'b000;
						Data_out_2 = 3'b000;
						Data_out_3 = 3'b000;
						Data_out_4 = 3'b000;
						Data_out_5 = 3'b000;
						Data_out_6 = 3'b000;
						Data_out_7 = 3'b000;
					end
		3'b001: begin
						Data_out_0 = 3'b000;
						Data_out_1 = Data_in;
						Data_out_2 = 3'b000;
						Data_out_3 = 3'b000;
						Data_out_4 = 3'b000;
						Data_out_5 = 3'b000;
						Data_out_6 = 3'b000;
						Data_out_7 = 3'b000;
					end
		3'b010: begin
						Data_out_0 = 3'b000;
						Data_out_1 = 3'b000;
						Data_out_2 = Data_in;
						Data_out_3 = 3'b000;
						Data_out_4 = 3'b000;
						Data_out_5 = 3'b000;
						Data_out_6 = 3'b000;
						Data_out_7 = 3'b000;
					end
		3'b011: begin
						Data_out_0 = 3'b000;
						Data_out_1 = 3'b000;
						Data_out_2 = 3'b000;
						Data_out_3 = Data_in;
						Data_out_4 = 3'b000;
						Data_out_5 = 3'b000;
						Data_out_6 = 3'b000;
						Data_out_7 = 3'b000;
					end
		3'b100: begin
						Data_out_0 = 3'b000;
						Data_out_1 = 3'b000;
						Data_out_2 = 3'b000;
						Data_out_3 = 3'b000;
						Data_out_4 = Data_in;
						Data_out_5 = 3'b000;
						Data_out_6 = 3'b000;
						Data_out_7 = 3'b000;
					end
		3'b101: begin
						Data_out_0 = 3'b000;
						Data_out_1 = 3'b000;
						Data_out_2 = 3'b000;
						Data_out_3 = 3'b000;
						Data_out_4 = 3'b000;
						Data_out_5 = Data_in;
						Data_out_6 = 3'b000;
						Data_out_7 = 3'b000;
					end
		3'b110: begin
						Data_out_0 = 3'b000;
						Data_out_1 = 3'b000;
						Data_out_2 = 3'b000;
						Data_out_3 = 3'b000;
						Data_out_4 = 3'b000;
						Data_out_5 = 3'b000;
						Data_out_6 = Data_in;
						Data_out_7 = 3'b000;
					end
		3'b111: begin
						Data_out_0 = 3'b000;
						Data_out_1 = 3'b000;
						Data_out_2 = 3'b000;
						Data_out_3 = 3'b000;
						Data_out_4 = 3'b000;
						Data_out_5 = 3'b000;
						Data_out_6 = 3'b000;
						Data_out_7 = Data_in;
					end
	 endcase
end				 
				 
endmodule