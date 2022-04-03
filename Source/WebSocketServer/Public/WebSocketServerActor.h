// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WebSocketServerRunnable.h"
#include "WebSocketServerActor.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReceiveMessageDelegate, FString, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReceiveJsonMessageDelegate, FString, Type, FString, Command, FString, brief);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FConnectMessageDelegate, FString, Host, int, Port, FString, Origin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCloseMessageDelegate, FString, Host, int, Port, FString, Origin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FErrorMessageDelegate);

UCLASS()
class WEBSOCKETSERVER_API AWebSocketServerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWebSocketServerActor();

	UPROPERTY(BlueprintAssignable, Category="WebSocketEvent")
	FReceiveMessageDelegate OnMessage;
	UPROPERTY(BlueprintAssignable, Category="WebSocketEvent")
	FConnectMessageDelegate OnConnect;
	UPROPERTY(BlueprintAssignable, Category="WebSocketEvent")
	FCloseMessageDelegate OnClose;
	UPROPERTY(BlueprintAssignable, Category="WebSocketEvent")
	FErrorMessageDelegate OnError;
	UPROPERTY(BlueprintAssignable, Category="WebSocketEvent")
	FReceiveJsonMessageDelegate OnJsonMessage;
	UFUNCTION(BlueprintCallable, Category="WebSocketEvent")
	void SendServerMessage(const FString &Message);
	UFUNCTION(BlueprintCallable, Category="WebSocketEvent")
	void SendServerJsonMessage(const FString &Type, const FString &Command = "", const FString &Brief = "");
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category="WebSocketServer")
	uint32 Port;

private:
	TSharedPtr<FRunnableThread> ThreadPtr;
	TSharedPtr<WebSocketServerRunnable> RunnablePtr;

	void ReceiveMessageCallback(const FString &Message);
	void ConnectMessageCallback(const FString &Host, const int Port_, const FString &Origin);
	void CloseMessageCallback(const FString &Host, const int Port_, const FString &Origin);
	void ErrorMessageCallback();
};
