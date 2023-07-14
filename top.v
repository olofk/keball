module top
  (input wire	     i_clk,
   output reg [15:0] o_left,
   output reg [15:0] o_right);

   reg [15:0]	     lbuf [0:383999];
   reg [15:0]	     rbuf [0:383999];

   initial $readmemh("left.hex", lbuf);
   initial $readmemh("right.hex", rbuf);

   reg [31:0]	     i = 0;

   always @(posedge i_clk) begin
      i <= i+1;
      if (i==383999)
	i <= 0;
      o_left  <= lbuf[i];
      o_right <= rbuf[i];
   end

endmodule
   
