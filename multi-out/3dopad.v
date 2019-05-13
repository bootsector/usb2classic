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
 
module threedeeohpad (
    input system_clock,
    input ps,
    input clk,
    output reg dat = 1'b1,
    input [BITS-1:0] i
);

parameter BITS = 16;

wire tmp;

shiftin s1 (
    .clk(clk), 
    .latch(ps), 
    .data(tmp),
    .system_clock(system_clock),
    .i(i)
);

always @(*) begin
    dat = !tmp || ps;
end

endmodule