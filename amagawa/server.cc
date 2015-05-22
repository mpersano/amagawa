#include "session.h"
#include "server.h"

namespace amagawa {

server::server(boost::asio::io_service& io, int port, request_handler_t request_handler)
: acceptor_(io, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
, request_handler_(request_handler)
{
	start_accept();
}

void
server::start_accept()
{
	auto socket = std::make_shared<boost::asio::ip::tcp::socket>(acceptor_.get_io_service());

	acceptor_.async_accept(
		*socket,
		[=](boost::system::error_code ec) {
			if (!ec) {
				std::make_shared<session>(socket, request_handler_)->start_read();
			}

			start_accept();
		});
}

}
