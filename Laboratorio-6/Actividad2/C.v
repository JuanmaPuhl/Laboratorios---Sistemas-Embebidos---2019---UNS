module C(N1,N2,N3,N4,N5,N6,N7,N8,S1,S2,S3,S4,S5,S6,S7,S8);

input [7:0] N1;
input [7:0] N2;
input [7:0] N3;
input [7:0] N4;
input [7:0] N5;
input [7:0] N6;
input [7:0] N7;
input [7:0] N8;

output [7:0] S1;
output [7:0] S2;
output [7:0] S3;
output [7:0] S4;
output [7:0] S5;
output [7:0] S6;
output [7:0] S7;
output [7:0] S8;

wire [7:0] salida [23:0];

B b1(
.N1(N1),
.N2(N2),
.N3(N3),
.N4(N4),
.S1(salida[0]),
.S2(salida[1]),
.S3(salida[2]),
.S4(salida[3])
);

B b2(
.N1(N5),
.N2(N6),
.N3(N7),
.N4(N8),
.S1(salida[4]),
.S2(salida[5]),
.S3(salida[6]),
.S4(salida[7])
);

A a1(
.N1(salida[0]),
.N2(salida[7]),
.S1(salida[8]),
.S2(salida[9])
);

A a2(
.N1(salida[1]),
.N2(salida[6]),
.S1(salida[10]),
.S2(salida[11])
);

A a3(
.N1(salida[2]),
.N2(salida[5]),
.S1(salida[12]),
.S2(salida[13])
);

A a4(
.N1(salida[3]),
.N2(salida[4]),
.S1(salida[14]),
.S2(salida[15])
);



D d2(
.N1(salida[15]),
.N2(salida[13]),
.N3(salida[11]),
.N4(salida[9]),
.S1(S5),
.S2(S6),
.S3(S7),
.S4(S8)
);

D d1(
.N1(salida[8]),
.N2(salida[10]),
.N3(salida[12]),
.N4(salida[14]),
.S1(S1),
.S2(S2),
.S3(S3),
.S4(S4)
);

endmodule