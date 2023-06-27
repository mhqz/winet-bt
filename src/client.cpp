#include "util/signal.h"

#include <boost/asio/spawn.hpp>
#include <boost/asio/steady_timer.hpp>

namespace asio = boost::asio;
namespace errc = boost::system::errc;
namespace sys = boost::system;


using Cancel = ouinet::Signal<void()>;

int main() {
    asio::io_context ctx;
    sys::error_code ec_write;
    Cancel cancel;

    asio::spawn(ctx, [&](asio::yield_context yield){
        asio::steady_timer timer{ctx};
        timer.expires_from_now(std::chrono::seconds(2));
        timer.async_wait(yield);
    });
    ctx.run();

    return 0;
}
