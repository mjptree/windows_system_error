#if !defined(_68K_) && !defined(_MPPC_) && !defined(_X86_) && !defined(_IA64_) && !defined(_AMD64_)

	#if defined(_M_IX86)
		#define _X86_
	#endif

	#if defined(_M_AMD64)
		#define _AMD64_
	#endif

	#if defined (_M_M68K)
		#define _68K_
	#endif

	#if defined(_M_MPPC)
		#define _MPPC_
	#endif

	#if !defined(_M_IX86) && defined(_M_IA64)
		#define _IA64_
	#endif

#endif

#include <sstream>
#include <minwindef.h>
#include <rpc.h>
#include "win_error_category.h"

namespace wse {

    /// Uniquely identify error category
    [[nodiscard]] const char* win_error_category::name() const noexcept {
        return "OS Error";
    }

    /// Retrieve corresponding OS error message for given error value
    std::string win_error_category::message(int ev) const {
        constexpr DWORD flags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
        constexpr DWORD langId = MAKELANGID(LANG_SYSTEM_DEFAULT, SUBLANG_SYS_DEFAULT);
        DWORD errCode = ev;
        constexpr DWORD size = 2048;

        // Providing our own buffer spares us having to taking care of calling LocalFree()
        // on the otherwise allocated LPTSTR, which is costly.
        TCHAR buf[size] = {0};

        // The default constructor for std::string accepts a char array, which is handled by
        // the ANSII implementation of FormatMessage. FormatMessageW() expects a buffer of
        // type WCHAR[], which would result in error-prone conversions.
        DWORD result = FormatMessageA(
            flags,
            nullptr,
            errCode,
            langId,
            buf,
            size,
            nullptr
            );
        
        if (!result) {
            std::stringstream output;
            output << "OS Error " << ev << " (FormatMessageA() returned error " << GetLastError() << ")";
            return output.str();
        }

        // Windows terminates lines with "\r\n" (carriage return, line feed)
        if (char* crlf = strrchr(buf, '\r'); crlf != nullptr) *crlf = 0;
        return {buf};
    }

} // namespace wse