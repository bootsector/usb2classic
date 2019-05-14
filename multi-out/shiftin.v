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
 
// MSB first
module shiftin (
    input system_clock,
    input clk,
    input latch,
    output reg data = 1'b1,
    input [BITS-1:0] i
);

parameter BITS = 16;

reg [BITS-1:0] tmp = ~0;

reg sync_clk, xfer_pipe;

always @(posedge system_clock) begin
    { sync_clk, xfer_pipe } <= { xfer_pipe, clk };
end

always @(posedge sync_clk or posedge latch) begin
    if (latch) begin
        tmp <= i;
    end else begin
        tmp <= {tmp[BITS-2:0],1'b0}; 
    end
end

always @(*) begin
    data = tmp[BITS-1];
end

endmodule