#pragma once

#include <boost/asio.hpp>
#include "namespaces.hpp"

namespace asio_utp {

class context;
class udp_multiplexer_impl;

class service : public asio::execution_context::service {
public:
    using endpoint_type = asio::ip::udp::endpoint;
    using socket_type = asio::ip::udp::socket;

public:
    static asio::io_context::id id;

    service(asio::execution_context& ctx)
        : asio::execution_context::service(ctx)
    {}

    template<class Executor>
    std::shared_ptr<::asio_utp::context>
    maybe_create_context(Executor&, const endpoint_type&, sys::error_code& ec);

    std::shared_ptr<::asio_utp::context>
    maybe_create_context(std::shared_ptr<udp_multiplexer_impl>);

    void erase_context(endpoint_type ep);

    template<class Executor>
    std::shared_ptr<udp_multiplexer_impl>
    maybe_create_udp_multiplexer(Executor&, const endpoint_type&, sys::error_code& ec);

    void erase_multiplexer(endpoint_type ep);

    void shutdown() override {}

    ~service();

private:
    std::map<endpoint_type, std::weak_ptr<::asio_utp::context>> _contexts;
    std::map<endpoint_type, std::weak_ptr<udp_multiplexer_impl>> _multiplexers;

    bool _debug = false;
};

} // namespace

#include "udp_multiplexer_impl.hpp"
#include "context.hpp"

namespace asio_utp {

template<class Executor>
inline
std::shared_ptr<::asio_utp::context>
service::maybe_create_context(Executor& ex, const endpoint_type& ep, sys::error_code& ec)
{
    auto i = _contexts.find(ep);

    if (i != _contexts.end()) return i->second.lock();

    auto m  = maybe_create_udp_multiplexer(ex, ep, ec);

    if (ec) return nullptr;

    return maybe_create_context(std::move(m));
}

inline
void service::erase_context(endpoint_type ep)
{
    _contexts.erase(ep);
}

template<class Executor>
inline
std::shared_ptr<udp_multiplexer_impl>
service::maybe_create_udp_multiplexer(Executor& ex, const endpoint_type& ep, sys::error_code& ec)
{
    if (_debug) {
        std::cerr << "maybe_create_udp_multiplexer " << ep << " " << _multiplexers.size() << "\n";
    }

    auto i = _multiplexers.find(ep);

    if (i != _multiplexers.end()) return i->second.lock();

    socket_type socket(ex);
    socket.open(ep.protocol());
    socket.bind(ep, ec);

    if (ec) return nullptr;

    auto m = std::make_shared<udp_multiplexer_impl>(std::move(socket));
    _multiplexers[m->local_endpoint()] = m;

    return m;
}

inline
void service::erase_multiplexer(endpoint_type ep)
{
    if (_debug) {
        std::cerr << "erase_multiplexer " << ep << " " << _multiplexers.size() << "\n";
    }

    assert(_multiplexers.count(ep));
    _multiplexers.erase(ep);
}

inline service::~service()
{
    if (_debug) {
        std::cerr << "~service " << _contexts.size() << " " << _multiplexers.size() << "\n";
        assert(_contexts.empty());
        assert(_multiplexers.empty());
    }
}

} // asio_utp
