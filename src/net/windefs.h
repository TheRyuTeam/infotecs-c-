#pragma once
#   ifndef NOMINMAX
#       define NOMINMAX 1
#   endif

#	define WINVER                  0x600
#	define _WIN32_WINDOWS          0x600

#	ifdef _WIN32_WINNT
#		undef _WIN32_WINNT
#	endif
#	define _WIN32_WINNT            0x600
#include <basetsd.h>

using socket_t = UINT_PTR;
constexpr socket_t kInvalidSocket = static_cast<socket_t>(~static_cast<socket_t>(0));
