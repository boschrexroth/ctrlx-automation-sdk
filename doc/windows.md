# Apps for Windows

ctrlX OS apps are built using the _App Builder Environment_ (QEMU VM), which is the standard.
Nevertheless, if you want to create apps for Windows, the following Data Layer APIs are supported on Windows:

Data Layer API | Windows Support
:----          | :----:
C++            | X
.NET           | X
Node.js        | X
Go             |
Python         |
Java           |
Rust           |

## Prerequisites
Please ensure that the latest available versions of the following are installed on the target machine.

### Microsoft Visual C++ Redistributable

[Microsoft Visual C++ Redistributable](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist)

### OpenSSL

The Data Layer C++ API uses OpenSSL libraries.
You can download and install the Windows distribution installer [Win64 OpenSSL Light](https://slproweb.com/products/Win32OpenSSL.html).

__Encryption Software__

Because of the __US Export Administration Regulations (US-EAR)__, we cannot ship OpenSSL within the SDK. Instead, you must install it separately on the target machine(s). The _License Exception ENC_, _5D992 (Mass Market) Exception_, and _Publicly Available Exception_ (EAR §734.3(b)(3) and §742.15(b)) are not applicable here.
