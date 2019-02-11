/* 
**  Font data for Lucida Console 8pt
*/

//#include <stdlib.h>

//#include "font.h"
#include "interface/graphic_lcd/font_lucida8pt.h"

const FONT_INFO lucidaConsole_8ptFontInfo =
{
	.height = 11, /*  Character height */
	.start = ' ', /*  Start character */
	.end = '~', /*  End character */
	//lucidaConsole_8ptDescriptors, /*  Character descriptor array */
	//lucidaConsole_8ptBitmaps, /*  Character bitmap array */
//};

/* Character bitmaps for Lucida Console 8pt */
/*const uint8_t lucidaConsole_8ptBitmaps[] =*/
	.char_bitmaps =
{
	/* @0 ' ' (2 pixels wide) */
	0b00000000, //   
	0b00000000, //   
	0b00000000, //   
	0b00000000, //   
	0b00000000, //   
	0b00000000, //   
	0b00000000, //   
	0b00000000, //   
	0b00000000, //   
	0b00000000, //   
	0b00000000, //   

	/* @11 '!' (1 pixels wide) */
	0b00000000, //  
	0b00000000, //  
	0b10000000, // #
	0b10000000, // #
	0b10000000, // #
	0b10000000, // #
	0b10000000, // #
	0b00000000, //  
	0b10000000, // #
	0b00000000, //  
	0b00000000, //  

	/* @22 '"' (4 pixels wide) */
	0b00000000, //     
	0b10010000, // #  #
	0b10010000, // #  #
	0b10010000, // #  #
	0b00000000, //     
	0b00000000, //     
	0b00000000, //     
	0b00000000, //     
	0b00000000, //     
	0b00000000, //     
	0b00000000, //     

	/* @33 '#' (6 pixels wide) */
	0b00000000, //       
	0b00000000, //       
	0b00010100, //    # #
	0b00010100, //    # #
	0b01111100, //  #####
	0b00101000, //   # # 
	0b11111100, // ######
	0b01010000, //  # #  
	0b01010000, //  # #  
	0b00000000, //       
	0b00000000, //       

	/* @44 '$' (5 pixels wide) */
	0b00000000, //      
	0b00100000, //   #  
	0b01111000, //  ####
	0b10100000, // # #  
	0b11100000, // ###  
	0b00110000, //   ## 
	0b00101000, //   # #
	0b00101000, //   # #
	0b11110000, // #### 
	0b00100000, //   #  
	0b00000000, //      

	/* @55 '%' (7 pixels wide) */
	0b00000000, //        
	0b00000000, //        
	0b01000010, //  #    #
	0b10100100, // # #  # 
	0b10101000, // # # #  
	0b01010100, //  # # # 
	0b00101010, //   # # #
	0b01001010, //  #  # #
	0b10000100, // #    # 
	0b00000000, //        
	0b00000000, //        

	/* @66 '&' (7 pixels wide) */
	0b00000000, //        
	0b00000000, //        
	0b00100000, //   #    
	0b01010000, //  # #   
	0b01010000, //  # #   
	0b01100010, //  ##   #
	0b10010010, // #  #  #
	0b10001100, // #   ## 
	0b01111100, //  ##### 
	0b00000000, //        
	0b00000000, //        

	/* @77 ''' (1 pixels wide) */
	0b00000000, //  
	0b10000000, // #
	0b10000000, // #
	0b10000000, // #
	0b00000000, //  
	0b00000000, //  
	0b00000000, //  
	0b00000000, //  
	0b00000000, //  
	0b00000000, //  
	0b00000000, //  

	/* @88 '(' (4 pixels wide) */
	0b00000000, //     
	0b00110000, //   ##
	0b01000000, //  #  
	0b10000000, // #   
	0b10000000, // #   
	0b10000000, // #   
	0b10000000, // #   
	0b10000000, // #   
	0b10000000, // #   
	0b01000000, //  #  
	0b00110000, //   ##

	/* @99 ')' (4 pixels wide) */
	0b00000000, //     
	0b11000000, // ##  
	0b00100000, //   # 
	0b00010000, //    #
	0b00010000, //    #
	0b00010000, //    #
	0b00010000, //    #
	0b00010000, //    #
	0b00010000, //    #
	0b00100000, //   # 
	0b11000000, // ##  

	/* @110 '*' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b00100000, //   #  
	0b11011000, // ## ##
	0b01100000, //  ##  
	0b01010000, //  # # 
	0b00000000, //      
	0b00000000, //      
	0b00000000, //      
	0b00000000, //      
	0b00000000, //      

	/* @121 '+' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b00000000, //      
	0b00100000, //   #  
	0b00100000, //   #  
	0b11111000, // #####
	0b00100000, //   #  
	0b00100000, //   #  
	0b00100000, //   #  
	0b00000000, //      
	0b00000000, //      

	/* @132 ',' (2 pixels wide) */
	0b00000000, //   
	0b00000000, //   
	0b00000000, //   
	0b00000000, //   
	0b00000000, //   
	0b00000000, //   
	0b00000000, //   
	0b11000000, // ##
	0b11000000, // ##
	0b01000000, //  #
	0b10000000, // # 

	/* @143 '-' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b00000000, //      
	0b00000000, //      
	0b00000000, //      
	0b11111000, // #####
	0b00000000, //      
	0b00000000, //      
	0b00000000, //      
	0b00000000, //      
	0b00000000, //      

	/* @154 '.' (2 pixels wide) */
	0b00000000, //   
	0b00000000, //   
	0b00000000, //   
	0b00000000, //   
	0b00000000, //   
	0b00000000, //   
	0b00000000, //   
	0b11000000, // ##
	0b11000000, // ##
	0b00000000, //   
	0b00000000, //   

	/* @165 '/' (7 pixels wide) */
	0b00000000, //        
	0b00000010, //       #
	0b00000100, //      # 
	0b00001100, //     ## 
	0b00001000, //     #  
	0b00010000, //    #   
	0b00010000, //    #   
	0b00100000, //   #    
	0b01100000, //  ##    
	0b01000000, //  #     
	0b10000000, // #      

	/* @176 '0' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b01110000, //  ### 
	0b10001000, // #   #
	0b10001000, // #   #
	0b10001000, // #   #
	0b10001000, // #   #
	0b10001000, // #   #
	0b01110000, //  ### 
	0b00000000, //      
	0b00000000, //      

	/* @187 '1' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b01100000, //  ##  
	0b10100000, // # #  
	0b00100000, //   #  
	0b00100000, //   #  
	0b00100000, //   #  
	0b00100000, //   #  
	0b11111000, // #####
	0b00000000, //      
	0b00000000, //      

	/* @198 '2' (4 pixels wide) */
	0b00000000, //     
	0b00000000, //     
	0b11100000, // ### 
	0b00010000, //    #
	0b00010000, //    #
	0b00100000, //   # 
	0b01000000, //  #  
	0b10000000, // #   
	0b11110000, // ####
	0b00000000, //     
	0b00000000, //     

	/* @209 '3' (4 pixels wide) */
	0b00000000, //     
	0b00000000, //     
	0b11110000, // ####
	0b00010000, //    #
	0b00010000, //    #
	0b01100000, //  ## 
	0b00010000, //    #
	0b00010000, //    #
	0b11100000, // ### 
	0b00000000, //     
	0b00000000, //     

	/* @220 '4' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b00010000, //    # 
	0b00110000, //   ## 
	0b01010000, //  # # 
	0b10010000, // #  # 
	0b11111000, // #####
	0b00010000, //    # 
	0b00010000, //    # 
	0b00000000, //      
	0b00000000, //      

	/* @231 '5' (4 pixels wide) */
	0b00000000, //     
	0b00000000, //     
	0b11110000, // ####
	0b10000000, // #   
	0b10000000, // #   
	0b11100000, // ### 
	0b00010000, //    #
	0b00010000, //    #
	0b11100000, // ### 
	0b00000000, //     
	0b00000000, //     

	/* @242 '6' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b00111000, //   ###
	0b01000000, //  #   
	0b10000000, // #    
	0b10110000, // # ## 
	0b11001000, // ##  #
	0b10001000, // #   #
	0b01110000, //  ### 
	0b00000000, //      
	0b00000000, //      

	/* @253 '7' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b11111000, // #####
	0b00010000, //    # 
	0b00010000, //    # 
	0b00100000, //   #  
	0b01000000, //  #   
	0b01000000, //  #   
	0b10000000, // #    
	0b00000000, //      
	0b00000000, //      

	/* @264 '8' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b01110000, //  ### 
	0b10001000, // #   #
	0b10010000, // #  # 
	0b01110000, //  ### 
	0b10001000, // #   #
	0b10001000, // #   #
	0b01110000, //  ### 
	0b00000000, //      
	0b00000000, //      

	/* @275 '9' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b01110000, //  ### 
	0b10001000, // #   #
	0b10001000, // #   #
	0b01111000, //  ####
	0b00001000, //     #
	0b00010000, //    # 
	0b11100000, // ###  
	0b00000000, //      
	0b00000000, //      

	/* @286 ':' (2 pixels wide) */
	0b00000000, //   
	0b00000000, //   
	0b00000000, //   
	0b11000000, // ##
	0b11000000, // ##
	0b00000000, //   
	0b00000000, //   
	0b11000000, // ##
	0b11000000, // ##
	0b00000000, //   
	0b00000000, //   

	/* @297 ';' (2 pixels wide) */
	0b00000000, //   
	0b00000000, //   
	0b00000000, //   
	0b11000000, // ##
	0b11000000, // ##
	0b00000000, //   
	0b00000000, //   
	0b11000000, // ##
	0b11000000, // ##
	0b01000000, //  #
	0b10000000, // # 

	/* @308 '<' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b00000000, //      
	0b00001000, //     #
	0b00110000, //   ## 
	0b11000000, // ##   
	0b11000000, // ##   
	0b00110000, //   ## 
	0b00001000, //     #
	0b00000000, //      
	0b00000000, //      

	/* @319 '=' (7 pixels wide) */
	0b00000000, //        
	0b00000000, //        
	0b00000000, //        
	0b00000000, //        
	0b11111110, // #######
	0b00000000, //        
	0b11111110, // #######
	0b00000000, //        
	0b00000000, //        
	0b00000000, //        
	0b00000000, //        

	/* @330 '>' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b00000000, //      
	0b10000000, // #    
	0b01100000, //  ##  
	0b00011000, //    ##
	0b00011000, //    ##
	0b01100000, //  ##  
	0b10000000, // #    
	0b00000000, //      
	0b00000000, //      

	/* @341 '?' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b11110000, // #### 
	0b10001000, // #   #
	0b00001000, //     #
	0b00010000, //    # 
	0b00100000, //   #  
	0b00000000, //      
	0b00100000, //   #  
	0b00000000, //      
	0b00000000, //      

	/* @352 '@' (7 pixels wide) */
	0b00000000, //        
	0b00000000, //        
	0b00111100, //   #### 
	0b01000100, //  #   # 
	0b10011100, // #  ### 
	0b10100100, // # #  # 
	0b10111110, // # #####
	0b01000000, //  #     
	0b00111000, //   ###  
	0b00000000, //        
	0b00000000, //        

	/* @363 'A' (7 pixels wide) */
	0b00000000, //        
	0b00000000, //        
	0b00010000, //    #   
	0b00101000, //   # #  
	0b00101000, //   # #  
	0b01000100, //  #   # 
	0b01111100, //  ##### 
	0b01000100, //  #   # 
	0b10000010, // #     #
	0b00000000, //        
	0b00000000, //        

	/* @374 'B' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b11110000, // #### 
	0b10001000, // #   #
	0b10001000, // #   #
	0b11110000, // #### 
	0b10001000, // #   #
	0b10001000, // #   #
	0b11110000, // #### 
	0b00000000, //      
	0b00000000, //      

	/* @385 'C' (6 pixels wide) */
	0b00000000, //       
	0b00000000, //       
	0b00111100, //   ####
	0b01000000, //  #    
	0b10000000, // #     
	0b10000000, // #     
	0b10000000, // #     
	0b01000000, //  #    
	0b00111100, //   ####
	0b00000000, //       
	0b00000000, //       

	/* @396 'D' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b11110000, // #### 
	0b10001000, // #   #
	0b10001000, // #   #
	0b10001000, // #   #
	0b10001000, // #   #
	0b10001000, // #   #
	0b11110000, // #### 
	0b00000000, //      
	0b00000000, //      

	/* @407 'E' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b11111000, // #####
	0b10000000, // #    
	0b10000000, // #    
	0b11110000, // #### 
	0b10000000, // #    
	0b10000000, // #    
	0b11111000, // #####
	0b00000000, //      
	0b00000000, //      

	/* @418 'F' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b11111000, // #####
	0b10000000, // #    
	0b10000000, // #    
	0b11110000, // #### 
	0b10000000, // #    
	0b10000000, // #    
	0b10000000, // #    
	0b00000000, //      
	0b00000000, //      

	/* @429 'G' (6 pixels wide) */
	0b00000000, //       
	0b00000000, //       
	0b00111100, //   ####
	0b01000000, //  #    
	0b10000000, // #     
	0b10001100, // #   ##
	0b10000100, // #    #
	0b01000100, //  #   #
	0b00111100, //   ####
	0b00000000, //       
	0b00000000, //       

	/* @440 'H' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b10001000, // #   #
	0b10001000, // #   #
	0b10001000, // #   #
	0b11111000, // #####
	0b10001000, // #   #
	0b10001000, // #   #
	0b10001000, // #   #
	0b00000000, //      
	0b00000000, //      

	/* @451 'I' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b11111000, // #####
	0b00100000, //   #  
	0b00100000, //   #  
	0b00100000, //   #  
	0b00100000, //   #  
	0b00100000, //   #  
	0b11111000, // #####
	0b00000000, //      
	0b00000000, //      

	/* @462 'J' (4 pixels wide) */
	0b00000000, //     
	0b00000000, //     
	0b01110000, //  ###
	0b00010000, //    #
	0b00010000, //    #
	0b00010000, //    #
	0b00010000, //    #
	0b00010000, //    #
	0b11100000, // ### 
	0b00000000, //     
	0b00000000, //     

	/* @473 'K' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b10001000, // #   #
	0b10010000, // #  # 
	0b10100000, // # #  
	0b11000000, // ##   
	0b10100000, // # #  
	0b10010000, // #  # 
	0b10001000, // #   #
	0b00000000, //      
	0b00000000, //      

	/* @484 'L' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b10000000, // #    
	0b10000000, // #    
	0b10000000, // #    
	0b10000000, // #    
	0b10000000, // #    
	0b10000000, // #    
	0b11111000, // #####
	0b00000000, //      
	0b00000000, //      

	/* @495 'M' (6 pixels wide) */
	0b00000000, //       
	0b00000000, //       
	0b11001100, // ##  ##
	0b11001100, // ##  ##
	0b11010100, // ## # #
	0b10110100, // # ## #
	0b10110100, // # ## #
	0b10100100, // # #  #
	0b10000100, // #    #
	0b00000000, //       
	0b00000000, //       

	/* @506 'N' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b10001000, // #   #
	0b11001000, // ##  #
	0b11101000, // ### #
	0b10101000, // # # #
	0b10011000, // #  ##
	0b10011000, // #  ##
	0b10001000, // #   #
	0b00000000, //      
	0b00000000, //      

	/* @517 'O' (6 pixels wide) */
	0b00000000, //       
	0b00000000, //       
	0b01111000, //  #### 
	0b10000100, // #    #
	0b10000100, // #    #
	0b10000100, // #    #
	0b10000100, // #    #
	0b10000100, // #    #
	0b01111000, //  #### 
	0b00000000, //       
	0b00000000, //       

	/* @528 'P' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b11110000, // #### 
	0b10001000, // #   #
	0b10001000, // #   #
	0b11110000, // #### 
	0b10000000, // #    
	0b10000000, // #    
	0b10000000, // #    
	0b00000000, //      
	0b00000000, //      

	/* @539 'Q' (7 pixels wide) */
	0b00000000, //        
	0b00000000, //        
	0b01111000, //  ####  
	0b10000100, // #    # 
	0b10000100, // #    # 
	0b10000100, // #    # 
	0b10000100, // #    # 
	0b10000100, // #    # 
	0b01111000, //  ####  
	0b00001100, //     ## 
	0b00000110, //      ##

	/* @550 'R' (6 pixels wide) */
	0b00000000, //       
	0b00000000, //       
	0b11110000, // ####  
	0b10001000, // #   # 
	0b10001000, // #   # 
	0b11110000, // ####  
	0b10010000, // #  #  
	0b10001000, // #   # 
	0b10000100, // #    #
	0b00000000, //       
	0b00000000, //       

	/* @561 'S' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b01111000, //  ####
	0b10000000, // #    
	0b10000000, // #    
	0b01110000, //  ### 
	0b00001000, //     #
	0b00001000, //     #
	0b11110000, // #### 
	0b00000000, //      
	0b00000000, //      

	/* @572 'T' (7 pixels wide) */
	0b00000000, //        
	0b00000000, //        
	0b11111110, // #######
	0b00010000, //    #   
	0b00010000, //    #   
	0b00010000, //    #   
	0b00010000, //    #   
	0b00010000, //    #   
	0b00010000, //    #   
	0b00000000, //        
	0b00000000, //        

	/* @583 'U' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b10001000, // #   #
	0b10001000, // #   #
	0b10001000, // #   #
	0b10001000, // #   #
	0b10001000, // #   #
	0b10001000, // #   #
	0b01110000, //  ### 
	0b00000000, //      
	0b00000000, //      

	/* @594 'V' (7 pixels wide) */
	0b00000000, //        
	0b00000000, //        
	0b10000010, // #     #
	0b01000100, //  #   # 
	0b01000100, //  #   # 
	0b01000100, //  #   # 
	0b00101000, //   # #  
	0b00101000, //   # #  
	0b00010000, //    #   
	0b00000000, //        
	0b00000000, //        

	/* @605 'W' (7 pixels wide) */
	0b00000000, //        
	0b00000000, //        
	0b10000010, // #     #
	0b10010010, // #  #  #
	0b10010010, // #  #  #
	0b01101010, //  ## # #
	0b01101100, //  ## ## 
	0b01001100, //  #  ## 
	0b01000100, //  #   # 
	0b00000000, //        
	0b00000000, //        

	/* @616 'X' (7 pixels wide) */
	0b00000000, //        
	0b00000000, //        
	0b10000010, // #     #
	0b01000100, //  #   # 
	0b00101000, //   # #  
	0b00010000, //    #   
	0b00101000, //   # #  
	0b01000100, //  #   # 
	0b10000010, // #     #
	0b00000000, //        
	0b00000000, //        

	/* @627 'Y' (7 pixels wide) */
	0b00000000, //        
	0b00000000, //        
	0b10000010, // #     #
	0b01000100, //  #   # 
	0b00101000, //   # #  
	0b00010000, //    #   
	0b00010000, //    #   
	0b00010000, //    #   
	0b00010000, //    #   
	0b00000000, //        
	0b00000000, //        

	/* @638 'Z' (6 pixels wide) */
	0b00000000, //       
	0b00000000, //       
	0b11111100, // ######
	0b00000100, //      #
	0b00001000, //     # 
	0b00010000, //    #  
	0b00100000, //   #   
	0b01000000, //  #    
	0b11111100, // ######
	0b00000000, //       
	0b00000000, //       

	/* @649 '[' (3 pixels wide) */
	0b00000000, //    
	0b11100000, // ###
	0b10000000, // #  
	0b10000000, // #  
	0b10000000, // #  
	0b10000000, // #  
	0b10000000, // #  
	0b10000000, // #  
	0b10000000, // #  
	0b10000000, // #  
	0b11100000, // ###

	/* @660 '\' (7 pixels wide) */
	0b00000000, //        
	0b10000000, // #      
	0b01000000, //  #     
	0b01000000, //  #     
	0b00100000, //   #    
	0b00010000, //    #   
	0b00010000, //    #   
	0b00001000, //     #  
	0b00001000, //     #  
	0b00000100, //      # 
	0b00000010, //       #

	/* @671 ']' (3 pixels wide) */
	0b00000000, //    
	0b11100000, // ###
	0b00100000, //   #
	0b00100000, //   #
	0b00100000, //   #
	0b00100000, //   #
	0b00100000, //   #
	0b00100000, //   #
	0b00100000, //   #
	0b00100000, //   #
	0b11100000, // ###

	/* @682 '^' (7 pixels wide) */
	0b00000000, //        
	0b00010000, //    #   
	0b00010000, //    #   
	0b00101000, //   # #  
	0b01101100, //  ## ## 
	0b01000100, //  #   # 
	0b10000010, // #     #
	0b00000000, //        
	0b00000000, //        
	0b00000000, //        
	0b00000000, //        

	/* @693 '_' (7 pixels wide) */
	0b00000000, //        
	0b00000000, //        
	0b00000000, //        
	0b00000000, //        
	0b00000000, //        
	0b00000000, //        
	0b00000000, //        
	0b00000000, //        
	0b00000000, //        
	0b11111110, // #######
	0b00000000, //        

	/* @704 '`' (2 pixels wide) */
	0b10000000, // # 
	0b01000000, //  #
	0b00000000, //   
	0b00000000, //   
	0b00000000, //   
	0b00000000, //   
	0b00000000, //   
	0b00000000, //   
	0b00000000, //   
	0b00000000, //   
	0b00000000, //   

	/* @715 'a' (6 pixels wide) */
	0b00000000, //       
	0b00000000, //       
	0b00000000, //       
	0b01110000, //  ###  
	0b00001000, //     # 
	0b01111000, //  #### 
	0b10001000, // #   # 
	0b10001000, // #   # 
	0b01111100, //  #####
	0b00000000, //       
	0b00000000, //       

	/* @726 'b' (5 pixels wide) */
	0b00000000, //      
	0b10000000, // #    
	0b10000000, // #    
	0b10110000, // # ## 
	0b11001000, // ##  #
	0b10001000, // #   #
	0b10001000, // #   #
	0b10001000, // #   #
	0b11110000, // #### 
	0b00000000, //      
	0b00000000, //      

	/* @737 'c' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b00000000, //      
	0b01111000, //  ####
	0b10000000, // #    
	0b10000000, // #    
	0b10000000, // #    
	0b10000000, // #    
	0b01111000, //  ####
	0b00000000, //      
	0b00000000, //      

	/* @748 'd' (5 pixels wide) */
	0b00000000, //      
	0b00001000, //     #
	0b00001000, //     #
	0b01111000, //  ####
	0b10001000, // #   #
	0b10001000, // #   #
	0b10001000, // #   #
	0b10001000, // #   #
	0b01111000, //  ####
	0b00000000, //      
	0b00000000, //      

	/* @759 'e' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b00000000, //      
	0b01110000, //  ### 
	0b10001000, // #   #
	0b11111000, // #####
	0b10000000, // #    
	0b10000000, // #    
	0b01111000, //  ####
	0b00000000, //      
	0b00000000, //      

	/* @770 'f' (6 pixels wide) */
	0b00000000, //       
	0b00011100, //    ###
	0b00100000, //   #   
	0b11111100, // ######
	0b00100000, //   #   
	0b00100000, //   #   
	0b00100000, //   #   
	0b00100000, //   #   
	0b00100000, //   #   
	0b00000000, //       
	0b00000000, //       

	/* @781 'g' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b00000000, //      
	0b01111000, //  ####
	0b10001000, // #   #
	0b10001000, // #   #
	0b10001000, // #   #
	0b10001000, // #   #
	0b01111000, //  ####
	0b00001000, //     #
	0b01110000, //  ### 

	/* @792 'h' (5 pixels wide) */
	0b00000000, //      
	0b10000000, // #    
	0b10000000, // #    
	0b10111000, // # ###
	0b11001000, // ##  #
	0b10001000, // #   #
	0b10001000, // #   #
	0b10001000, // #   #
	0b10001000, // #   #
	0b00000000, //      
	0b00000000, //      

	/* @803 'i' (3 pixels wide) */
	0b00000000, //    
	0b00100000, //   #
	0b00000000, //    
	0b11100000, // ###
	0b00100000, //   #
	0b00100000, //   #
	0b00100000, //   #
	0b00100000, //   #
	0b00100000, //   #
	0b00000000, //    
	0b00000000, //    

	/* @814 'j' (4 pixels wide) */
	0b00000000, //     
	0b00010000, //    #
	0b00000000, //     
	0b01110000, //  ###
	0b00010000, //    #
	0b00010000, //    #
	0b00010000, //    #
	0b00010000, //    #
	0b00010000, //    #
	0b00010000, //    #
	0b11100000, // ### 

	/* @825 'k' (5 pixels wide) */
	0b00000000, //      
	0b10000000, // #    
	0b10000000, // #    
	0b10001000, // #   #
	0b10010000, // #  # 
	0b11100000, // ###  
	0b10100000, // # #  
	0b10010000, // #  # 
	0b10001000, // #   #
	0b00000000, //      
	0b00000000, //      

	/* @836 'l' (3 pixels wide) */
	0b00000000, //    
	0b11100000, // ###
	0b00100000, //   #
	0b00100000, //   #
	0b00100000, //   #
	0b00100000, //   #
	0b00100000, //   #
	0b00100000, //   #
	0b00100000, //   #
	0b00000000, //    
	0b00000000, //    

	/* @847 'm' (7 pixels wide) */
	0b00000000, //        
	0b00000000, //        
	0b00000000, //        
	0b10110110, // # ## ##
	0b11011010, // ## ## #
	0b10010010, // #  #  #
	0b10010010, // #  #  #
	0b10010010, // #  #  #
	0b10010010, // #  #  #
	0b00000000, //        
	0b00000000, //        

	/* @858 'n' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b00000000, //      
	0b10111000, // # ###
	0b11001000, // ##  #
	0b10001000, // #   #
	0b10001000, // #   #
	0b10001000, // #   #
	0b10001000, // #   #
	0b00000000, //      
	0b00000000, //      

	/* @869 'o' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b00000000, //      
	0b01110000, //  ### 
	0b10001000, // #   #
	0b10001000, // #   #
	0b10001000, // #   #
	0b10001000, // #   #
	0b01110000, //  ### 
	0b00000000, //      
	0b00000000, //      

	/* @880 'p' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b00000000, //      
	0b10110000, // # ## 
	0b11001000, // ##  #
	0b10001000, // #   #
	0b10001000, // #   #
	0b10001000, // #   #
	0b11110000, // #### 
	0b10000000, // #    
	0b10000000, // #    

	/* @891 'q' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b00000000, //      
	0b01111000, //  ####
	0b10001000, // #   #
	0b10001000, // #   #
	0b10001000, // #   #
	0b10001000, // #   #
	0b01111000, //  ####
	0b00001000, //     #
	0b00001000, //     #

	/* @902 'r' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b00000000, //      
	0b10111000, // # ###
	0b11001000, // ##  #
	0b10000000, // #    
	0b10000000, // #    
	0b10000000, // #    
	0b10000000, // #    
	0b00000000, //      
	0b00000000, //      

	/* @913 's' (4 pixels wide) */
	0b00000000, //     
	0b00000000, //     
	0b00000000, //     
	0b11110000, // ####
	0b10000000, // #   
	0b11000000, // ##  
	0b00110000, //   ##
	0b00010000, //    #
	0b11100000, // ### 
	0b00000000, //     
	0b00000000, //     

	/* @924 't' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b01000000, //  #   
	0b11111000, // #####
	0b01000000, //  #   
	0b01000000, //  #   
	0b01000000, //  #   
	0b01000000, //  #   
	0b00111000, //   ###
	0b00000000, //      
	0b00000000, //      

	/* @935 'u' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b00000000, //      
	0b10001000, // #   #
	0b10001000, // #   #
	0b10001000, // #   #
	0b10001000, // #   #
	0b10011000, // #  ##
	0b11101000, // ### #
	0b00000000, //      
	0b00000000, //      

	/* @946 'v' (7 pixels wide) */
	0b00000000, //        
	0b00000000, //        
	0b00000000, //        
	0b10000010, // #     #
	0b01000100, //  #   # 
	0b01000100, //  #   # 
	0b00101000, //   # #  
	0b00101000, //   # #  
	0b00010000, //    #   
	0b00000000, //        
	0b00000000, //        

	/* @957 'w' (7 pixels wide) */
	0b00000000, //        
	0b00000000, //        
	0b00000000, //        
	0b10000010, // #     #
	0b10010010, // #  #  #
	0b10101010, // # # # #
	0b10101010, // # # # #
	0b01101100, //  ## ## 
	0b01000100, //  #   # 
	0b00000000, //        
	0b00000000, //        

	/* @968 'x' (6 pixels wide) */
	0b00000000, //       
	0b00000000, //       
	0b00000000, //       
	0b10000100, // #    #
	0b01001000, //  #  # 
	0b00110000, //   ##  
	0b00110000, //   ##  
	0b01001000, //  #  # 
	0b10000100, // #    #
	0b00000000, //       
	0b00000000, //       

	/* @979 'y' (7 pixels wide) */
	0b00000000, //        
	0b00000000, //        
	0b00000000, //        
	0b10000010, // #     #
	0b01000100, //  #   # 
	0b01000100, //  #   # 
	0b00101000, //   # #  
	0b00101000, //   # #  
	0b00010000, //    #   
	0b00110000, //   ##   
	0b11100000, // ###    

	/* @990 'z' (5 pixels wide) */
	0b00000000, //      
	0b00000000, //      
	0b00000000, //      
	0b11111000, // #####
	0b00001000, //     #
	0b00010000, //    # 
	0b00100000, //   #  
	0b01000000, //  #   
	0b11111000, // #####
	0b00000000, //      
	0b00000000, //      

	/* @1001 '{' (4 pixels wide) */
	0b00000000, //     
	0b00110000, //   ##
	0b01000000, //  #  
	0b01000000, //  #  
	0b01000000, //  #  
	0b11000000, // ##  
	0b01000000, //  #  
	0b01000000, //  #  
	0b01000000, //  #  
	0b01000000, //  #  
	0b00110000, //   ##

	/* @1012 '|' (1 pixels wide) */
	0b00000000, //  
	0b10000000, // #
	0b10000000, // #
	0b10000000, // #
	0b10000000, // #
	0b10000000, // #
	0b10000000, // #
	0b10000000, // #
	0b10000000, // #
	0b10000000, // #
	0b10000000, // #

	/* @1023 '}' (4 pixels wide) */
	0b00000000, //     
	0b11000000, // ##  
	0b00100000, //   # 
	0b00100000, //   # 
	0b00100000, //   # 
	0b00110000, //   ##
	0b00100000, //   # 
	0b00100000, //   # 
	0b00100000, //   # 
	0b00100000, //   # 
	0b11100000, // ### 

	/* @1034 '~' (7 pixels wide) */
	0b00000000, //        
	0b00000000, //        
	0b00000000, //        
	0b00000000, //        
	0b00000000, //        
	0b01110010, //  ###  #
	0b10011100, // #  ### 
	0b00000000, //        
	0b00000000, //        
	0b00000000, //        
	0b00000000, //        
},//;

/* Character descriptors for Lucida Console 8pt */
/* { [Char width in bits], [Offset into lucidaConsole_8ptCharBitmaps in bytes] } */
//const FONT_CHAR_INFO lucidaConsole_8ptDescriptors[] =
.char_descriptors =
{
	{2, 0}, 		/*   */ 
	{1, 11}, 		/* ! */ 
	{4, 22}, 		/* " */ 
	{6, 33}, 		/* # */ 
	{5, 44}, 		/* $ */ 
	{7, 55}, 		/* % */ 
	{7, 66}, 		/* & */ 
	{1, 77}, 		/* ' */ 
	{4, 88}, 		/* ( */ 
	{4, 99}, 		/* ) */ 
	{5, 110}, 		/* * */ 
	{5, 121}, 		/* + */ 
	{2, 132}, 		/* , */ 
	{5, 143}, 		/* - */ 
	{2, 154}, 		/* . */ 
	{7, 165}, 		/* / */ 
	{5, 176}, 		/* 0 */ 
	{5, 187}, 		/* 1 */ 
	{4, 198}, 		/* 2 */ 
	{4, 209}, 		/* 3 */ 
	{5, 220}, 		/* 4 */ 
	{4, 231}, 		/* 5 */ 
	{5, 242}, 		/* 6 */ 
	{5, 253}, 		/* 7 */ 
	{5, 264}, 		/* 8 */ 
	{5, 275}, 		/* 9 */ 
	{2, 286}, 		/* : */ 
	{2, 297}, 		/* ; */ 
	{5, 308}, 		/* < */ 
	{7, 319}, 		/* = */ 
	{5, 330}, 		/* > */ 
	{5, 341}, 		/* ? */ 
	{7, 352}, 		/* @ */ 
	{7, 363}, 		/* A */ 
	{5, 374}, 		/* B */ 
	{6, 385}, 		/* C */ 
	{5, 396}, 		/* D */ 
	{5, 407}, 		/* E */ 
	{5, 418}, 		/* F */ 
	{6, 429}, 		/* G */ 
	{5, 440}, 		/* H */ 
	{5, 451}, 		/* I */ 
	{4, 462}, 		/* J */ 
	{5, 473}, 		/* K */ 
	{5, 484}, 		/* L */ 
	{6, 495}, 		/* M */ 
	{5, 506}, 		/* N */ 
	{6, 517}, 		/* O */ 
	{5, 528}, 		/* P */ 
	{7, 539}, 		/* Q */ 
	{6, 550}, 		/* R */ 
	{5, 561}, 		/* S */ 
	{7, 572}, 		/* T */ 
	{5, 583}, 		/* U */ 
	{7, 594}, 		/* V */ 
	{7, 605}, 		/* W */ 
	{7, 616}, 		/* X */ 
	{7, 627}, 		/* Y */ 
	{6, 638}, 		/* Z */ 
	{3, 649}, 		/* [ */ 
	{7, 660}, 		/* \ */ 
	{3, 671}, 		/* ] */ 
	{7, 682}, 		/* ^ */ 
	{7, 693}, 		/* _ */ 
	{2, 704}, 		/* ` */ 
	{6, 715}, 		/* a */ 
	{5, 726}, 		/* b */ 
	{5, 737}, 		/* c */ 
	{5, 748}, 		/* d */ 
	{5, 759}, 		/* e */ 
	{6, 770}, 		/* f */ 
	{5, 781}, 		/* g */ 
	{5, 792}, 		/* h */ 
	{3, 803}, 		/* i */ 
	{4, 814}, 		/* j */ 
	{5, 825}, 		/* k */ 
	{3, 836}, 		/* l */ 
	{7, 847}, 		/* m */ 
	{5, 858}, 		/* n */ 
	{5, 869}, 		/* o */ 
	{5, 880}, 		/* p */ 
	{5, 891}, 		/* q */ 
	{5, 902}, 		/* r */ 
	{4, 913}, 		/* s */ 
	{5, 924}, 		/* t */ 
	{5, 935}, 		/* u */ 
	{7, 946}, 		/* v */ 
	{7, 957}, 		/* w */ 
	{6, 968}, 		/* x */ 
	{7, 979}, 		/* y */ 
	{5, 990}, 		/* z */ 
	{4, 1001}, 		/* { */ 
	{1, 1012}, 		/* | */ 
	{4, 1023}, 		/* } */ 
	{7, 1034}, 		/* ~ */ 
}//;
};

