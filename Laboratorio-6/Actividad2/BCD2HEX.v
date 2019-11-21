module BCD2HEX(A0,A1,A2,A3,S0,S1,S2,S3);
input A0,A1,A2,A3;
output S0,S1,S2,S3;

assign S3 = (A2 & A1) | (A2 & A0) | (A3);
assign S2 = (A2 & ~A1 & ~A0) | (A3 & A0);
assign S1 = (A3 & ~A0) | (~A2 & A1) | (A1 & A0);
assign S0 = (~A3 & ~A2 & A0) | (A2 & A1 & ~A0) | (A3 & ~A0);


//A0 = A3;
//A1 = A2;
//A2 = A1;
//A3 = A0;
endmodule