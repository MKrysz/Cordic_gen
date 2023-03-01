module cordic(
    input signed [WD_EXT+1:0] i_angle, // in radians
    input i_clk,
    input i_rst,
    input i_start,
    output signed [WD_EXT+1:0] o_sin,
    output signed [WD_EXT+1:0] o_cos,
    output o_done
);

`include "cordic_params.v"

/* // TODO
    input/output format 
        1 integer bit - N fraction bits
*/

reg signed [WD_INT+1:0] x;
reg signed [WD_INT+1:0] y; 
reg signed [WD_INT+1:0] z; 
reg signed [WD_INT+1:0] angle;
// reg signed [N+1:0] sin;
// reg signed [N+1:0] cos;
reg done = 1'b0;


assign o_done = done;
assign o_sin = y[(WD_INT):(WD_INT-WD_EXT)];
assign o_cos = x[(WD_INT):(WD_INT-WD_EXT)];

reg [7:0] i = 0;    // GENERATED

reg [7:0] state = 8'b0;
localparam 
    s_rst = 8'b0,
    s_work = 8'b1;

wire [WD_INT+1:0] atan [WD_INT:0];
`include "cordic_atan_table.v"

always @(posedge i_clk) begin
    if(i_rst) begin
        x <= 0;
        y <= 0;
        done <= 0;
        i <= 0;
        state <= s_rst;
    end
    else begin
        case (state)
            s_rst: begin
                if(i_start) begin
                    angle <= i_angle <<< (WD_INT-WD_EXT);
                    state <= s_work;
                    i <= 0;
                    x <= START_VAL;
                    y <= 0;
                    z <= 0;
                    done <= 0;
                end
            end
            s_work: begin
                if (z<=angle) begin
                    z <= z + atan[i];
                    x <= x - (y>>>i);
                    y <= y + (x>>>i);
                end
                else begin
                    z <= z - atan[i];
                    x <= x + (y>>>i);
                    y <= y - (x>>>i);
                end
                i <= i+1;
                if(i == max_i) begin
                    done <= 1'b1;
                    state <= s_rst;
                end
            end
        endcase
    end
end

endmodule