module B(N1, N2, N3, N4, S1, S2 ,S3, S4);

input [7:0] N1;
input [7:0] N2;
input [7:0] N3;
input [7:0] N4;
output [7:0] S1;
output [7:0] S2;
output [7:0] S3;
output [7:0] S4;

wire [7:0] salida [11:0];

A a1(
.N1(N1),
.N2(N2),
.S1(salida[0]),
.S2(salida[1])
);

A a2(
.N1(N3),
.N2(N4),
.S1(salida[2]),
.S2(salida[3])
);

A a3(
.N1(salida[1]),
.N2(salida[2]),
.S1(salida[4]),
.S2(salida[5])
);

A a4(
.N1(salida[0]),
.N2(salida[3]),
.S1(salida[6]),
.S2(salida[7])
);

A a5(
.N1(salida[6]),
.N2(salida[4]),
.S1(S1),
.S2(S2)
);

A a6(
.N1(salida[5]),
.N2(salida[7]),
.S1(S3),
.S2(S4)
);



endmodule
 