#pragma once

#include <msgpack.hpp>

namespace amagawa {

using request_id_t = uint16_t;
using method_id_t = uint8_t;

enum class message_type { REQUEST, RESPONSE };

struct msg_header
{
	message_type type;

	MSGPACK_DEFINE(type);
};

template <typename Param>
struct msg_request
{
	msg_request()
	: type(message_type::REQUEST)
	, request_id(0)
	, method_id(0)
	{ }

	msg_request(request_id_t request_id, method_id_t method_id, const Param& param)
	: type(message_type::REQUEST)
	, request_id(request_id)
	, method_id(method_id)
	, param(param)
	{ }

	message_type type;
	request_id_t request_id;
	method_id_t method_id;
	Param param;

	MSGPACK_DEFINE(type, request_id, method_id, param);
};

template <typename Result>
struct msg_response
{
	msg_response()
	: type(message_type::RESPONSE)
	, request_id(0)
	{ }

	msg_response(request_id_t request_id, const Result& result)
	: type(message_type::RESPONSE)
	, request_id(request_id)
	, result(result)
	{ }

	message_type type;
	request_id_t request_id;
	Result result;

	MSGPACK_DEFINE(type, request_id, result);
};

}

MSGPACK_ADD_ENUM(amagawa::message_type);
