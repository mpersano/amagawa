#pragma once

#include <memory>

#include <msgpack.hpp>
using sbuffer_ptr_t = std::shared_ptr<msgpack::sbuffer>;

namespace amagawa {

class session;
using session_ptr_t = std::shared_ptr<session>;

class session_factory;
using session_factory_ptr_t = std::shared_ptr<session_factory>;

}
