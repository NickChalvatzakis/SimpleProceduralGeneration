// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Room.h"
#include "ProceduralGenerator.generated.h"

UCLASS()
class JAMONE_API AProceduralGenerator : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Rooms")
	TArray<ARoom*> RoomArray;
	
public:	
	// Sets default values for this actor's properties
	AProceduralGenerator();

	UPROPERTY(EditAnywhere, Category = "Generation|Generation Data")
	int32 MaxRoomNumber;
	UPROPERTY(EditAnywhere, Category = "Generation|Generation Data")
	int32 NumberOfRoomRetries;
	UPROPERTY(EditAnywhere, Category = "Generation|Room Settings")
		TSubclassOf<ARoomGate> Gate;

	UPROPERTY(EditAnywhere, Category = "Generation|Generation Settings")
		int32 MapSize;
	UPROPERTY(EditAnywhere, Category = "Generation|Room Settings")
		int32 MinRoomSize;
	UPROPERTY(EditAnywhere, Category = "Generation|Room Settings")
		int32 MaxRoomSize;
	/*Map showing the direction of rooms, the bool is true if the direction is towards the X or false towards the Y axis*/
	UPROPERTY(EditAnywhere, Category = "Generation|Room Graph")
		TArray<FVector> RoomGraphPositions;
	UPROPERTY(EditAnywhere, Category = "Generation|Room Graph")
		TArray<bool> RoomGraphAxis;

	double secs = FTimespan(FDateTime::Now().GetTicks()).GetTotalSeconds();
	UPROPERTY(EditAnywhere, Category = "Generation|Generation Data")
	int32 seed = (int32)(((int64)secs) % INT_MAX);
	

	UFUNCTION(CallInEditor, Category = "Generation")
	void ClearRooms();
	UFUNCTION(CallInEditor, Category = "Generation")
	void GenerateLevel();
	void PlaceRoom(int32 retries, ARoom* newRoom);


	void CreateDirectionalGraph();
	UFUNCTION(CallInEditor, Category = "Generation|Room Graph")
	void DestroyGates();
	UFUNCTION(CallInEditor, Category = "Generation|Room Graph")
	void CleanUpDuplicateWalls();
	UFUNCTION(CallInEditor, Category = "Generation|Room Graph")
	void PlaceGate();

	bool isRoomOverlapping(ARoom* newRoom);



	

};
