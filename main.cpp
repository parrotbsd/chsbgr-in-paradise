/*
 * @author: Jared Wallace
 * @date: 11-21-2012
 *
 * St.Somewhere Multi-function RPN Calculator Version 1.0
 *
 * This program is a multi-function RPN calculator.
 * See the README for detailed information on compiling and running,
 * as well as the limitations of the program.
 *
 * I should note that the visual design of my program owes quite a bit
 * to Orpie, my personal favorite RPN calculator.
*/
#include <iostream>
#include <ncurses.h>
#include <string>
#include <algorithm>
#include "calc.h"


using namespace std;

void displayScreen(int, int, Calc&);

int main()
{
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
  //get the console size
  getmaxyx(stdscr, row, col);
  //set the size of the stack
  calc.setStack(row-1);
  //initialize our data arrays
  calc.initialize();

  do
  {
    displayScreen(row, col, calc);
    refresh();
    ch = getch();
    switch (ch)
    {
      case 10: //enter
        //If no number is entered, place another copy of register 1 on the stack
        if (input == "")
          input = calc.getDisplay(0);
        //if input is kosher, send it to be assigned to the stack
        if (calc.inputCheck(input))
          calc.setInputValue(input);
        else
        {
          attron(A_STANDOUT);
          mvprintw(3, 1, "Invalid input.");
          mvprintw(4, 1, "Check base.");
          mvprintw(5, 1, "Press <enter> to continue.");
          attroff(A_STANDOUT);
          refresh();
          cin.get();
          move(3, 1);
          clrtoeol();
          move(4, 1);
          clrtoeol();
          move(5, 1);
          clrtoeol();
        }
        //reset input for the next entry
        input = "";
        move(row-1, 40);
        clrtoeol();
        break;
      case 92: //backslash
        for (int i=0; i<row-2; i++)
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
        displayScreen(row, col, calc);
        move(row-1, 40);
        clrtoeol();
        input = "";
        break;
      case 45://subtract
        if (input != "")
          calc.setInputValue(input);
        calc.subtract();
        displayScreen(row, col, calc);
        move(row-1, 40);
        clrtoeol();
        input = "";
        break;
      case 42://multiplication
        if (input != "")
          calc.setInputValue(input);
        calc.mult();
        displayScreen(row, col, calc);
        move(row-1, 40);
        clrtoeol();
        input = "";
        break;
      case 47://division
        if (input != "")
          calc.setInputValue(input);
        calc.div();
        displayScreen(row, col, calc);
        move(row-1, 40);
        clrtoeol();
        input = "";
        break;
      case 37://modulo
        if (input != "")
          calc.setInputValue(input);
        calc.mod();
        displayScreen(row, col, calc);
        move(row-1, 40);
        clrtoeol();
        input = "";
        break;
      case 110://negation
        input.insert(0,1,'-');
        mvprintw(row-1, 80-(input.length()), "%s", input.c_str());
        break;
      /*
       *To set the base, you press the lower case letters.
       *When entering hex, you need to use uppercase letters.
      */
      case 111://octal
        calc.setBase('o');
        //converts the stack to the relevant base
        for (int i=0; i<row-1; i++)
        {
          //This makes sure we don't try to convert empty registers
          if (calc.getValue(i))
            calc.setDisplay(i, calc.convertToString(calc.getValue(i),8));
        }
        break;
      case 98://binary
        calc.setBase('b');
        for (int i=0; i<row-1; i++)
        {
          if (calc.getValue(i))
            calc.setDisplay(i, calc.convertToString(calc.getValue(i),2));
        }
        break;
      case 100://decimal
        calc.setBase('d');
        for (int i=0; i<row-1; i++)
        {
          if (calc.getValue(i))
            calc.setDisplay(i, calc.convertToString(calc.getValue(i),10));
        }
        break;
      case 104://hex
        calc.setBase('h');
        for (int i=0; i<row-1; i++)
        {
          if (calc.getValue(i))
            calc.setDisplay(i, calc.convertToString(calc.getValue(i),16));
        }
        break;
      default:
        input+=ch;
        mvprintw(row-1, 80-(input.length()), "%s", input.c_str());
    }
    refresh();
  }while(ch != 113);


  endwin();
  return 0;
}

void displayScreen(int row, int col, Calc& calc)
{
  //draw our information display and a list of commands
  attron(A_BOLD);
  mvprintw(1, 1, "St.Somewhere -- Multifunction RPN");
  mvprintw(row-18, 1, "Commands:");
  attroff(A_BOLD);
  mvprintw(row-16, 1, "Quit               : q");
  mvprintw(row-15, 1, "Change to Decimal  : d");
  mvprintw(row-14, 1, "Change to Binary   : b");
  mvprintw(row-13, 1, "Change to Hex      : h");
  mvprintw(row-12, 1, "Change to Octal    : o");
  mvprintw(row-11, 1, "Addition           : +");
  mvprintw(row-10, 1, "Subtraction        : -");
  mvprintw(row-9, 1, "Multiplication     : *");
  mvprintw(row-8, 1, "Division           : /");
  mvprintw(row-7, 1, "Modulo             : %%");
  mvprintw(row-6, 1, "Negation           : n");
  mvprintw(row-5, 1, "Clear              : \\");
  attron(A_BOLD);
  mvprintw(row-20, 1, "Current Base:");
  attroff(A_BOLD);
  switch (calc.getBase())
  {
    case 10:
      printw(" Decimal");
      clrtoeol();
      break;
    case 8:
      printw(" Octal");
      clrtoeol();
      break;
    case 16:
      printw(" Hex");
      clrtoeol();
      break;
    case 2:
      printw(" Binary");
      clrtoeol();
      break;
    default:
      cerr << "Error in infoDisplay switch";
  }
  //draws a vertical line starting at (1,39) and going down to row-3
  mvvline(1, 39, 124, row-3);
  //draws our stack numbers
  int count = 1;
  for (int i=row-3; i>0; i--)
  {
    mvprintw(i, 40, "%d:", count);
    count++;
  }
  //clears stack area of the old values
  for (int i=3; i<row; i++)
  {
    move(row-i, 43);
    clrtoeol();
  }
  //draw the values in the stack
  for (int i=0; i<row-1; i++)
    mvprintw(row-i-3, 80-calc.getDisplay(i).length(), "%s", calc.getDisplay(i).c_str());
  //mvprintw(row-3, 60-calc.getDisplay(0).length(), "%s", calc.getDisplay(0).c_str());
  //draws the horizontal line
  mvhline(row-2, 1, 95, 80);
  move(row-1, 81);
}
