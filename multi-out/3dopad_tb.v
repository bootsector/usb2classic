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
module threedeeohpad_tb;
    // Inputs
    reg clk;
    reg latch;
    reg [15:0] i;
    
    reg sys_clk = 1'd0;

    // Outputs
    wire data;

    integer ii;

    // Instantiate the Unit Under Test (UUT)
    threedeeohpad uut (
        .clk(clk), 
        .system_clock(sys_clk),
        .ps(latch), 
        .dat(data),
        .i(i)
    );

    always #50 sys_clk <= !sys_clk; // 20Mhz

    initial begin
        $timeformat(-3, 2, " ms", 10); 
        $dumpfile("3do.vcd");
        $dumpvars(1, threedeeohpad_tb);
        // Initialize Inputs
        clk = 0;
        latch = 1;
        i = 16'b1100000000000110;

        #13000 clk = 1;
        #13000 clk = 0;

        latch = 0;

        for (ii = 0; ii < 32; ii = ii + 1) begin
            #13000 clk = 1;
            #13000 clk = 0;
            //$strobe("ii=%d,data=%d", ii, data);
        end

        #13000 latch = 1;

        #13000 clk = 1;
        #13000 clk = 0;

        latch = 0;

        for (ii = 0; ii < 32; ii = ii + 1) begin
            #13000 clk = 1;
            #13000 clk = 0;
            //$strobe("ii=%d,data=%d", ii, data);
        end

        $finish();
    end  

    initial begin
        $monitor("clock=%d,data=%d",clk,data);
    end

endmodule