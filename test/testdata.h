#pragma once

#include <glm/common.hpp>
#include <vector>
#include "models.h"

const glm::vec2 VEC2 = {12.0, 32.0};
const Mat4f MAT4F = {{1.0, -2.0},
                     {3.0, -4.0},
                     {5.0, -6.0},
                     {7.0, -8.0}};
const std::vector<uint16_t> INTS = {1, 2, 3, 4, 5, 6, 7, 8, 9};

static const Tag TAG = {123, "hello world"};
const Tag TAG_SHORT = {2, "_"};

const DictA DICT_PLAIN =
        {{{100, "hundred"},
          {5, "five"},
          {3, "three"}}};

const DictB<std::vector<float>> DICT_K =
        {{{{1.0, 2.0}, "three"},
                 {{4.0, 5.0} , "six"},
                 {{7.0, 8.0}, "nine"}}};

const std::pair<uint32_t, std::string> PAIR = {1, "one"};
const std::vector<std::pair<uint32_t, std::string>> PAIRS =
        {{1, "one"}, {2, "two"}, {4, "four"}};

