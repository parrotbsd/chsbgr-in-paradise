#ifndef CALC_H
#define CALC_H
#include <string>
#include <bitset>
#include <vector>

using namespace std;

const int BITS = 52;

class Calc
{
  private:
    int base;
    vector<string> display;
    vector<int> value;
    int stack;
    int convertToDecimal(string);
    string convertDB(int);
    void addBinary(bitset<BITS>&, bitset<BITS>);
    bool overflowCheck(int, int, char);
    void overflowErrorMessage();
  public:
    string convertToString(int, int);
    void initialize();
    bool inputCheck(string);
    void setStack(int);
    void setInputValue(string);
    void setValue(int, int);
    int getValue(int);
    void setBase(char);
    char getBase();
    void setDisplay(int, string);
    string getDisplayValue(int);
    vector<string> getDisplay();
    void add();
    void subtract();
    void mult();
    void div();
    void mod();
    void backslash();
    void repeat();
};

#endif
