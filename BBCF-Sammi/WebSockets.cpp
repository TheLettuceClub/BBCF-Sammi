#include "WebSockets.hpp"

void worker_t::work() {
	loop = uWS::Loop::get();
	app = std::make_shared<uWS::App>();

	app->ws<PerSocketData>("/*", {
		.compression = uWS::SHARED_COMPRESSOR,
		.maxBackpressure = 1*1024*1024,
		.open = [](uWS::WebSocket<false, true, PerSocketData>* ws) {
			//log here eventually
			std::cout << "new subscriber" << std::endl;
			ws->subscribe("broadcast");
		},
		.message = [](uWS::WebSocket<false, true, PerSocketData>* ws, std::string_view message, uWS::OpCode opCode) {
			std::cout << "new message" << std::endl;
			ws->send(message, opCode);
		},
		}).listen(port, [this](us_listen_socket_t* token) {
			listen_socket = token;
			if (listen_socket) {
				std::cout << "listening on port " << port << std::endl;
			}
			else {
				std::cout << "failed to listen on port " << port << std::endl;
			}
		}).run();
}