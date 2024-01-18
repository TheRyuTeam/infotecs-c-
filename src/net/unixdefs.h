#pragma once
#include "net.h"

NET_START

using socket_t = int;
using socklen_t = unsigned int;
constexpr socket_t kInvalidSocket = -1;

NET_END
