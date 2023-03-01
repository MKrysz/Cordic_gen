`timescale 1ns/1ns
`include "cordic.v"

module test;

reg clk = 0;
reg rst = 1;
reg start = 0;
reg [7:0] angle = 0;
wire done;
wire [7:0] sin;
wire [7:0] cos;

cordic uut (
    .i_angle(angle),
    .i_clk(clk),
    .i_rst(rst),
    .i_start(start),
    .o_sin(sin),
    .o_cos(cos),
    .o_done(done)
);

integer file;
reg [7:0] angle_for;

initial begin
    #10;
    rst <= 0;

    // $dumpfile("cordic_tb.vcd");
    // $dumpvars;
    // angle <= 8'd3;
    // start <= 1;
    // #2;
    // start <= 0;
    // wait(done == 1'b1);

    file = $fopen("../out.csv");
    $fdisplay(file, "angle,sin,cos");
    $display("Start");
    for (angle_for = 0; angle_for <= 8'h64; angle_for = angle_for + 1) begin
        angle <= angle_for;
        rst <= 0;
        start <= 1;
        #2;
        start <= 0;
        wait(done == 1'b1);
        #2;
        $fdisplay(file, "%0h,%0h,%0h", angle, sin, cos);
    end
    $fclose(file);

    $finish;
end

always begin
    #1 clk <= ~clk;
end

endmodule