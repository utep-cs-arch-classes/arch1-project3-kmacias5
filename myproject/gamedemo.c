#include <msp430.h>
#include "sound.h"
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include <abCircle.h>

#define GREEN_LED BIT6;
/**measure of board array **/
#define COLS 8
#define ROWS 15
/**type of figure there can be on game**/
#define SQUARE 1
#define LINE 2
#define ZI 3
/**struct to denote the current place falling, it has 4 coordinates for each part that makes up the entire piece**/
struct Piece{
  int type; 
  int coord1[2];
  int coord2[2];
  int coord3[2];
  int coord4[2];
};

/**variables that are used through the program**/
int redrawScreen;
/**background color**/
u_int bgColor = COLOR_BLACK;
/**letter, numbers,and margins color**/
u_int screenColor = COLOR_RED;
/**array to hold the board state**/
int board[8][15];
/**current score and lines cleared**/
int score = 0;
int lines = 0;
/**to store current piece being dropped**/
struct Piece currentP;

/**initialize board, all set to 0 which denotes no figure is drawn/placed**/
void init_board(){
  int i, j;
  for(i = 0; i < COLS; i++){
    for(j = 0; j < ROWS; j++){
      board[i][j] = 0;
    }
  }
}
/** initialize the current piece coordinates depending on the type**/
void init_currentPiece(){
  /**alternate between the two types of pieces**/
  if(currentP.type == LINE){
    currentP.type = ZI;
  }
  else if(currentP.type == SQUARE){
    currentP.type = LINE;
  }
  else if(currentP.type == ZI){
    currentP.type = SQUARE;
  }
  /**start the current piece at the middle of board**/
  if(currentP.type == SQUARE){
    currentP.coord1[0] = 3;
    currentP.coord1[1] = 0;
    currentP.coord2[0] = 4;
    currentP.coord2[1] = 0;
    currentP.coord3[0] = 3;
    currentP.coord3[1] = 1;
    currentP.coord4[0] = 4;
    currentP.coord4[1] = 1;
  }
  else if(currentP.type == LINE){
    currentP.coord1[0] = 3;
    currentP.coord1[1] = 0;
    currentP.coord2[0] = 3;
    currentP.coord2[1] = 1;
    currentP.coord3[0] = 3;
    currentP.coord3[1] = 2;
    currentP.coord4[0] = 3;
    currentP.coord4[1] = 3;    
  }
  else if(currentP.type == ZI){
    currentP.coord1[0] = 3;
    currentP.coord1[1] = 0;
    currentP.coord2[0] = 4;
    currentP.coord2[1] = 1;
    currentP.coord3[0] = 3;
    currentP.coord3[1] = 1;
    currentP.coord4[0] = 4;
    currentP.coord4[1] = 2;
  }
}

/**erase previous current piece locations on the board**/
void erasePrevious(){
  and_sr(~8);
  board[currentP.coord1[0]][currentP.coord1[1]] = 0;
  board[currentP.coord2[0]][currentP.coord2[1]] = 0;
  board[currentP.coord3[0]][currentP.coord3[1]] = 0;
  board[currentP.coord4[0]][currentP.coord4[1]] = 0;
  or_sr(8);
}
/**assign the current piece position on the board**/
void assignCurrent(){
  board[currentP.coord1[0]][currentP.coord1[1]] = 1;
  board[currentP.coord2[0]][currentP.coord2[1]] = 1;
  board[currentP.coord3[0]][currentP.coord3[1]] = 1;
  board[currentP.coord4[0]][currentP.coord4[1]] = 1;
}
/**draw the current piece on the LCD given its coordinates**/
void drawCurrent(){
  and_sr(~8);
  if(currentP.type == LINE){
  fillRectangle(currentP.coord1[0]*10+5, currentP.coord1[1]*10+5, 10, 10, COLOR_AQUAMARINE);
  fillRectangle(currentP.coord2[0]*10+5, currentP.coord2[1]*10+5, 10, 10, COLOR_AQUAMARINE);
  fillRectangle(currentP.coord3[0]*10+5, currentP.coord3[1]*10+5, 10, 10, COLOR_AQUAMARINE);
  fillRectangle(currentP.coord4[0]*10+5, currentP.coord4[1]*10+5, 10, 10, COLOR_AQUAMARINE);
  }
  else if(currentP.type == SQUARE){
  fillRectangle(currentP.coord1[0]*10+5, currentP.coord1[1]*10+5, 10, 10, COLOR_GREEN);
  fillRectangle(currentP.coord2[0]*10+5, currentP.coord2[1]*10+5, 10, 10, COLOR_GREEN);
  fillRectangle(currentP.coord3[0]*10+5, currentP.coord3[1]*10+5, 10, 10, COLOR_GREEN);
  fillRectangle(currentP.coord4[0]*10+5, currentP.coord4[1]*10+5, 10, 10, COLOR_GREEN);
  }
  else if(currentP.type == ZI){
  fillRectangle(currentP.coord1[0]*10+5, currentP.coord1[1]*10+5, 10, 10, COLOR_PURPLE);
  fillRectangle(currentP.coord2[0]*10+5, currentP.coord2[1]*10+5, 10, 10, COLOR_PURPLE);
  fillRectangle(currentP.coord3[0]*10+5, currentP.coord3[1]*10+5, 10, 10, COLOR_PURPLE);
  fillRectangle(currentP.coord4[0]*10+5, currentP.coord4[1]*10+5, 10, 10, COLOR_PURPLE);
  }
  __delay_cycles(50000);
  or_sr(8);
}

