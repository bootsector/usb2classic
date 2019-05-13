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
module shiftin_tb;
    // Inputs
    reg clk;
    reg latch;
    reg [15:0] i;
    reg system_clock = 1'd0;

    // Outputs
    wire data;

    integer ii;

    // Instantiate the Unit Under Test (UUT)
    shiftin uut (
        .clk(clk),
        .system_clock(system_clock), 
        .latch(latch), 
        .data(data),
        .i(i)
    );

    always #50 system_clock <= !system_clock; // 20Mhz

    initial begin
        $timeformat(-3, 2, " ms", 10); 
        $dumpfile("snes.vcd");
        $dumpvars(1, shiftin_tb);
        // Initialize Inputs
        clk = 0;
        latch = 0;
        i = 16'b1010101010101010;

        #5000 latch = 1;
        #5000 latch = 0;

        for (ii = 0; ii < 16; ii = ii + 1) begin
            //$strobe("ii=%d,data=%d", ii, data);
            #5000 clk = 1;
            #5000 clk = 0;
            //$display("ii=%d,data=%d", ii, data);
        end

        i = 16'b0101010101010101;

        #5000 latch = 1;
        #5000 latch = 0;

        for (ii = 0; ii < 16; ii = ii + 1) begin
            //$strobe("ii=%d,data=%d", ii, data);
            #5000 clk = 1;
            #5000 clk = 0;
            //$display("ii=%d,data=%d", ii, data);
        end

        $finish();
    end  

    initial begin
        $monitor("clk=%d,data=%d",clk,data);
    end

endmodule