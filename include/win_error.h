#ifndef WIN_ERROR_H
#define WIN_ERROR_H

#include <system_error>
#include "win_errc.h"
#include "win_error_category.h"

namespace std {

    /// Comply with concepts syntax.
    ///
    /// Returns true for std::is_error_code<wse::win_errc>::value.
    template<>
    struct is_error_code_enum<wse::win_errc> : true_type{};

    /// Convert a wse::win_errc to a std::error_code
    ///
    /// This function is require for the implicit construction of a
    /// wse::win_error.
    [[nodiscard]] inline error_code make_error_code(wse::win_errc ec) noexcept;

}

namespace wse {

    class [[nodiscard]] win_error : public std::system_error {
    public:
        win_error();
        explicit win_error(win_errc ec);
        win_error(int ec, const std::error_category& cat);
    };

    bool operator==(const win_error& lhs, const win_error& rhs) noexcept;
    bool operator==(const win_error& lhs, const std::error_code& rhs) noexcept;
    bool operator==(const win_error& lhs, const win_errc& rhs) noexcept;
}

#endif // WIN_ERROR_H