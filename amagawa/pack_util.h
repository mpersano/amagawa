#pragma once

#include <msgpack.hpp>

#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/algorithm.hpp>

namespace msgpack {
MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
namespace adaptor {

template <typename... Args>
struct pack<boost::fusion::vector<Args...>>
{
	template <typename Stream>
	packer<Stream>& operator()(packer<Stream>& o, const boost::fusion::vector<Args...>& t) const
	{
		o.pack_array(boost::fusion::size(t));
		boost::fusion::for_each(t, [&o](const auto& v) { o.pack(v); });
		return o;
	}
};

} } }
