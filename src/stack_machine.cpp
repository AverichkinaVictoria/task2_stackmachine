////////////////////////////////////////////////////////////////////////////////
// Module Name:  stack_machine.h/cpp
// Authors:      Sergey Shershakov
// Version:      0.2.0
// Date:         23.01.2017
//
// This is a part of the course "Algorithms and Data Structures" 
// provided by  the School of Software Engineering of the Faculty 
// of Computer Science at the Higher School of Economics.
////////////////////////////////////////////////////////////////////////////////

#include "stack_machine.h"

#include <vector>
#include <sstream>
#include <iostream>
#include <stdlib.h>

namespace xi {

//==============================================================================
// Free functions -- helpers
//==============================================================================



//==============================================================================
// class PlusOp
//==============================================================================


int PlusOp::operation(char op, int a, int b, int /*c*/) // < just commented unused argument. This does not affect the code anyhow.
{
    if(op != '+')
        throw std::logic_error("Operation other than Plus (+) are not supported");

    // here we just ignore unused operands
    return a + b;
}

IOperation::Arity PlusOp::getArity() const
{
    return arDue;
}


//==============================================================================
// class MinusOp
//==============================================================================

int MinusOp::operation(char op, int a, int b, int /*c*/)
{
    if(op != '-')
        throw std::logic_error("Operation other than Minus (-) are not supported");
    return a - b;
}

IOperation::Arity MinusOp::getArity() const
{
    return arDue;
}

//==============================================================================
// class OrOp
//==============================================================================

int OrOp::operation(char op, int a, int b, int /*c*/)
{
    if(op != '|')
        throw std::logic_error("Operation other than Or (|) are not supported");
    return a | b;
}
IOperation::Arity OrOp::getArity() const
{
    return arDue;
}


//==============================================================================
// class SigChangeOp
//==============================================================================

int SigChangeOp::operation(char op, int a, int /*b*/, int /*c*/)
{
    if(op != '!')
        throw std::logic_error("Operation other than SignChange (!) are not supported");
    return -a;
}
IOperation::Arity SigChangeOp::getArity() const
{
    return arUno;
}



//==============================================================================
// class StackMachine
//==============================================================================


void StackMachine::registerOperation(char symb, IOperation *oper)
{
    SymbolToOperMapConstIter i = _opers.find(symb);
    if(i == _opers.end())
        _opers[symb] = oper;
    else
        throw std::logic_error("This operation was already registered!");
}

IOperation *StackMachine::getOperation(char symb)
{
    SymbolToOperMapConstIter i = _opers.find(symb);
    if (i == _opers.end())
        return nullptr;
    else
        return i -> second;
}

int StackMachine::calculate(const std::string &expr, bool clearStack)
{
    if (clearStack)
        _s.clear();

    std::string split_str = expr;   //that string will be used in work
    split_str += ' ';   //for right split
    std::vector<std::string> nums;  //will save all numbers and operations
    std::string extra="";   //for creation the number
    for (int i = 0; i < split_str.length(); i++)    //we are collecting all char symbols from ' ' to ' ' and after that push the result to vector of numbers and operations
    {
        if (split_str[i] != ' ')
            extra += split_str[i];
        else if (split_str[i] == ' ')
        {
            nums.push_back(extra);
            extra = "";
        }
    }
    for (std::string numStr : nums)
    {
        int number;
        bool flag = true;
        try
        {
            number = std::stoi(numStr);
        }
        catch(std::invalid_argument)
        {
            flag = false;
        }
        if(flag)    //if it is the number - push to the stack, else - calculate the operation or throw the exception if it's not possible
            _s.push(number);
        else
        {
            char oper = *numStr.c_str();
            int result;
            IOperation *newOper = getOperation(oper);
            if(numStr.length() != 1)
                throw std::logic_error("Wrong operation!");
            else if(newOper == nullptr)
                throw std::logic_error("There is no such operation!");
            else
            {
                if (newOper->getArity() == IOperation::arUno)
                {
                    int first = _s.pop();
                    result = newOper->operation(oper, first);
                }
                else if(newOper->getArity() == IOperation::arDue)
                {
                    int second = _s.pop();
                    int first = _s.pop();
                    result = newOper->operation(oper, first, second);
                }
                else if(newOper->getArity() == IOperation::arTre)
                {
                    int third = _s.pop();
                    int second = _s.pop();
                    int first = _s.pop();
                    result = newOper->operation(oper, first, second, third);
                }
                else
                    break;
                _s.push(result);
            }
        }
    }
    return _s.top();
}

} // namespace xi
