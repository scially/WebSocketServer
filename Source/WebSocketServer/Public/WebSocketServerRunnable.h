#pragma once

#include "CoreMinimal.h"
#include "Templates/TypeHash.h"
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> Server;
typedef Server::connection_ptr ConnectionPtr;
typedef websocketpp::connection_hdl ConnectionHdl;

struct ConnectionHdlKeyFuncs: public BaseKeyFuncs<ConnectionHdl, ConnectionHdl, false> {
	
	inline static KeyInitType GetSetKey(ElementInitType Element) { //返回元素的键。在集合中，通常是元素本身。
		return Element;
	}

	inline static bool Matches(KeyInitType A, KeyInitType B) { //如果 A 和 B 等值将返回 true，否则返回 false。
		return A.lock() == B.lock();
	}

	inline static uint32 GetKeyHash(KeyInitType Key) { //返回 Key 的散列值。
		return GetTypeHash(static_cast<void*>(Key.lock().get()));
	}
};

DECLARE_MULTICAST_DELEGATE_OneParam(FWebSocketServerReceiveMessage, FString);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FWebSocketServerConnectMessage, FString, int, FString); // host, port, origin
DECLARE_MULTICAST_DELEGATE_ThreeParams(FWebSocketServerCloseMessage,   FString, int, FString);  // host, port, origin
DECLARE_MULTICAST_DELEGATE(FWebSocketServerErrorMessage);

class WebSocketServerRunnable : public FRunnable {
public:
	WebSocketServerRunnable();
	virtual ~WebSocketServerRunnable() { Endpoint.stop();}
	
	WebSocketServerRunnable(uint32 port);
	virtual uint32 Run() override;
	virtual void Stop() override { Endpoint.stop();}
	virtual void Exit() override { Endpoint.stop();}
	
	TSet<ConnectionHdl, ConnectionHdlKeyFuncs> GetConnections() const {return Connections;}
	FWebSocketServerReceiveMessage& GetReceiveMessageDelegate() {return ReceiveMessageDelegate;}
	FWebSocketServerConnectMessage& GetReceiveConnectDelegate() {return ConnectMessageDelegate;}
	FWebSocketServerCloseMessage&   GetReceiveCloseDelegate() {return CloseMessageDelegate;}
	FWebSocketServerErrorMessage&   GetReceiveErrorDelegate() {return ErrorMessageDelegate;}
	void SendServerMessage(const FString& msg);
private:
	void OnMessage(ConnectionHdl hdl, Server::message_ptr msg);
	void OnConnection(ConnectionHdl hdl);
	void OnClosed(ConnectionHdl hdl);
	void OnError(ConnectionHdl hdl);
	uint32 Port;
	Server Endpoint;
	TSet<ConnectionHdl, ConnectionHdlKeyFuncs> Connections;
	FWebSocketServerReceiveMessage ReceiveMessageDelegate;
	FWebSocketServerConnectMessage ConnectMessageDelegate;
	FWebSocketServerCloseMessage   CloseMessageDelegate;
	FWebSocketServerErrorMessage   ErrorMessageDelegate;
};