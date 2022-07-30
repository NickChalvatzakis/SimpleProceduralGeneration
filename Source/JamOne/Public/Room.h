// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomWall.h"
#include "RoomGate.h"
#include "UtilityLibrary.h"

#include "Room.generated.h"

UCLASS()
class JAMONE_API ARoom : public AActor
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere, Category = "Room Data")
	int32 RoomID;
	UPROPERTY(EditAnywhere, Category = "Room Data")
	FIntVector RoomPosition;
	UPROPERTY(EditAnywhere, Category = "Room Data")
	FIntVector RoomSize;
	UPROPERTY(EditAnywhere, Category = "Room Data")
	FIntVector RoomSizeInUnits;
	UPROPERTY(EditAnywhere, Category = "Room Data")
	uint8 Axis;

	UPROPERTY(EditAnywhere, Category = "Room Data|Tiles")
	TSubclassOf<ARoomWall> Wall;
	UPROPERTY(EditAnywhere, Category = "Room Data|Tiles")
	TArray<ARoomWall*> Walls;
	UPROPERTY(EditAnywhere, Category = "Room Data|Tiles")
	TSubclassOf<ARoomGate> Gate;
	ARoomGate* SpawnedGate;


public:

	UPROPERTY(EditAnywhere, Category = "Room Settings")
	UStaticMeshComponent* Floor;

	UFUNCTION(BlueprintCallable)
	int32 GetRoomID();
	UFUNCTION(BlueprintCallable)
	int32 GetAxis();
	UFUNCTION(BlueprintCallable)
	FIntVector GetRoomPosition();
	UFUNCTION(BlueprintCallable)
	FIntVector GetRoomSize();
	UFUNCTION(BlueprintCallable)
	void SetRoomID(int32 id);
	UFUNCTION(BlueprintCallable)
	void SetAxis(uint8 a);
	UFUNCTION(BlueprintCallable)
	void SetRoomSizeUnits(int32 sizeX, int32 sizeY);
	UFUNCTION(BlueprintCallable)
	FIntVector GetRoomSizeUnits();
	UFUNCTION(BlueprintCallable, CallInEditor)
	void GenerateWalls();
	UFUNCTION(BlueprintCallable, CallInEditor)
	void DestroyWalls();
	UFUNCTION(BlueprintCallable)
	void GenerateGate(FVector GatePosition, FRotator GateRotation);
	UFUNCTION(BlueprintCallable)
	float GetDifBasedOnAxis(float Offset, bool isX);
	float GetDifBasedOnAxis(float A, float B);


public:	
	// Sets default values for this actor's properties
	ARoom();	
	virtual void BeginDestroy() override;

public:	

	void GenerateRoom(int32 MinRoomSize, int32 MaxRoomSize, int32 MapSize);
	void GenerateNextRoom(int32 MinRoomSize, int32 MaxRoomSize, int32 MapSize, ARoom* prevRoom,int32 seed);
	ARoom* Instantiate(UWorld* World);
	TArray<ARoomWall*> GetWalls();
};

