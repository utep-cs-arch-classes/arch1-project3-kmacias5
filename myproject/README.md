-README:

   PROJECT 3:     BLOCK GAME

   DATE:	  November 29, 2016

   GROUP:	  Karina Macias
	
   REFERENCES:    www.learntosolveit.com/cprogramming/Ex_4.12_recursive.itoa.html
		  https://github.com/utep-cs-arch-classes/arch1-project3-kmacias5/

   DESCRIPTION:   Block Game is a similar game to the famous Tetris game. The goal of the game is to clear as much lines
   		  as you can by filling the empty spaces with falling pieces or blocks that you can move to the right, left
		  or drop faster. The available pieces are a line, square and a Z figure. Every time you clear a line, you earn two points.
		  
		  The implementation performs the following operations in order:
		    -To start playing, press reset button on bottom.
		    -After you press button, screen would be cleared and a falling piece would appear.
		    -You can move it through the screen by pressing S1 to move right, S2 to move left, or S3 to fall down faster.
		    -The falling piece would continue to fall down even if you don't press any button until it reaches the bottom
		    or another piece on the bottom is located.
		    -If you want to pause the game, you have to press S4 until you want to continue.
		    -Every time a line is filled with a block that line is automatically cleared and gives you points for it.
		    -The game is over until one piece reaches the top of the screen.
		    
   HOW TO USE: 	    -To compile and test all files: $ make load
		    -To delete binaries: $ make clean
