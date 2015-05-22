#include <boost/test/unit_test.hpp>

#define BOOST_THREAD_PROVIDES_FUTURE
#include <boost/thread.hpp>

#include <amagawa/fwd.h>
#include <amagawa/server.h>
#include <amagawa/dispatcher.h>
#include <amagawa/session.h>

static const int PORT = 4141;

BOOST_AUTO_TEST_CASE(server0)
{
	amagawa::dispatcher d;
	d.add_handler(0, [](int a, float b) -> float { return a + b; });
	d.add_handler(1, [](const std::string& str) -> std::string { return "x" + str + "x"; });

	boost::asio::io_service server_io;
	amagawa::server server(server_io, PORT, [&d](const msgpack::object& o)
		{
			return d.dispatch(o);
		});

	boost::thread server_thread([&server_io]() { server_io.run(); });

	boost::asio::io_service client_io;

	auto client_socket = std::make_shared<boost::asio::ip::tcp::socket>(client_io);
	client_socket->connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), PORT));

	auto client = std::make_shared<amagawa::session>(client_socket, [](const msgpack::object&) { return sbuffer_ptr_t(); });
	client->start_read();

	boost::thread client_thread([&client_io]() { client_io.run(); });

	BOOST_CHECK(client->invoke<float>(0, 3, 2.5f).get() == 5.5f);
	BOOST_CHECK(client->invoke<std::string>(1, std::string("o")).get() == "xox");

	client_io.stop();
	client_thread.join();

	server_io.stop();
	server_thread.join();
}
