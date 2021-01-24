#pragma once

#include <vector>

namespace parser::internal
{
template <typename T>
struct Stack : public std::vector<T>
{
    using base_type = std::vector<T>;
    void push(const T& token) { base_type::emplace_back(token); };
    const T& top() const { return base_type::back(); };

    T pop() {
        auto result = base_type::back();
        base_type::pop_back();
        return result;
    }
};
} // namespace parser::internal