module conversor(N,S,S1,S2,S3);
input [7:0] N;
output [9:0] S;
output [6:0] S1;
output [6:0] S2;
output [6:0] S3;


wire [27:0] res;
wire [3:0] U;
wire [3:0] T;
wire [3:0] H;

BCD2HEX bcd1( //BIEN
.A0(N[5]),
.A1(N[6]),
.A2(N[7]),
.A3(0),
.S0(res[0]),
.S1(res[1]),
.S2(res[2]),
.S3(res[3])
);

BCD2HEX bcd2( //BIEN
.A0(N[4]),
.A1(res[0]),
.A2(res[1]),
.A3(res[2]),
.S0(res[4]),
.S1(res[5]),
.S2(res[6]),
.S3(res[7])
);

BCD2HEX bcd3( //BIEN
.A0(N[3]),
.A1(res[4]),
.A2(res[5]),
.A3(res[6]),
.S0(res[8]),
.S1(res[9]),
.S2(res[10]),
.S3(res[11])
);

BCD2HEX bcd4( //BIEN
.A0(N[2]),
.A1(res[8]),
.A2(res[9]),
.A3(res[10]),
.S0(res[12]),
.S1(res[13]),
.S2(res[14]),
.S3(res[15])
);

BCD2HEX bcd5( //BIEN
.A0(N[1]),
.A1(res[12]),
.A2(res[13]),
.A3(res[14]),
.S0(res[20]),
.S1(res[21]),
.S2(res[22]),
.S3(res[23])
);

BCD2HEX bcd6( //BIEN
.A0(res[11]),
.A1(res[7]),
.A2(res[3]),
.A3(0),
.S0(res[16]),
.S1(res[17]),
.S2(res[18]),
.S3(res[19])
);

BCD2HEX bcd7( //BIEN
.A0(res[15]),
.A1(res[16]),
.A2(res[17]),
.A3(res[18]),
.S0(res[24]),
.S1(res[25]),
.S2(res[26]),
.S3(res[27])
);

assign S[0] = N[0];
assign S[1] = res[20];
assign S[2] = res[21];
assign S[3] = res[22];
assign S[4] = res[23];
assign S[5] = res[24];
assign S[6] = res[25];
assign S[7] = res[26];
assign S[8] = res[27];
assign S[9] = res[19];

assign U[0] = S[0];
assign U[1] = S[1];
assign U[2] = S[2];
assign U[3] = S[3];

assign T[0] = S[4];
assign T[1] = S[5];
assign T[2] = S[6];
assign T[3] = S[7];

assign H[0] = S[8];
assign H[1] = S[9];
assign H[2] = 0;
assign H[3] = 0;

segment7 segment1(
	.bcd(U),
	.seg(S1)
);


segment7 segment2(
	.bcd(T),
	.seg(S2)
);

segment7 segment3(
	.bcd(H),
	.seg(S3)
);

endmodule