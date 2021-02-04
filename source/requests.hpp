#pragma once
#include <cstddef>

namespace requests {
    void request();
    size_t buffer_writer(char*, size_t, size_t, void*);
}