// video module for the DE1-SoC board
// Jan 2016
// by Fred Aulich
// Monitor counters and sync pulses


module monitor (

clk			, // 50 Mhz clock
gpio
);
// signal directions

input 		clk;
output[5:0] gpio;


// internal counters

reg[10:0]	contvidv; // vertical counter 
reg[10:0]	contvidh; // horizontal counter
reg[2:0]		framev;   // frame counter
reg[4:0]	   clkcount; // clock divider

// test points 40 pin header GPIO JP0 

reg[5:0]    gpio; 	

/////////////////////////////
////    control values  /////   
/////////////////////////////
reg 			vid_clk;

wire			vsync = ((contvidv >= 491) & (contvidv < 493))? 1'b0 : 1'b1;
wire			hsync = ((contvidh >= 664) & (contvidh < 760))? 1'b0 : 1'b1;
wire			vid_blank = ((contvidv >= 8) & (contvidv <  420) &(contvidh >= 20) & (contvidh < 624))? 1'b1 : 1'b0;
wire			clrvidh = (contvidh <= 800) ? 1'b0 : 1'b1;
wire  		clrvidv = (contvidv <= 525) ? 1'b0 : 1'b1;

////////////////////////////////////////
/// general clock divider 
/////////////////////////////////////////

 
 always @ (posedge clk )

begin 
		
		clkcount <= clkcount + 1;
		
end

///////////////////////////
///  25 Mhz clock    //////
///////////////////////////

always vid_clk <= clkcount[0]; 

///////////////////////////
/// frame counter    //////
///////////////////////////


always @ (posedge vsync)

begin

		framev <= framev + 1;
		
		end
		
/////////////////////////////////
// horizontal counter       /////
/////////////////////////////////

always @ (posedge vid_clk )

begin 

		if(clrvidh)
		begin
		contvidh <= 0;
		end
		
		else
		begin
		contvidh <= contvidh + 1;
		end
end

////////////////////////////////////////
//vertical counter when clrvidv is low /
////////////////////////////////////////

always @ (posedge vid_clk)

begin 

		if (clrvidv)
		begin
		contvidv <= 0;
		end
		
		else
		begin
			if
			(contvidh == 798)
			begin
			contvidv <= contvidv + 1; 
			end
		end
end


///////////////////////////////////////////////
/// test pins 40 pin header to logic analyzer//
///////////////////////////////////////////////

always gpio[0] <= vid_clk;
always gpio[1] <= vsync;
always gpio[2] <= hsync;
always gpio[3] <= vid_blank;
always gpio[4] <= clrvidh;
always gpio[5] <= clrvidv;

endmodule
	