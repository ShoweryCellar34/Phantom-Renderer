#include <PR/error.hpp>

#include <string>

namespace PR {
    error::error(const std::string& message) : m_message(message) {
    }

    const char* error::what() const throw() {
        return m_message.c_str();
    }
}
