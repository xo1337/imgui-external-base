#pragma once

#include <includes.hpp>

namespace util {

    std::string rand_str(std::size_t length) {
        static std::string characters = xorstr_("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<std::size_t> distribution(0, sizeof(characters) - 2);

        std::string string;
        for (std::size_t i = 0; i < length; ++i)
            string += characters[distribution(generator)];

        return string;
    }

}