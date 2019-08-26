#ifndef WIN_ERROR_CATEGORY_H
#define WIN_ERROR_CATEGORY_H

#include <string>
#include <system_error>
#include "../win_errc.cpp"

namespace wse {

    /// Singleton class used for identifying distinct categories of error types
    const class win_error_category : public std::error_category {
        [[nodiscard]] const char* name() const noexcept override;
        [[nodiscard]] std::string message(int ev) const override;
    } winSystemError{};
    
}

#endif // WIN_ERROR_CATEGORY_H