/*
 *   Copyright 2012 Jared Wallace
 *   This file is part of St. Somewhere.
 *
 *   St. Somewhere is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   St. Somewhere is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with St. Somewhere.  If not, see <http://www.gnu.org/licenses/>.
 *
*/
#include <iostream>
#include <cmath>
#include <bitset>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ncurses.h>
#include "calc.h"
#include <vector>

using namespace std;

const int INT_MAX = 999999999;
const int INT_MIN = -999999999;
/*
  I found this on the interwebs after itoa failed to work. It's so elegant I had to include it :)
  I have made some minor cosmetic adjustments.
  C++ version 0.4 std::string style "itoa":
  Contributions from Stuart Lowe, Ray-Yuan Sheu,
  Rodrigo de Salvo Braz, Luc Gallant, John Maloney
  and Brian Hunt
*/
string Calc::convertToString(int value, int base)
{

  string buf;
  buf.reserve(50);

  int quotient = value;
  do
  {
    buf += "0123456789ABCDEF"[abs(quotient%base)];
    quotient /= base;
  } while (quotient);
  if ( value < 0)
    buf += '-';
  reverse( buf.begin(), buf.end() );
  return buf;
}
/*
  This function accepts a string of numbers, entered by the user. It iterates over
  each character, adding the appropriate value of each place to a variable called
  sum. If at any point the conversion will lead to sum being greater than the
  limit, the operation stops and displays an overflow message. If the conversion
  completes successfully, it returns the integer value of the input in base 10.
*/
int Calc::convertToDecimal(string input)
{
  int length = input.length(), sum(0);
  for (int i=0; i<length; i++)
  {
    switch (input[i])
    {
      case '0':
        break;
      case '1':
        if (overflowCheck(sum, 1*pow(base, length-i-1), '+'))
          sum += 1*pow(base, length-i-1);
        else
        {
          overflowErrorMessage();
          return 0;
        }
        break;
      case '2':
        if (overflowCheck(sum, 2*pow(base, length-i-1), '+'))
          sum += 2*pow(base, length-i-1);
        else
        {
          overflowErrorMessage();
          return 0;
        }
        break;
      case '3':
        if (overflowCheck(sum, 3*pow(base, length-i-1), '+'))
          sum += 3*pow(base, length-i-1);
        else
        {
          overflowErrorMessage();
          return 0;
        }
        break;
      case '4':
        if (overflowCheck(sum, 4*pow(base, length-i-1), '+'))
          sum += 4*pow(base, length-i-1);
        else
        {
          overflowErrorMessage();
          return 0;
        }
        break;
      case '5':
        if (overflowCheck(sum, 5*pow(base, length-i-1), '+'))
          sum += 5*pow(base, length-i-1);
        else
        {
          overflowErrorMessage();
          return 0;
        }
        break;
      case '6':
        if (overflowCheck(sum, 6*pow(base, length-i-1), '+'))
          sum += 6*pow(base, length-i-1);
        else
        {
          overflowErrorMessage();
          return 0;
        }
        break;
      case '7':
        if (overflowCheck(sum, 7*pow(base, length-i-1), '+'))
          sum += 7*pow(base, length-i-1);
        else
        {
          overflowErrorMessage();
          return 0;
        }
        break;
      case '8':
        if (overflowCheck(sum, 8*pow(base, length-i-1), '+'))
          sum += 8*pow(base, length-i-1);
        else
        {
          overflowErrorMessage();
          return 0;
        }
        break;
      case '9':
        if (overflowCheck(sum, 9*pow(base, length-i-1), '+'))
          sum += 9*pow(base, length-i-1);
        else
        {
          overflowErrorMessage();
          return 0;
        }
        break;
      case 'a':
      case 'A':
        if (overflowCheck(sum, 10*pow(16, length-i-1), '+'))
          sum += 10*pow(16, length-i-1);
        else
        {
          overflowErrorMessage();
          return 0;
        }
        break;
      case 'b':
      case 'B':
        if (overflowCheck(sum, 11*pow(16, length-i-1), '+'))
          sum += 11*pow(16, length-i-1);
        else
        {
          overflowErrorMessage();
          return 0;
        }
        break;
      case 'c':
      case 'C':
        if (overflowCheck(sum, 12*pow(16, length-i-1), '+'))
          sum += 12*pow(16, length-i-1);
        else
        {
          overflowErrorMessage();
          return 0;
        }
        break;
      case 'd':
      case 'D':
        if (overflowCheck(sum, 13*pow(16, length-i-1), '+'))
          sum += 13*pow(16, length-i-1);
        else
        {
          overflowErrorMessage();
          return 0;
        }
        break;
      case 'e':
      case 'E':
        if (overflowCheck(sum, 14*pow(16, length-i-1), '+'))
          sum += 14*pow(16, length-i-1);
        else
        {
          overflowErrorMessage();
          return 0;
        }
        break;
      case 'f':
      case 'F':
        if (overflowCheck(sum, 15*pow(16, length-i-1), '+'))
          sum += 15*pow(16, length-i-1);
        else
        {
          overflowErrorMessage();
          return 0;
        }
        break;
      default:
        cerr << "Error in convertToDecimal switch";
    }
  }
  return sum;
}

