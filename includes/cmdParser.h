/**
 * @file cmdParser.h
 * @author sami dhiab sami@theion.de
 * @brief
 * @version 0.1
 * @date 2022-08-15
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include <iostream>
#include <sstream>
#include <string>
class cmdParser
{
private:
    int args = 1;
    char* values[10]; //assume 5-1=4 args
    char* AppName = values[5];
public:
    cmdParser(int args, char* args_values[5]) : args(args) {
        std::cout << "getting command line args" << std::endl;
        for (int i = 0; i < args; i++)
        {
            std::cout << "passing array content internally: "<< args_values[i]<< std::endl;
            values[i] = args_values[i];
        }
    }
    double parse_arg_float(std::string Sarg)
    {
        std::cout << "getting value arg " << Sarg << std::endl;
        for (int arg = 0; arg < args; arg++)
        {
            if (Sarg == values[arg]) // we found this arg
            {
                std::cout << "value arg " << Sarg << " is " << values[arg + 1] << std::endl;
                return std::stod(std::string(values[arg + 1]));// return its value
            }
        }
        return 0;
    }
    ~cmdParser() {};
};




