#pragma once

#include <exception>
#include <string>

namespace PR {
    class error : std::exception {
    private:
        std::string m_message;

    public:
        error(const std::string& message);

        const char* what() const throw();
    };
}
