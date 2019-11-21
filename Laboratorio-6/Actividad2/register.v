module register(clk, in, out);
input clk;
input [7:0] in;
output [7:0] out;

reg [7:0] out;
always @ (posedge(clk))
begin
	out <= in;
end

endmodule