#pragma once
#include "uwebsockets/App.h"
#include <memory>
#include <thread>

struct PerSocketData {};

struct worker_t {
	void work();
	int port = 42617; // release date of BBCF on Steam
	us_listen_socket_t* listen_socket;
	uWS::Loop* loop;
	std::shared_ptr<uWS::App> app;
	std::shared_ptr<std::thread> thread;
};