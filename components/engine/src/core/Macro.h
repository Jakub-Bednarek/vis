//
// Created by BlackFlage on 09.10.2021.
//

#ifndef MAIN_MACRO_H
#define MAIN_MACRO_H
#include <iostream>

#define ASSERT(x, message) { if(!(x)) { std::cout << "Assertion failed! Message: " << message << '\n'; } }

#endif //MAIN_MACRO_H
