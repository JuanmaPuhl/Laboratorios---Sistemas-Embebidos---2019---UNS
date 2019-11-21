module A(N1 , N2, S1, S2);

input [7:0] N1;
input [7:0 ]N2;
output [7:0] S1;
output [7:0] S2;

reg [7:0] S1, S2; 

always @(N1 or N2)
begin
	if(N1 < N2)
		begin
			S1 = N1;
			S2 = N2;
		end
	else
		begin
			S1 = N2;
			S2 = N1;
		end
end

endmodule