/**method to convert int to string**/
/**Source: www.learntosolveit.com/cprogramming/Ex_4.12_recursive_itoa.html**/
void itoa(int n, char s[]){
  static int i;
  if(n/10)
    itoa(n/10, s);
  else{
    i = 0;
    if(n < 0)
      s[i++] = '-';
  }
  s[i++] = abs(n) % 10 + '0';
  s[i] = '\0';
}

/**convert score and lines ints to string by calling itoa method and draw on LCD**/
void drawNumbers(){
  char res[100];
  itoa(score, res);
  drawString5x7(100, 80, res , screenColor, bgColor);
  itoa(lines, res);
  drawString5x7(100, 110, res , screenColor, bgColor);
}

/**draw letters, numbers, and margin**/
void drawScreen(){
  clearScreen(bgColor);
  fillRectangle(0, 0, 85, 5, screenColor);
  fillRectangle(0, 0, 5, 160,screenColor);
  fillRectangle(0, 155, 85, 5, screenColor);
  fillRectangle(85, 0, 5, 160, screenColor);
  fillRectangle(90, 0, 38, 160, bgColor);
  drawString5x7(95, 30, "BLOCK", COLOR_GOLD, bgColor);
  drawString5x7(95, 40, "GAME", COLOR_GOLD, bgColor);
  drawString5x7(90, 70, "Score:", screenColor, bgColor);
  drawString5x7(90, 100, "Lines:", screenColor, bgColor);
  drawNumbers();
}

/**draw board on LCD for every "coordinate"(location) in the array we show a rectangle of 10x10, and start location is i*10+5 and j*10+5**/
void drawBoard(){
  int i, j;
  for(i = 0; i < COLS; i++){
    for(j = 0; j < ROWS; j++){
      if(board[i][j] == 0){
	fillRectangle(i*10+5, j*10+5, 10, 10, bgColor);
      }
    }
  }
}

void makeLineSound(){
  buzzer_set_period(1000);
}
void makeButtonSound(){
  buzzer_set_period(4000);
}

/**swap method to "eliminate" the given row  on board**/
/**for every line cleared add two points to score, and update lines**/
void swap(int s){
  int i, j;
  for(j = s; j >=1; j--){
    for(i = 0; i < 8; i++){
      board[i][j] = board[i][j-1];
    }
  }
  score +=2;
  lines++;
}

/**check if a line can be cleared, if yes then call swap method to erase that line**/
/**make a sound every time that a line is cleared**/
void checkForLines(){
  int x, y;
   int count = 0;
   and_sr(~8);
   /**start at first row, compare all columns to count the pieces placed at row**/
   for(x = 0; x < 15; x++){
     count = 0;
     for(y = 0; y < 8; y++){
       if(board[y][x] != 0){
	 count++;
       }
     }
     /**if capacity of pieces at row x is filled, clear it**/
     if(count == 8){
       makeLineSound();
       swap(x);
     }
   }
   /**update numbers and board on LCD**/
   drawNumbers();
   drawBoard();
   or_sr(8);
}

/**if one piece is located at the top of board, the game is over**/
/**display on LCD a message**/
void checkForLost(){
  int i;
  for(i = 0; i < COLS; i++){
    if(board[i][0] == 1){
      drawString5x7(20, 80, "GAME OVER", COLOR_RED, bgColor);
      while(1);
    }
  }
}

