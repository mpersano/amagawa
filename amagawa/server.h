#pragma once

#include <msgpack.hpp>

#include <boost/asio.hpp>

#include "fwd.h"

namespace amagawa {

class server
{
	using request_handler_t = std::function<sbuffer_ptr_t(const msgpack::object& o)>;

public:
	server(boost::asio::io_service& io, int port, request_handler_t request_handler_);

private:
	void start_accept();

	boost::asio::ip::tcp::acceptor acceptor_;
	request_handler_t request_handler_;
};

}
