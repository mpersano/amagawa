#pragma once

#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/algorithm.hpp>
#include <boost/fusion/functional/invocation/invoke.hpp>
#include <boost/fusion/include/mpl.hpp>

#include <msgpack.hpp>

#include "fwd.h"
#include "message.h"

namespace amagawa {

class session;

class dispatcher
{
	using message_handler_t = std::function<sbuffer_ptr_t(request_id_t request_id, const msgpack::object&)>;

public:
	template <typename Handler>
	void add_handler(method_id_t method_id, const Handler& handler)
	{
		add_handler(method_id, handler, &Handler::operator());
	}

	sbuffer_ptr_t dispatch(const msgpack::object& o) const;

private:
	template <typename Handler, typename Result, typename... Args>
	void add_handler(method_id_t method_id, const Handler& handler, Result(Handler::*)(Args...)const)
	{
		handler_map_.insert(std::make_pair(method_id, [method_id, handler](request_id_t request_id, const msgpack::object& o)
			{
				namespace mpl = boost::mpl;

				typename boost::fusion::result_of::as_vector<
					typename mpl::transform<
						mpl::vector<Args...>,
						std::remove_const<
							std::remove_reference<
								mpl::placeholders::_1
							>
						>
					>::type
				>::type params;

				if (o.type != msgpack::type::ARRAY)
					throw msgpack::type_error();

				msgpack::object *p = o.via.array.ptr;
				boost::fusion::for_each(params, [&p](auto& v) { p->convert(&v); ++p; });

				Result result = boost::fusion::invoke(handler, params);

				auto sb = std::make_shared<msgpack::sbuffer>();
				msgpack::pack(*sb, msg_response<Result>(request_id, result));

				return sb;
			}));
	}

	std::map<method_id_t, message_handler_t> handler_map_;
};

}
