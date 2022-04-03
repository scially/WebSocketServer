// Copyright Epic Games, Inc. All Rights Reserved.

#include "WebSocketServer.h"

#define LOCTEXT_NAMESPACE "FWebSocketServerModule"

void FWebSocketServerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FWebSocketServerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FWebSocketServerModule, WebSocketServer)