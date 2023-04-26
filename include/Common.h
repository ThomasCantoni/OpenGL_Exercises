#pragma once
#include <iostream>
#include <stdexcept>

#define DIE_ON_ERROR(VALUE, MSG) if (VALUE == 0) { std::cout << MSG << "\n"; throw std::runtime_error(MSG); }
#define DIE_ON_NULL(VALUE, MSG) if (VALUE == NULL) { std::cout << MSG << "\n"; throw std::runtime_error(MSG); }
#define DIE(MSG) { std::cout << MSG << "\n"; throw std::runtime_error(MSG); }

struct Color {
    float R;
    float G;
    float B;
    float A;
};