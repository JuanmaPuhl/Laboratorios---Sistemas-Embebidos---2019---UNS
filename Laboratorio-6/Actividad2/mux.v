module mux(N1,N2,N3,N4,N5,N6,N7,N8, sel, S);
input [7:0] N1;
input [7:0] N2;
input [7:0] N3;
input [7:0] N4;
input [7:0] N5;
input [7:0] N6;
input [7:0] N7;
input [7:0] N8;
input [2:0] sel;
output [7:0] S;


reg [7:0] S;

always @ (N1 or N2 or N3 or N4 or N5 or N6 or N7 or N8 or sel)
begin
	case (sel)
		3'b000 : S <= N1;
		3'b001 : S <= N2;
		3'b010 : S <= N3;
		3'b011 : S <= N4;
		3'b100 : S <= N5;
		3'b101 : S <= N6;
		3'b110 : S <= N7;
		3'b111 : S <= N8;
	endcase
end


endmodule