void main()
{
  P1DIR |= GREEN_LED;		/**< Green led on when CPU on */	      
  P1OUT |= GREEN_LED;

  /**initialize LCD, switches and buzzer **/
  configureClocks();
  lcd_init();
  p2sw_init(15);
  buzzer_init();
  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8); /**< GIE on **/

  /**display the game screen**/
  drawScreen();
  /**initialize board and current piece, and display all on LCD**/
  init_board();
  currentP.type = SQUARE;
  init_currentPiece();
  drawCurrent();

  /**for loop to keep drawing or pause it**/
  for(;;) { 
    while (!redrawScreen) { /**< Pause CPU if screen doesn't need updating */
      P1OUT &= ~GREEN_LED;    /**< Green led off witHo CPU */
      or_sr(0x10);	      /**< CPU OFF */
    }
    P1OUT |= GREEN_LED;       /**< Green led on when CPU on */
    redrawScreen = 0;
  }
}

/** Watchdog timer interrupt handler. 30 interrupts/sec */
void wdt_c_handler()
{
  static short count = 0;
  P1OUT |= GREEN_LED;		      /**< Green LED on when cpu on */
  count ++;
  /**variable to read input of switches**/
  u_int switches = p2sw_read();
 
  if (count == 30) {
    /**if current piece reached the bottom, or another piece is down, place piece on board, check for lines to be cleared, check if game over, and create another piece**/
    if(currentP.coord4[1] == 14 || (currentP.type == LINE && board[currentP.coord4[0]][(currentP.coord4[1])+1] != 0) || (currentP.type == SQUARE && board[currentP.coord4[0]][(currentP.coord3[1])+1] != 0) || (currentP.type == SQUARE && board[currentP.coord3[0]][(currentP.coord4[1])+1] != 0 ) || (currentP.type == ZI && board[currentP.coord4[0]][(currentP.coord4[1])+1] != 0) || (currentP.type == ZI && board[currentP.coord3[0]][(currentP.coord3[1])+1] != 0)){
      assignCurrent();
      drawCurrent();
      checkForLines();
      buzzer_set_period(0);
      checkForLost();
      init_currentPiece();
    }
    /**if S1 pressed, move current piece to left by subtracting one to column coordinates and update board to display on LCD**/
    else if(~switches & 1 && currentP.coord1[0] >=1 && currentP.coord3[0] >=1 && board[(currentP.coord1[0])-1][(currentP.coord1[1])] == 0 &&  board[(currentP.coord3[0])-1][(currentP.coord3[1])] == 0 ){
      erasePrevious();
      makeButtonSound();
      currentP.coord1[0]--;
      currentP.coord2[0]--;
      currentP.coord3[0]--;
      currentP.coord4[0]--;
      drawBoard();
      assignCurrent();
      drawCurrent();
      buzzer_set_period(0);
    }
    /**if S2 pressed, move current piece to right by adding one to column coordinates and update board to display on LCD**/
    else if(~switches & 2 && currentP.coord2[0] <= 6 && currentP.coord4[0] <=6&& board[(currentP.coord2[0])+1][(currentP.coord2[1])] == 0 &&  board[(currentP.coord4[0])+1][(currentP.coord4[1])] == 0){
      erasePrevious();
      makeButtonSound();
      currentP.coord1[0]++;
      currentP.coord2[0]++;
      currentP.coord3[0]++;
      currentP.coord4[0]++;
      drawBoard();
      assignCurrent();
      drawCurrent();
      buzzer_set_period(0);
    }
    /**if S3 pressed, move current piece faster by adding two to row coordinates and update board to display on LCD**/
    else if(~switches & 4 && ((currentP.coord4[1])+2 < 12 ) && ((board[currentP.coord4[0]][currentP.coord4[1]+2]) == 0) && ((board[currentP.coord3[0]][currentP.coord3[1]+2]) == 0)){
      erasePrevious();
      makeButtonSound();
      currentP.coord1[1]+=2;
      currentP.coord2[1]+=2;
      currentP.coord3[1]+=2;
      currentP.coord4[1]+=2;
      drawBoard();
      assignCurrent();
      drawCurrent();
      buzzer_set_period(0);
    }
    /**if S4 pressed, pause game**/
    else if(~switches & 8){
      redrawScreen = 1;
    }
    /**else move down the current piece by adding one to row coordinates**/
    else{
      buzzer_set_period(2500);
      erasePrevious();
      currentP.coord1[1]++;
      currentP.coord2[1]++;
      currentP.coord3[1]++;
      currentP.coord4[1]++;
      drawBoard();
      assignCurrent();
      drawCurrent();
      buzzer_set_period(0);
    }
    count = 0;
  }
  P1OUT &= ~GREEN_LED;		    /**< Green LED off when cpu off */
}
