#include "pch.h"
#include "TrainingLink.h"

void TrainingLink::StartWsServer()
{
	cvarManager->log("[RLINK] Starting WebSocket server");
	ws_connections = new ConnectionSet();
	ws_server = new PluginServer();

	cvarManager->log("[RLINK] Starting asio");
	ws_server->init_asio();
	ws_server->set_open_handler(websocketpp::lib::bind(&TrainingLink::OnWsOpen, this, _1));
	ws_server->set_close_handler(websocketpp::lib::bind(&TrainingLink::OnWsClose, this, _1));
	ws_server->set_http_handler(websocketpp::lib::bind(&TrainingLink::OnHttpRequest, this, _1));

	cvarManager->log("[RLINK] Starting listen on port " + std::to_string(*cvarPort.get()));
	ws_server->listen(*cvarPort);

	ws_server->start_accept();
	ws_server->run();

}

void TrainingLink::StopWsServer()
{
	if (ws_server)
	{
		ws_server->stop();
		ws_server->stop_listening();
		delete ws_server;
		ws_server = nullptr;
	}

	if (ws_connections)
	{
		ws_connections->clear();
		delete ws_connections;
		ws_connections = nullptr;
	}
}


void TrainingLink::OnHttpRequest(websocketpp::connection_hdl hdl)
{
	PluginServer::connection_ptr connection = ws_server->get_con_from_hdl(hdl);

	const std::string resource = connection->get_resource();
	std::regex rgx("/trainingpack/([A-Z0-9-]+)");
	std::smatch match;

	if (resource.find("/trainingpack") != std::string::npos)
	{
		cvarManager->log("[RLINK] Loading Training Pack!");

		if (std::regex_search(resource.begin(), resource.end(), match, rgx)) {
			std::string packCode = match[1];
			std::stringstream launch;
			launch << "sleep 1; load_training " << packCode;
			cvarManager->executeCommand(launch.str());
		}
	}
}

