#include <iostream>
#include "boost/asio.hpp"
#include "boost/thread.hpp"
#include "boost/bind.hpp"

using namespace boost::asio;

size_t read_complete(char* buf, const boost::system::error_code& err, size_t bytes)
{
    if (err) return (0);
    bool found = std::find (buf, buf + bytes, '\n') < buf + bytes;
    return found ? 0 : 1;
}

void sync_echo(std::string msg, io_service& service, ip::tcp::endpoint const& ep)
{
    msg += "\n";
    ip::tcp::socket sock(service);
    sock.connect(ep);
    sock.write_some (buffer(msg));
    char buf[1024];
    int bytes = read (sock, buffer(buf), bind(read_complete, buf, _1, _2));
    std::string copy(buf, bytes - 1);
    msg = msg.substr(0, msg.size() - 1);
    std::cout << "server returned: " << msg << "-> "
        << (copy == msg ? "OK":"FAIL") << std::endl;
    sock.close();
}

int main()
{
    io_service service;
    char const* messages[] = {"I say hi",
                        "So does Tony",
                        "July is not home yet",
                        "Programming is fun! Sometimes"};
    boost::thread_group threads;
    ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 8000);
    for (auto p : messages)
    {
        threads.create_thread(boost::bind(sync_echo, p,
                boost::ref(service), boost::cref(ep)));
        boost::this_thread::sleep(boost::posix_time::millisec(100));
    }
    threads.join_all();
    return 0;
}