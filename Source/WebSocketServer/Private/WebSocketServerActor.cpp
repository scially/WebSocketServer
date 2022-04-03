// Fill out your copyright notice in the Description page of Project Settings.

#include "WebSocketServerActor.h"
#include "WebSocketServerRunnable.h"
#include "Json.h"
// Sets default values
AWebSocketServerActor::AWebSocketServerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Port = 9092;
}

// Called when the game starts or when spawned
void AWebSocketServerActor::BeginPlay()
{
	Super::BeginPlay();
	RunnablePtr = MakeShared<WebSocketServerRunnable>(Port);
	ThreadPtr = MakeShareable<FRunnableThread>(FRunnableThread::Create(RunnablePtr.Get(), TEXT("WebSocketServerThread")));

	// Register Event Callback
	RunnablePtr->GetReceiveMessageDelegate().AddLambda(
	std::bind(
		&AWebSocketServerActor::ReceiveMessageCallback,
		this,
		std::placeholders::_1));
	RunnablePtr->GetReceiveConnectDelegate().AddLambda(
		std::bind(
			&AWebSocketServerActor::ConnectMessageCallback,
			this,
			std::placeholders::_1,
			std::placeholders::_2,
			std::placeholders::_3));
	RunnablePtr->GetReceiveCloseDelegate().AddLambda(
	std::bind(
		&AWebSocketServerActor::CloseMessageCallback,
		this,
		std::placeholders::_1,
		std::placeholders::_2,
		std::placeholders::_3));
	RunnablePtr->GetReceiveErrorDelegate().AddLambda(
	std::bind(
		&AWebSocketServerActor::ErrorMessageCallback,
		this));
}

// Called every frame
void AWebSocketServerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AWebSocketServerActor::SendServerMessage(const FString &Message)
{
	RunnablePtr->SendServerMessage(Message);
}

void AWebSocketServerActor::SendServerJsonMessage(const FString &Type, const FString &Command, const FString &Brief)
{
	FString jsonMsg;
	const auto writer = TJsonWriterFactory<TCHAR>::Create(&jsonMsg);
	TSharedRef<FJsonObject> object(new FJsonObject);
	object->SetStringField("Type", Type);
	object->SetStringField("Command", Command);
	object->SetStringField("Brief", Brief);
	FJsonSerializer::Serialize(object, writer);
	SendServerMessage(jsonMsg);
}

void AWebSocketServerActor::ReceiveMessageCallback(const FString &Message)
{
	this->OnMessage.Broadcast(Message);

	// For GZPI, right json and send it
	const auto jsonReader = TJsonReaderFactory<TCHAR>::Create(Message);
	TSharedPtr<FJsonObject> msgObject;
	if (!FJsonSerializer::Deserialize(jsonReader, msgObject))
		return;

	FString type_, command, brief;
	msgObject->TryGetStringField(TEXT("Type"), type_);
	msgObject->TryGetStringField(TEXT("Command"), command);
	msgObject->TryGetStringField(TEXT("Brief"), brief);
	this->OnJsonMessage.Broadcast(type_, command, brief);
}

void AWebSocketServerActor::ConnectMessageCallback(const FString &Host, const int Port_, const FString &Origin)
{
	this->OnConnect.Broadcast(Host, Port_, Origin);
}

void AWebSocketServerActor::CloseMessageCallback(const FString &Host, const int Port_, const FString &Origin)
{
	this->OnClose.Broadcast(Host, Port_, Origin);
}

void AWebSocketServerActor::ErrorMessageCallback()
{
	this->OnError.Broadcast();
}