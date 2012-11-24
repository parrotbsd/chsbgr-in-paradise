/*
 * @author: Jared Wallace
 * @date: 11-21-2012
 *
 * St.Somewhere Multi-function RPN Calculator Version 1.1
 *
 * This program is a multi-function RPN calculator.
 * See the README for detailed information on compiling and running,
 * as well as the limitations of the program.
*/
#include <iostream>
#include <ncurses.h>
#include <string>
#include <algorithm>
#include "calc.h"


using namespace std;

void displayScreen(int, int, Calc&, WINDOW*, WINDOW*);

int main()
{
  WINDOW *stackwin, *infowin;
  initscr();
  raw();
  noecho();
  keypad(stdscr, TRUE);
  int ch, row, col;
  string input = "";
  input.reserve(50);
  Calc calc;
  //set default base to decimal
  calc.setBase('d');
  //initialize our data arrays
  calc.initialize();
  //get the console size
  getmaxyx(stdscr, row, col);
  //initialize our windows
  stackwin = newwin(row, 30, 1, 41);
  infowin = newwin(row, 40, 1, 1);

  do
  {
    displayScreen(row, col, calc, stackwin, infowin);
    wrefresh(stackwin);
    refresh();
    wrefresh(infowin);
    ch = getch();
    switch (ch)
    {
      case 10: //enter
        //next two lines put another copy of the value in register 1 onto the stack
        if (input == "")
          input = calc.getDisplay(0);
        if (calc.inputCheck(input))
          calc.setInputValue(input);
        else
        {
          attron(A_STANDOUT);
          mvwprintw(infowin, row-1, 1, "Invalid input, check current base. Press <enter> to continue.");
          attroff(A_STANDOUT);
          refresh();
          cin.get();
          wmove(infowin, row-1, 1);
          clrtoeol();
        }
        //reset input for the next entry
        input = "";
        wmove(stackwin, row-1, 1);
        clrtoeol();
        break;
      case 92: //backslash
        for (int i=0; i<3; i++)
        {
          if (calc.getValue(i+1))
          {
            calc.setValue(i, calc.getValue(i+1));
            calc.setDisplay(i, calc.getDisplay(i+1));
          }
          else
          {
            calc.setValue(i, 0);
            calc.setDisplay(i, "");
          }
        }
        break;
      case 43: //plus
        /*
         * The next two lines allow us to immediately hit an operator after we enter the operand,
         * without having to press enter in-between.
         * These lines are present for each operator(+,-,/,*,%).
        */
        if (input != "")
          calc.setInputValue(input);
        calc.add();
        displayScreen(row, col, calc, stackwin, infowin);
        wmove(stackwin, row-1, 1);
        clrtoeol();
        input = "";
        break;
      case 45://subtract
        if (input != "")
          calc.setInputValue(input);
        calc.subtract();
        displayScreen(row, col, calc, stackwin, infowin);
        wmove(stackwin, row-1, 1);
        clrtoeol();
        input = "";
        break;
      case 42://multiplication
        if (input != "")
          calc.setInputValue(input);
        calc.mult();
        displayScreen(row, col, calc, stackwin, infowin);
        wmove(stackwin, row-1, 1);
        clrtoeol();
        input = "";
        break;
      case 47://division
        if (input != "")
          calc.setInputValue(input);
        calc.div();
        displayScreen(row, col, calc, stackwin, infowin);
        wmove(stackwin, row-1, 1);
        clrtoeol();
        input = "";
        break;
      case 37://modulo
        if (input != "")
          calc.setInputValue(input);
        calc.mod();
        displayScreen(row, col, calc, stackwin, infowin);
        wmove(stackwin, row-1, 1);
        clrtoeol();
        input = "";
        break;
      case 110:
        input.insert(0,1,'-');
        mvwprintw(stackwin, row-1, col-40-(input.length()), "%s", input.c_str());
        break;
      /*
       *To set the base, you press the lower case letters.
       *When entering hex, you need to use uppercase letters.
      */
      case 111://octal
        calc.setBase('o');
        //converts the stack to the relevant base
        for (int i=0; i<4; i++)
          calc.setDisplay(i, calc.itoa(calc.getValue(i),8));
        break;
      case 98://binary
        calc.setBase('b');
        for (int i=0; i<4; i++)
          calc.setDisplay(i, calc.itoa(calc.getValue(i),2));
        break;
      case 100://decimal
        calc.setBase('d');
        for (int i=0; i<4; i++)
          calc.setDisplay(i, calc.itoa(calc.getValue(i),10));
        break;
      case 104://hex
        calc.setBase('h');
        for (int i=0; i<4; i++)
          calc.setDisplay(i, calc.itoa(calc.getValue(i),16));
        break;
      default:
        input+=ch;
        mvwprintw(stackwin, row-1, col-(input.length()), "%s", input.c_str());
    }
    refresh();
  }while(ch != 113);

  delwin(stackwin);
  delwin(infowin);
  endwin();
  return 0;
}

void displayScreen(int row, int col, Calc& calc, WINDOW* stackwin, WINDOW* infowin)
{
  //draw our information display and a list of commands
  attron(A_BOLD);
  mvwprintw(infowin, row-21, col/3, "St.Somewhere -- Multifunction RPN");
  mvwprintw(infowin, row-18, 1, "Commands:");
  attroff(A_BOLD);
  mvwprintw(infowin, row-16, 1, "Quit               : q");
  mvwprintw(infowin, row-15, 1, "Change to Decimal  : d");
  mvwprintw(infowin, row-14, 1, "Change to Binary   : b");
  mvwprintw(infowin, row-13, 1, "Change to Hex      : h");
  mvwprintw(infowin, row-12, 1, "Change to Octal    : o");
  mvwprintw(infowin, row-11, 1, "Addition           : +");
  mvwprintw(infowin, row-10, 1, "Subtraction        : -");
  mvwprintw(infowin, row-9, 1, "Multiplication     : *");
  mvwprintw(infowin, row-8, 1, "Division           : /");
  mvwprintw(infowin, row-7, 1, "Modulo             : %%");
  mvwprintw(infowin, row-6, 1, "Negation           : n");
  mvwprintw(infowin, row-5, 1, "Clear              : \\");
  attron(A_BOLD);
  mvwprintw(infowin, row-20, 1, "Current Base:");
  attroff(A_BOLD);
  switch (calc.getBase())
  {
    case 10:
      wprintw(infowin, " Decimal");
      clrtoeol();
      break;
    case 8:
      wprintw(infowin, " Octal");
      clrtoeol();
      break;
    case 16:
      wprintw(infowin, " Hex");
      clrtoeol();
      break;
    case 2:
      wprintw(infowin, " Binary");
      clrtoeol();
      break;
    default:
      cerr << "Error in infoDisplay switch";
  }
  wmove(stackwin, row-6, 1);
  clrtoeol();
  wmove(stackwin, row-5, 1);
  clrtoeol();
  wmove(stackwin, row-4, 1);
  clrtoeol();
  wmove(stackwin, row-3, 1);
  clrtoeol();
  mvwprintw(stackwin, row-6, 1, "%s", calc.getDisplay(3).c_str());
  mvwprintw(stackwin, row-5, 1, "%s", calc.getDisplay(2).c_str());
  mvwprintw(stackwin, row-4, 1, "%s", calc.getDisplay(1).c_str());
  mvwprintw(stackwin, row-3, 1, "%s", calc.getDisplay(0).c_str());
  wmove(stackwin, row-2, 1);
  clrtoeol();
  mvwprintw(stackwin, row-2, col-60, "--------------------");
  wmove(stackwin, row-1, col-61);
}
