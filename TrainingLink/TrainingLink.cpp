#define _USE_MATH_DEFINES

#include "pch.h"
#include "TrainingLink.h"
#include "bakkesmod/wrappers/GameObject/Stats/StatEventWrapper.h"

BAKKESMOD_PLUGIN(TrainingLink, "TrainingLink", "1.0", PLUGINTYPE_THREADED)

void TrainingLink::onLoad()
{
	cvarManager->log("[RLINK] Loading...");

	cvarPort = std::make_shared<int>();
	cvarManager->registerCvar("RLINK_Port", "56789", "WebSocket port for TrainingLink", true).bindTo(cvarPort);

	StartWsServer();
}

void TrainingLink::onUnload()
{
	StopWsServer();
	cvarManager->log("[RLINK] Unloaded");
}
