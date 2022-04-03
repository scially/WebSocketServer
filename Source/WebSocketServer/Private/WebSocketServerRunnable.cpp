
#include "WebSocketServerRunnable.h"
#include <functional>

WebSocketServerRunnable::WebSocketServerRunnable():WebSocketServerRunnable(9090) {
    
}

WebSocketServerRunnable::WebSocketServerRunnable(uint32 port) :Port(port) {
    Endpoint.set_error_channels(websocketpp::log::elevel::all);
    Endpoint.set_access_channels(websocketpp::log::alevel::all ^ websocketpp::log::alevel::frame_payload);
   
    // Initialize Asio
    Endpoint.init_asio();
    // Bind Message Handler
    Endpoint.set_message_handler(std::bind(&WebSocketServerRunnable::OnMessage, this, std::placeholders::_1, std::placeholders::_2));
    Endpoint.set_open_handler(std::bind(&WebSocketServerRunnable::OnConnection, this, std::placeholders::_1));
    Endpoint.set_close_handler(std::bind(&WebSocketServerRunnable::OnClosed, this, std::placeholders::_1));
    Endpoint.set_fail_handler(std::bind(&WebSocketServerRunnable::OnError, this, std::placeholders::_1));
}

void WebSocketServerRunnable::SendServerMessage(const FString& msg){
    std::string content(TCHAR_TO_UTF8(*msg));
    for(auto pair : Connections)
    {
        const Server::connection_ptr con = Endpoint.get_con_from_hdl(pair);
        const Server::message_ptr context = con->get_message(websocketpp::frame::opcode::text, content.size());
        context->set_payload(content);
        Endpoint.send(pair, context);
    }
}

uint32 WebSocketServerRunnable::Run() {
    Endpoint.listen(Port);
    // Queues a connection accept operation
    Endpoint.start_accept();
    // Start the Asio io_service run loop
    uint32 status = 1;
    try {
        status = Endpoint.run();
    }
    catch (...) {
        UE_LOG(LogTemp, Warning, TEXT("WebSocketServer Error..."),);
    }
    return status;
}

void WebSocketServerRunnable::OnMessage(ConnectionHdl hdl, Server::message_ptr msg) {
    FString Message = ANSI_TO_TCHAR(msg->get_payload().c_str());
    UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
    ReceiveMessageDelegate.Broadcast(Message);
}

void WebSocketServerRunnable::OnConnection(ConnectionHdl hdl) {
    UE_LOG(LogTemp, Log, TEXT("New Connection"));
    Server::connection_ptr con = Endpoint.get_con_from_hdl(hdl);
    std::string host = con->get_host();
    uint16_t port = con->get_port();
    std::string origin = con->get_origin();
    ConnectMessageDelegate.Broadcast(FString(host.c_str()), port, FString(origin.c_str()));
    Connections.Add(hdl);
}

void WebSocketServerRunnable::OnClosed(ConnectionHdl hdl) {
    UE_LOG(LogTemp, Warning, TEXT("Closed Connection"));
    Server::connection_ptr con = Endpoint.get_con_from_hdl(hdl);
    std::string host = con->get_host();
    uint16_t port = con->get_port();
    std::string origin = con->get_origin();
    CloseMessageDelegate.Broadcast(FString(host.c_str()), port, FString(origin.c_str()));
    Connections.Remove(hdl);
}

void WebSocketServerRunnable::OnError(ConnectionHdl hdl) {
    UE_LOG(LogTemp, Error, TEXT("WebSocketServer Error"));
    ErrorMessageDelegate.Broadcast();
}