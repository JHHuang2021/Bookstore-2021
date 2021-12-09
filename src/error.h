#ifndef ERROR_H
#define ERROR_H
#include <exception>
class Error : public std::exception {
   public:
    Error() = default;
    const char* what() const noexcept override;
};
#endif