/** \file lcddemo.c
 *  \brief A simple demo that draws a string and circle
 */

#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

/** Initializes everything, clears the screen, draws "hello" and the circle */
main()
{
  configureClocks();
  lcd_init();
  u_char width = screenWidth, height = screenHeight;

  clearScreen(COLOR_BLACK);

  // drawString5x6(0,0, "Karina!", COLOR_RED, COLOR_BLACK);

  //fillRectangle(50,50, 30,30, COLOR_ORANGE);
  
}
