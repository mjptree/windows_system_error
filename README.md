# windows_system_error

Wrap windows system error codes in `std::system_error` types and handle them like `std::exceptions`.

This library is currently highly experimental. The API will most certainly undergo substantial changes, before full stabilization.

# Example

```
#include <handleapi.h>
#include <iostream>
#include "win_error.h"

// Sequential write from buffer to file
DWORD write(HANDLE handle, const char* buffer) {
  DWORD bytes = 0;
  LPWORD len = sizeof buf;
  BOOL result = WriteFile(handle, buffer, len, &bytes, nullptr);
  if (result) {
    return bytes;
  } else {
    throw wse::win_error{};
  }
}

int main() {
  HANDLE handle;
  const char[] buffer = "Test data to write.";
  DWORD writtenBytes = 0;
  
  try {
  
    // Write will most likely fail
    writtenBytes = write(handle, buffer);
  } 
  
  // wse::win_error inherits from std::system_error and can therefore
  // be caught also by std::runtime_error and std::exception.
  catch (std::exception& error) {
  
    // wse::win_error saves the error code internally upon creation and
    // retrieves the appropriate error message from the OS.
    std::cout << "OS returned following error message:\n"
          << error.what() << std::endl;
  }
}
```

