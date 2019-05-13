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
module multiout_tb;
    // Inputs
    reg system_clock = 1'd0;
    reg [11:0] shiftout_in;
    reg snes_clk;
    reg snes_latch;
    reg genesis_p7;
    reg threedo_clk;
    reg threedo_latch;
    
    // Outputs
    wire snes_data;
    wire [5:0] genesis_p;
    wire threedo_data;

    integer ii;

    // Instantiate the Unit Under Test (UUT)
    multiout uut (
        .system_clock(system_clock),
        .shiftout_in(shiftout_in),
        .snes_clk(snes_clk),
        .snes_latch(snes_latch),
        .genesis_p7(genesis_p7),
        .threedo_clk(threedo_clk),
        .threedo_latch(threedo_latch),

        .snes_data(snes_data),
        .genesis_p(genesis_p),
        .threedo_data(threedo_data)
    );

    always #50 system_clock <= !system_clock; // 20Mhz

    initial begin
        $timeformat(-3, 2, " ms", 10); 
        $dumpfile("multiout.vcd");
        $dumpvars(1, multiout_tb);
        // Initialize Inputs
        threedo_clk = 0;
        threedo_latch = 1;
        shiftout_in = 12'b111110101111;

        #13000 threedo_clk = 1;
        #13000 threedo_clk = 0;

        threedo_latch = 0;

        for (ii = 0; ii < 32; ii = ii + 1) begin
            #13000 threedo_clk = 1;
            #13000 threedo_clk = 0;
        end

        #13000 threedo_latch = 1;

        #13000 threedo_clk = 1;
        #13000 threedo_clk = 0;

        threedo_latch = 0;

        for (ii = 0; ii < 32; ii = ii + 1) begin
            #13000 threedo_clk = 1;
            #13000 threedo_clk = 0;
        end

        $finish();
    end  

    initial begin
        $monitor("threedo_clk=%d,threedo_data=%d",threedo_clk,threedo_data);
    end

endmodule