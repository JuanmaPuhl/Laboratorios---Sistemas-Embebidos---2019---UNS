module counter(sw,cin);

input  sw;
output [2:0] cin;

reg [2:0] counter_up; 

always @(posedge sw)
begin
	if(counter_up < 3'b111)
		counter_up <= counter_up + 3'b001;
	else
		counter_up <= 3'b000;
end

assign cin = counter_up;

endmodule