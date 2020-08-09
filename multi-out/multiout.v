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

module multiout (
    // System Clock
    input system_clock,
     
    // External Shift Out input
    input [11:0] shiftout_in,

    // (S)NES output connection signals
    input snes_latch,
    input snes_clk,
    output reg snes_data = 1'b1,

    // Sega Genesis connection signals
    input genesis_p7,
    output reg [5:0] genesis_p = 6'b111111,

    // 3DO connection signals
    input threedo_clk,
    input threedo_latch,
    output reg threedo_data = 1'b1
);

// (S)NES Shift-In output
wire shiftin_output;

// Sega Genesis Output
wire [5:0] genesis_p_out;

// 3DO Shift-In output
wire threedo_output;
    
// (S)NES Shift-In Register
shiftin si1 (
    .clk(snes_clk), 
    .latch(snes_latch), 
    .data(shiftin_output),
    .system_clock(system_clock),
    .i({shiftout_in[0],
        shiftout_in[1],
        shiftout_in[2],
        shiftout_in[3],
        shiftout_in[4],
        shiftout_in[5],
        shiftout_in[6],
        shiftout_in[7],
        shiftout_in[8],
        shiftout_in[9],
        shiftout_in[10],
        shiftout_in[11]
    })
);

// Sega Genesis 6button pad encoder
smdsixbutton smdpad (
    .p7(genesis_p7),
    .up(shiftout_in[4]), 
    .dw(shiftout_in[5]),
    .lf(shiftout_in[6]),
    .rg(shiftout_in[7]),
    .a(shiftout_in[1]),
    .b(shiftout_in[0]),
    .c(shiftout_in[8]),
    .st(shiftout_in[3]),
    .x(shiftout_in[10]),
    .y(shiftout_in[9]),
    .z(shiftout_in[11]),
    .md(shiftout_in[2]),
    .clk(system_clock),
    .p(genesis_p_out)
);

// 3DO pad encoder
threedeeohpad threedopad (
    .clk(threedo_clk),
    .ps(threedo_latch), 
    .dat(threedo_output),
    .system_clock(system_clock),
    .i({
        1'b1,
        1'b1,
        shiftout_in[5],  // Down
        shiftout_in[4],  // Up
        shiftout_in[7],  // Right
        shiftout_in[6],  // Left
        shiftout_in[1],  // A
        shiftout_in[0],  // B
        shiftout_in[8],  // C
        shiftout_in[3],  // P
        shiftout_in[2],  // X
        shiftout_in[11], // R
        shiftout_in[10], // L
        1'b1,
        1'b1,
        1'b0
    })
);

always @(*) begin
    snes_data = shiftin_output;
    genesis_p = genesis_p_out;
    threedo_data = threedo_output;
end

endmodule