void Calc::add()
{
  int elements = value.size();
  //check for overflow first
  if (elements == 1)
    return;
  if (!overflowCheck(value.back(), value.at(elements-2), '+'))
  {
    overflowErrorMessage();
    return;
  }
  //add the bottom two values in the stack and store the result in the second to last spot
  value.at(elements-2) += value.back();
  display.at(elements-2) = convertToString(value.at(elements-2), base);
  //we want binary in two's complement
  if (base == 2)
    display.back() = convertDB(value.back());
  //move the stack down
  value.pop_back();
  display.pop_back();
}

void Calc::subtract()
{
  int elements = value.size();
  if (elements == 1)
    return;
  if (!overflowCheck(value.back(), value.at(elements-2), '-'))
  {
    overflowErrorMessage();
    return;
  }
  //subtract the bottom number from the one above it and store in the second to last spot
  value.at(elements-2) = value.at(elements-2) - value.back();
  display.at(elements-2) = convertToString(value.at(elements-2), base);
  //we want binary in two's complement
  if (base == 2)
    display.at(elements-2) = convertDB(value.at(elements-2));
  //move the stack down
  value.pop_back();
  display.pop_back();
}


void Calc::mult()
{
  int elements = value.size();
  if (elements == 1)
    return;
  if (!overflowCheck(value.back(), value.at(elements-2), '*'))
  {
    overflowErrorMessage();
    return;
  }
  //multiply the bottom two numbers in the stack and store in the second to last spot
  value.at(elements-2) *= value.back();
  display.at(elements-2) = convertToString(value.at(elements-2), base);
  //we want binary in two's complement
  if (base == 2)
    display.at(elements-2) = convertDB(value.at(elements-2));
  //move the stack down
  value.pop_back();
  display.pop_back();
}

void Calc::div()
{
  int elements = value.size();
  if (elements == 1)
    return;
  if (value.back() == 0)
  {
    overflowErrorMessage();
    return;
  }
  //Divide the bottom number into the one above it(gives quotient) and store into the second to last spot
  value.at(elements-2) = value.at(elements-2)/value.back();
  display.at(elements-2) = convertToString(value.at(elements-2), base);
  //we want binary in two's complement
  if (base == 2)
    display.at(elements-2) = convertDB(value.at(elements-2));
  //move the stack down
  value.pop_back();
  display.pop_back();
}

void Calc::mod()
{
  int elements = value.size();
  if (elements == 1)
    return;
  if (value.back() == 0)
  {
    overflowErrorMessage();
    return;
  }
  //Finds the remainder of the bottom number divided into the one above and store into the second to last spot
  value.at(elements-2) = value.at(elements-2)%value.back();
  display.at(elements-2) = convertToString(value.at(elements-2), base);
  //we want binary in two's complement
  if (base == 2)
    display.at(elements-2) = convertDB(value.at(elements-2));
  //move the stack down
  value.pop_back();
  display.pop_back();
}

void Calc::backslash()
{
  int elements=value.size();
  if (elements == 0)
    return;
  value.pop_back();
  display.pop_back();
}

void Calc::repeat()
{
  int elements = value.size();
  value.push_back(value.at(elements-1));
  display.push_back(display.at(elements-1));
}

/*
  Accepts some decimal value, and returns that number in two's complement binary representation as a string.
  Part of this function was written by Matthew Wallace (the two lines before return)
*/
string Calc::convertDB(int initial)
{

  bitset<BITS> result;
  int n(0);
  double numBits;
  bool negative(false);
  string intermediate, final;

  if (initial < 1)
  {
    initial = abs(initial);
    negative = true;
  }

  numBits = log10(double(initial))/ log10(double(2));

  if (negative)
  {
    numBits += 2;
    result = initial;
    result.flip();
    bitset<BITS> addOne = 1;
    addBinary(result, addOne);
  }
  else
  {
    numBits += 2;
    result = initial;
    result.set(numBits-1,0);
  }
  intermediate=result.to_string<char,char_traits<char>,allocator<char> >();
  final = intermediate.substr ((BITS-numBits+1), numBits+1);
  return final;
}
/*
 Accepts two bitsets and adds them together, storing the result in the first bitset passed to it.
*/
void Calc::addBinary(bitset<BITS> &result, bitset<BITS> second)
{
  int carry = 0;

  for (int i=0; i<BITS; i++)
  {
    switch (result[i]+second[i] + carry)

    {
      case 0:
        carry = 0;
        break;
      case 1:
        result.set(i);
        carry = 0;
        break;
      case 2:
        result.reset(i);
        carry = 1;
        break;
      case 3:
        result.set(i);
        carry = 1;
        break;
      default:
        cout << "error in addBinary switch";
    }
  }
}

void Calc::setStack(int rows)
{
  stack = rows;
}

