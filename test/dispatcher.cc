#include <boost/test/unit_test.hpp>

#include <amagawa/pack_util.h>
#include <amagawa/dispatcher.h>

namespace {

template <typename Return, typename... Args>
Return
call_through_dispatcher(const amagawa::dispatcher& d, amagawa::method_id_t method_id, Args... args)
{
	// pack message

	using param_vector_t = boost::fusion::vector<Args...>;
	amagawa::msg_request<param_vector_t> request_message(0x1337, method_id, param_vector_t(args...));

	msgpack::sbuffer sb;
	msgpack::pack(sb, request_message);

	msgpack::unpacked msg;
	msgpack::unpack(&msg, sb.data(), sb.size());

	// dispatch

	std::shared_ptr<msgpack::sbuffer> ret_sb = d.dispatch(msg.get());

	// unpack result

	msgpack::unpacked ret_msg;
	msgpack::unpack(&ret_msg, ret_sb->data(), ret_sb->size());

	amagawa::msg_response<Return> response_message;
	ret_msg.get().convert(&response_message);

	BOOST_CHECK(response_message.request_id == request_message.request_id);

	return response_message.result;
}

}

BOOST_AUTO_TEST_CASE(dispatcher0)
{
	const amagawa::method_id_t method_id = 0;

	amagawa::dispatcher d;
	d.add_handler(method_id, []() -> int { return 42; });

	BOOST_CHECK(call_through_dispatcher<int>(d, method_id) == 42);
}

BOOST_AUTO_TEST_CASE(dispatcher1)
{
	const amagawa::method_id_t method_id = 0;

	amagawa::dispatcher d;
	d.add_handler(method_id, [](int a, float b) -> float { return a + b; });

	BOOST_CHECK(call_through_dispatcher<float>(d, method_id, 3, 5.5) == 8.5);
}
