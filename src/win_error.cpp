#if !defined(_68K_) && !defined(_MPPC_) && !defined(_X86_) && !defined(_IA64_) && !defined(_AMD64_) && defined(_M_IX86)
#define _X86_
#endif

#if !defined(_68K_) && !defined(_MPPC_) && !defined(_X86_) && !defined(_IA64_) && !defined(_AMD64_) && defined(_M_AMD64)
#define _AMD64_
#endif

#if !defined(_68K_) && !defined(_MPPC_) && !defined(_X86_) && !defined(_IA64_) && !defined(_AMD64_) && defined(_M_M68K)
#define _68K_
#endif

#if !defined(_68K_) && !defined(_MPPC_) && !defined(_X86_) && !defined(_IA64_) && !defined(_AMD64_) && defined(_M_MPPC)
#define _MPPC_
#endif

#if !defined(_68K_) && !defined(_MPPC_) && !defined(_X86_) && !defined(_M_IX86) && !defined(_AMD64_) && defined(_M_IA64)
#if !defined(_IA64_)
        #define _IA64_
    #endif
#endif

#include <errhandlingapi.h>
#include "win_error.h"

namespace std {

    [[nodiscard]] inline error_code make_error_code(wse::win_errc ec) noexcept {
        return {static_cast<int>(ec), wse::winSystemError};
    }

}

namespace wse {

    win_error::win_error()
        : system_error(win_errc(static_cast<int>(GetLastError())))
        {}

    win_error::win_error(win_errc ec)
        : system_error(ec)
        {}

    win_error::win_error(int ec, const std::error_category& cat)
        : system_error(std::error_code(ec, cat))
        {}

    bool operator==(const win_error& lhs, const win_error& rhs) noexcept {
        return lhs.code() == rhs.code();
    }

    bool operator==(const win_error& lhs, const std::error_code& rhs) noexcept {
        return lhs.code() == rhs;
    }

    bool operator==(const win_error& lhs, const win_errc& rhs) noexcept {
        return static_cast<win_errc>(lhs.code().value()) == rhs;
    }
}