#include "log.h"
#include "dispatcher.h"

namespace amagawa {

std::shared_ptr<msgpack::sbuffer>
dispatcher::dispatch(const msgpack::object& o) const
{
	msg_request<msgpack::object> m;
	o.convert(&m);

	auto it = handler_map_.find(m.method_id);

	if (it == handler_map_.end()) {
		log_err("unhandled method %d", m.method_id);
		return std::make_shared<msgpack::sbuffer>(); // XXX should throw something here
	}

	return it->second(m.request_id, m.param);
}

}