void Calc::setInputValue(string input)
{
  bool negative = false;
  //Now we handle negation
  if (input[0] == '-')
  {
    switch (base)
    {
      case 2:
        input = input.substr(1, input.length()-1); //remove neg sign
        value.push_back(convertToDecimal(input)); //convert to abs value in decimal
        value.back() *= -1; //negates the decimal value
        display.push_back(convertDB(value.back())); //converts the display value into two's complement
        return;
        break;
      default:
        input = input.substr(1, input.length()-1); //remove neg sign
        negative = true;
        break;
    }
  }

  switch (base)
  {
    case 10:
      {
        value.push_back(convertToDecimal(input.c_str()));
        display.push_back(convertToString(value.back(), 10));
        if (negative)
        {
          display[0].insert(0,1,'-');
          value[0] *= -1;
        }
      }
      break;
    case 2:
      value.push_back(convertToDecimal(input));
      display.push_back(input);
      display.back().insert(0,1,'0'); //two's complement
      break;
    case 8:
      value.push_back(convertToDecimal(input));
      display.push_back(input);
      if (negative)
        {
          display[0].insert(0,1,'-');
          value[0] *= -1;
        }
      break;
    case 16:
      value.push_back(convertToDecimal(input));
      display.push_back(input);
      if (negative)
        {
          display[0].insert(0,1,'-');
          value[0] *= -1;
        }
      break;
    default:
      cerr << "Error in setValue switch";
  }
}

char Calc::getBase()
{
  return base;
}

void Calc::setBase(char bse)
{
  if (bse=='d' || bse=='o' || bse=='h' || bse=='b')
  {
    switch (bse)
    {
      case 'd':
        base = 10;
        break;
      case 'b':
        base = 2;
        break;
      case 'h':
        base = 16;
        break;
      case 'o':
        base = 8;
        break;
      default:
        cerr << "Error in base assignment switch";
    }
  }
  else
    cerr << "invalid selection";
}

void Calc::setDisplay(int pos, string input)
{
  //takes care of two's complement
  if (base == 2)
  {
    if (input[0] == '-')
      display.at(pos) = convertDB(value.at(pos));
    else
    {
      display.at(pos) = input;
      display.at(pos).insert(0,1,'0');
    }
  }
  else
    display.at(pos) = input;
}

vector<string> Calc::getDisplay()
{
  return display;
}

string Calc::getDisplayValue(int pos)
{
  return display.at(pos);
}

void Calc::setValue(int pos, int input)
{
  value.at(pos) = input;
}

int Calc::getValue(int pos)
{
  return value.at(pos);
}

/*
 * The main difference between this function and the one that follows is
 * that this function checks the decimal value for validity and the other
 * one is checking for invalid characters in the string.
*/
bool Calc::overflowCheck(int value1, int value2, char operand)
{
  switch (operand)
  {
    case '+':
      if (value1>0 && value2>0)
      {
        if (value1>(INT_MAX-value2))
          return false;
      }
      else if (value1<0 && value2<0)
      {
        if (value1 < (INT_MIN-value2))
          return false;
      }
      break;
    case '-':
      if (value1<0 && value2>0)
      {
        if (value1 < (INT_MIN+value2))
          return false;
      }
      break;
    case '/':
      if (value2 == 0)
        return false;
      break;
    case '*':
      if (value1>0 && value2>0) //both values positive
      {
        if (value1 > (INT_MAX/value2))
          return false;
      }
      else if (value1>0 && value2<0) //value1 positive, value2 negative
      {
        if (value2 < (INT_MIN/value1))
          return false;
      }
      else if (value1<0 && value2>0) //value1 negative, value2 positive
      {
        if (value1 < (INT_MIN/value2))
          return false;
      }
      else if (value1<0 && value2<0) //both values negative
      {
        if (value2 < (INT_MAX/value1))
          return false;
      }
      break;
    default:
      cerr << "Error in overflow check switch";
  }
  return true;
}


bool Calc::inputCheck(string input)
{


    switch (base)
    {
      case 10:
        for (int i=0; i<input.length(); i++)
        {
          if (!isdigit(input[i]) && input[i] != '-')
            return false;
        }
        break;
      case 2:
        for (int i=0; i<input.length(); i++)
        {
          if (input[i] == '0' || input[i] == '1' || input[i] == '-')
            return true;
          else
            return false;
        }
        break;
      case 8:
        for (int i=0; i<input.length(); i++)
        {
          if (input[i] == '8' || input[i] == '9' || !isdigit(input[i]) && input[i] != '-')
            return false;
        }
        break;
      case 16:
        for (int i=0; i<input.length(); i++)
        {
          if (input[i] == 'A' || input[i] == 'B' || input[i] == 'C' || input[i] == 'D' || input[i] == 'E'
             || input[i] == 'F' || isdigit(input[i]) || input[i] == '-')
              return true;
          else
            return false;
        }
        break;
      default:
        cerr << "Error in inputCheck switch";
    }
    return true;
}

void Calc::overflowErrorMessage()
{
  attron(A_STANDOUT);
  mvprintw(3,1, "Overflow. Input too large.");
  mvprintw(4,1, "Press <enter> to continue.");
  attroff(A_STANDOUT);
  refresh();
  cin.get();
  move(3,1);
  clrtoeol();
  move(4,1);
  clrtoeol();
}

