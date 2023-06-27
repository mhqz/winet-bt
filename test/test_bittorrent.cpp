#define BOOST_TEST_MODULE bittorrent
#include <boost/test/included/unit_test.hpp>
#include <boost/optional.hpp>
#include <boost/asio.hpp>

#include <namespaces.h>
#include <iostream>
#include <util/wait_condition.h>

#define private public
#include <bittorrent/node_id.h>
#include <bittorrent/dht.h>
#include <bittorrent/code.h>
#include <util/hash.h>

BOOST_AUTO_TEST_SUITE(bittorrent)

using namespace std;
using namespace ouinet;
using namespace ouinet::bittorrent;
using Clock = chrono::steady_clock;

using boost::optional;

static
__attribute__((unused))
float seconds(Clock::duration d)
{
    using namespace chrono;
    return duration_cast<milliseconds>(d).count() / 1000.f;
}


BOOST_AUTO_TEST_CASE(test_bep_5)
{
    using namespace ouinet::bittorrent::dht;

    asio::io_context ctx;

    DhtNode dht(ctx);

    asio::spawn(ctx, [&] (auto yield) {
        sys::error_code ec;
        Signal<void()> cancel_signal;

        NodeID infohash = util::sha1_digest("ouinet-test-" + to_string(time(0)));

        dht.start({asio::ip::make_address("0.0.0.0"), 0}, yield[ec]); // TODO: IPv6
        BOOST_REQUIRE(!ec);

        dht.tracker_announce(infohash, dht.wan_endpoint().port(), cancel_signal, yield[ec]);
        BOOST_REQUIRE(!ec);

        auto peers = dht.tracker_get_peers(infohash , cancel_signal, yield[ec]);
        BOOST_REQUIRE(!ec);

        BOOST_REQUIRE(peers.count(dht.wan_endpoint()));

        dht.stop();
    });

    ctx.run();
}

BOOST_AUTO_TEST_SUITE_END()
