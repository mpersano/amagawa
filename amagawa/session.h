#pragma once

#include <msgpack.hpp>

#include <boost/asio.hpp>

#define BOOST_THREAD_PROVIDES_FUTURE
#include <boost/thread.hpp>
#include <boost/thread/future.hpp>

#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/include/vector.hpp>

#include "fwd.h"
#include "pack_util.h"
#include "message.h"

namespace amagawa {

using socket_ptr_t = std::shared_ptr<boost::asio::ip::tcp::socket>;

class session : public std::enable_shared_from_this<session>
{
	using request_handler_t = std::function<sbuffer_ptr_t(const msgpack::object&)>;
	using response_handler_t = std::function<void(const msgpack::object&)>;

public:
	session(socket_ptr_t socket, request_handler_t request_handler);
	virtual ~session() { }

	template <typename Result, typename... Args>
	boost::future<Result> invoke(method_id_t method_id, Args... args)
	{
		++last_request_id_;

		auto promise = std::make_shared<boost::promise<Result>>();

		request_map_.insert(std::make_pair(last_request_id_, [promise](const msgpack::object& o)
			{
				Result result;
				o.convert(&result);
				promise->set_value(result);
			}));

		using param_vector_t = boost::fusion::vector<Args...>;
		msg_request<param_vector_t> message{last_request_id_, method_id, param_vector_t(args...)};

		auto sb = std::make_shared<msgpack::sbuffer>();
		msgpack::pack(*sb, message);

		send(sb);

		return promise->get_future();
	}

	void start_read();

private:
	void send(sbuffer_ptr_t sb);

	void on_message(const msgpack::object& o);
	void on_request(const msgpack::object& o);
	void on_response(const msgpack::object& o);

	void do_write();

	socket_ptr_t socket_;
	msgpack::unpacker unpacker_;
	std::deque<sbuffer_ptr_t> write_msgs_;
	std::map<request_id_t, response_handler_t> request_map_;
	request_id_t last_request_id_;
	request_handler_t request_handler_;
};

}
