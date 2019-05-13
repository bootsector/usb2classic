/*
 * USB2Classic - USB controllers adapter for retro consoles!
 * Copyright (c) 2019 Bruno Freitas - bruno@brunofreitas.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
`timescale 1ns / 100ps // time-unit = 1 ns, precision = 100 ps
module smdsixbutton_tb;
    // Inputs
    reg p7; // Clock from Sega Mega Drive
    reg up; // Up button
    reg dw; // Down button
    reg lf; // Left button
    reg rg; // Right button
    reg a;  // A button
    reg b;  // B button
    reg c;  // C button
    reg st; // Start button
    reg x;  // X button
    reg y;  // Y button
    reg z;  // Z button
    reg md; // Mode button

    reg clk = 1'd0;

    integer j, k;

    // Outputs
    wire [5:0] p;

    // Instantiate the Unit Under Test (UUT)
    smdsixbutton joystick (
        .p7(p7), 
        .up(up), 
        .dw(dw),
        .lf(lf),
        .rg(rg),
        .a(a),
        .b(b),
        .c(c),
        .st(st),
        .x(x),
        .y(y),
        .z(z),
        .md(md),
        .clk(clk),

        .p(p)
    );

    always #50 clk <= !clk; // 20Mhz
    //always #100 clk <= !clk; // 10Mhz

    initial begin
        $timeformat(-3, 2, " ms", 10); 
        $dumpfile("smdsixbutton.vcd");
        $dumpvars(0, joystick);

        // Initialize Inputs
        up = 0;
        dw = 1;
        lf = 1;
        rg = 1;
        a = 1;
        b = 1;
        c = 1;
        st = 1;
        x = 1;
        y = 0;
        z = 1;
        md = 1;

        p7 = 1;

        #1660000 $display("delay");

        for(j = 0; j < 20; j=j+1) begin
            #13000 p7 = 0;
            #13000 p7 = 1;
            #13000 p7 = 0;
            #13000 p7 = 1;
            #13000 p7 = 0;
            #13000 p7 = 1;
            #13000 p7 = 0;
            #13000 p7 = 1;
            #1660000 $display("delay");
        end

        $finish();
    end  

    initial begin
        $monitor("p7=%d,p1=%d,p2=%d,p3=%d,p4=%d,p6=%d,p9=%d,time=%t \n",p7,p[5],p[4],p[3],p[2],p[1],p[0],$time);
    end

endmodule