#include "network.h"
#include<boost/asio.hpp>
#include<Siv3D.hpp>

namespace asio = boost::asio;
using asio::ip::tcp;

#define HOST "google.com"

String upload(json piece){
	asio::io_service io;
	tcp::socket socket(io);

	tcp::resolver resolver(io);
	tcp::resolver::query query(HOST, "http");

	boost::system::error_code error;

	tcp::endpoint endpoint(*resolver.resolve(query));
	socket.connect(endpoint, error);

	if (error) {
		return Format(L"connect failed:", Widen(error.message()));
	}
	else {
		return L"connected";
	}
}
