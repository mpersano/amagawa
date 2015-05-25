#include "session.h"

namespace amagawa {

session::session(socket_ptr_t socket, request_handler_t request_handler)
: socket_(socket)
, last_request_id_(0)
, request_handler_(request_handler)
{
	unpacker_.reserve_buffer(1024*1024);
}

void
session::start_read()
{
	auto self = shared_from_this();
	auto unpacker = &unpacker_;

	socket_->async_read_some(
		boost::asio::buffer(unpacker_.buffer(), unpacker_.buffer_capacity()),
		[this, self, unpacker](boost::system::error_code ec, size_t bytes_transferred)
			{
	
				if (ec) {
					if (ec == boost::asio::error::eof) {
						// XXX
					}
	
					// XXX
					return;
				}
	
				unpacker->buffer_consumed(bytes_transferred);
	
				while (true) {
					try {
						if (!unpacker->execute())
							break;
	
						on_message(unpacker->data());	
					} catch (msgpack::unpack_error) {
						// wtf
						std::cerr << "unpack error\n";
						return;
					} catch (msgpack::type_error) {
						// something wrong with the types
						std::cerr << "type error\n";
						return;
					} catch (...) {
						// lolwut
						std::cerr << "unhandled error\n";
						return;
					}
	
					unpacker->reset();
				}
	
				start_read();
			});
}

void
session::send(sbuffer_ptr_t sb)
{
	bool write_in_progress = !write_msgs_.empty();

	write_msgs_.push_back(sb);

	if (!write_in_progress)
		do_write();
}

void
session::do_write()
{
	auto& msg = write_msgs_.front();

	boost::asio::async_write(
		*socket_,
		boost::asio::buffer(msg->data(), msg->size()),
		[this](boost::system::error_code ec, size_t)
			{
				if (ec) {
					socket_->close();
					return;
				}

				write_msgs_.pop_front();

				if (!write_msgs_.empty())
					do_write();
			});
}

void
session::on_message(const msgpack::object& o)
{
	msg_header header;
	o.convert(&header);

	switch (header.type) {
		case message_type::REQUEST:
			on_request(o);
			break;

		case message_type::RESPONSE:
			on_response(o);
			break;
	}
}

void
session::on_request(const msgpack::object& o)
{
	send(request_handler_(o));
}

void
session::on_response(const msgpack::object& o)
{
	msg_response<msgpack::object> m;
	o.convert(&m);

	auto it = request_map_.find(m.request_id);

	if (it == request_map_.end()) {
		// XXX
		return;
	}

	it->second(m.result);

	request_map_.erase(it);
}

}
