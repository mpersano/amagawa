#pragma once

#include <msgpack.hpp>

namespace amagawa {

using request_id_t = uint16_t;
using msg_type_t = uint8_t;
using method_id_t = uint8_t;

static const msg_type_t MSG_REQUEST = 1;
static const msg_type_t MSG_RESPONSE = 2;

struct msg_header
{
	msg_type_t type;

	MSGPACK_DEFINE(type);
};

template <typename Param>
struct msg_request
{
	msg_request()
	: type(MSG_REQUEST)
	, request_id(0)
	, method_id(0)
	{ }

	msg_request(request_id_t request_id, method_id_t method_id, const Param& param)
	: type(MSG_REQUEST)
	, request_id(request_id)
	, method_id(method_id)
	, param(param)
	{ }

	msg_type_t type;
	request_id_t request_id;
	method_id_t method_id;
	Param param;

	MSGPACK_DEFINE(type, request_id, method_id, param);
};

template <typename Result>
struct msg_response
{
	msg_response()
	: type(MSG_RESPONSE)
	, request_id(0)
	{ }

	msg_response(request_id_t request_id, const Result& result)
	: type(MSG_RESPONSE)
	, request_id(request_id)
	, result(result)
	{ }

	msg_type_t type;
	request_id_t request_id;
	Result result;

	MSGPACK_DEFINE(type, request_id, result);
};

}
