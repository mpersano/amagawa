#pragma once

#include <msgpack.hpp>

#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/algorithm.hpp>

namespace msgpack {

template <typename Stream, typename... Args>
inline packer<Stream>& operator<<(packer<Stream>& o, const boost::fusion::vector<Args...>& t)
{
	o.pack_array(boost::fusion::size(t));
	boost::fusion::for_each(t, [&o](const auto& v) { o.pack(v); });
	return o;
}

}
