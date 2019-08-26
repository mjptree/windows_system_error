#include "catch.hpp"

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

#include <minwindef.h>
#include <rpc.h>
#include "win_error.h"

TEST_CASE("Default constructed wse::win_error defaults to success.") {
    wse::win_error testError{};
    std::string firstCall = testError.what();

    // To make the tests portable we need to retrieve a message with the same language ID
    auto retrieveMessage= []() {
        constexpr DWORD flags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
        constexpr DWORD langId = MAKELANGID(LANG_SYSTEM_DEFAULT, SUBLANG_SYS_DEFAULT);
        constexpr DWORD size = 2048;
        TCHAR buf[size] = {0};
        FormatMessageA(
                flags,
                nullptr,
                0,
                langId,
                buf,
                size,
                nullptr
        );
        if (char* crlf = strrchr(buf, '\r'); crlf != nullptr) *crlf = 0;
        return std::string{buf};
    };
    std::string msg = retrieveMessage();

    // The default state of win_error is ERROR_SUCCESS
    CHECK(firstCall == msg);
    CHECK(testError.code().value() == 0);

    // wse::win_error can be compared to instances of itself,...
    CHECK(testError == wse::win_error{wse::win_errc::success_exception});

    // ... std::error_code and wse::win_errc
    wse::win_errc ev = wse::win_errc::success_exception;
    std::error_code ec = std::make_error_code(ev);
    CHECK(testError == ev);
    CHECK(testError == ec);

    // Accordingly, two different instances of wse::win_error are not equal if
    // originating by different errors
    wse::win_error otherError{wse::win_errc::invalid_function};
    CHECK_FALSE(testError == otherError);
    CHECK_FALSE(strcmp(testError.what(), otherError.what()) == 0);
    CHECK_FALSE(testError == wse::win_errc::file_not_found);

    // And just for safety
    CHECK(wse::win_error{wse::win_errc::file_not_found} == wse::win_errc::file_not_found);

    // Subsequent calls of the `what` method do not reallocate internal string
    std::string secondCall = testError.what();
    CHECK(firstCall == secondCall);

}

TEST_CASE("The error codes from system calls can conveniently be wrapped in wse::win_error") {
    using wse::win_errc;

    // Example 1: We manually set the error code in the system and then retrieve it
    //            again through default construction
    auto autoThrow = [](win_errc ec) {
        SetLastError(static_cast<DWORD>(ec));
        throw wse::win_error{};
    };

    CHECK_THROWS_AS(autoThrow(win_errc::more_data), wse::win_error);

    // Example 2: Manually set and throw error again, but try catch it this time
    try {
        autoThrow(win_errc::path_not_found);
    } catch (std::system_error& e) {
        SUCCEED("Successfully caught thrown wse::win_error");
        wse::win_error other{win_errc::path_not_found};
        CHECK(e.code() == other.code());

        // Remember that comparing char* via operator==() actually compares the
        // pointer address, not the content of the char array!
        CHECK(strcmp(e.what(), other.what()) == 0);
    } catch (std::exception&) {
        FAIL("Failed to catch thrown wse::win_error");
    } catch (...) {
        FAIL("Failed to catch anything");
    }

    // Example 3: wse::win_error can be caught by std::exception and still provides
    //            the correct `what`
    try {
        autoThrow(win_errc::access_denied);
    } catch (std::exception& e) {
        SUCCEED("Successfully caught via std::exception");
        wse::win_error other{win_errc::access_denied};
        CHECK(strcmp(e.what(), other.what()) == 0);
    } catch (...) {
        FAIL("Failed to catch exception");
    }
}