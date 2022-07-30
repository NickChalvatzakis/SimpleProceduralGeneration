// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralGenerator.h"


#include "CustomLogs.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProceduralGenerator::AProceduralGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AProceduralGenerator::ClearRooms()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARoom::StaticClass(), FoundActors);

	for(const auto& room : FoundActors)
	{
		room->Destroy();
		
	}

	RoomArray.Empty(0);
	
}


void AProceduralGenerator::GenerateLevel()
{
	//FMath::RandInit(seed);
	LogInfo("Generating Level");
	ClearRooms();
	for(int i = 0; i < MaxRoomNumber; i++)
	{
		LogInfo("Placing Rooms");
		PlaceRoom(NumberOfRoomRetries, nullptr);
	}
}

void AProceduralGenerator::PlaceRoom(int32 retries, ARoom* newRoom)
{
	LogInfo("Placing Room try : " + FString::FromInt(retries));
	while (retries > 0 && newRoom == nullptr) {
		newRoom = NewObject<ARoom>(this);
		

		if (RoomArray.Num() <= 0)
		{
			newRoom->GenerateRoom(MinRoomSize, MaxRoomSize, MapSize);
			newRoom->SetRoomID(0);
			newRoom = newRoom->Instantiate(GetWorld());
			RoomArray.Add(newRoom);
		}
		else
		{

			newRoom->GenerateNextRoom(MinRoomSize, MaxRoomSize, MapSize, RoomArray[RoomArray.Num() - 1], seed);
			if (!isRoomOverlapping(newRoom))
			{
				newRoom->SetRoomID(RoomArray.Num());
				newRoom = newRoom->Instantiate(GetWorld());
				RoomArray.Add(newRoom);

			}
			else PlaceRoom(--retries,nullptr);

		}
		FString infoString = FString(TEXT("Created Room ID : {0}  - Room Position : {1} - Room Size : {2}"));
		infoString = FString::Format(*infoString, { newRoom->GetRoomID(), newRoom->GetRoomPosition().ToString(), newRoom->GetRoomSize().ToString() });
		newRoom->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		GEngine->ForceGarbageCollection(true);

		
	}

}

void AProceduralGenerator::CreateDirectionalGraph()
{
	for(int i = 1; i < RoomArray.Num(); i++)
	{
		if(RoomArray[i]->GetAxis() == 0)
		{
			RoomGraphPositions.Emplace(FVector(RoomArray[i]->GetRoomPosition() - RoomArray[i - 1]->GetRoomPosition()));
			RoomGraphAxis.Emplace(true);
		}
		else
		{
			RoomGraphPositions.Emplace(FVector(RoomArray[i]->GetRoomPosition() - RoomArray[i - 1]->GetRoomPosition()));
			RoomGraphAxis.Emplace(false);
		}
		//RoomGraph[i-1] = FVector(RoomArray[i]->GetRoomPosition() - RoomArray[i - 1]->GetRoomPosition());
	}
}
void AProceduralGenerator::DestroyGates()
{
	RoomGraphPositions.Empty(0);
	RoomGraphAxis.Empty(0);

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARoomGate::StaticClass(), FoundActors);

	for (const auto& gate : FoundActors)
	{
		gate->Destroy();

	}

}

void AProceduralGenerator::CleanUpDuplicateWalls()
{
	for(int i = 1; i < RoomArray.Num(); i++)
	{
		for(const auto& wall : RoomArray[i]->GetWalls())
		{
			if (wall == nullptr) continue;
			for(const auto& prevWall : RoomArray[i-1]->GetWalls())
			{
				if(wall->GetPosition().Equals(prevWall->GetPosition()))
				{
					
					RoomArray[i]->GetWalls().RemoveSingleSwap(wall,true);
					wall->Destroy();
				}
			}
		}
	}
	GEngine->ForceGarbageCollection(true);
}

void AProceduralGenerator::PlaceGate()
{

	CreateDirectionalGraph();
	for(int i = 0; i < RoomGraphPositions.Num(); i++)
	{
		//Y Axis
		if(!RoomGraphAxis[i])
		{
			float MinDistance = 500.f;
			ARoomWall* minWall;
			//Due to CleanUpDuplicateWalls(), only the previous Room contains the walls we are looking for
			for(const auto& wall : RoomArray[i]->GetWalls())
			{
				if (wall == nullptr) continue;
				//Get the vector with min distance to the next room
				if(MinDistance > (FVector(RoomArray[i + 1]->GetRoomPosition()) - wall->GetPosition()).Size())
				{
					MinDistance = (FVector(RoomArray[i + 1]->GetRoomPosition()) - wall->GetPosition()).Size();
					minWall = wall;
				}
			}
			int minIndex = RoomArray[i]->GetWalls().Find(minWall);
			//next is minIndex + 2
			// position is the X - dif/2 
			float GatePosition_X = RoomArray[i]->GetDifBasedOnAxis(minWall->GetPosition().X, RoomArray[i]->GetWalls()[minIndex + 2]->GetPosition().X); // minWall->GetPosition().X - (minWall->GetPosition().X - RoomArray[i]->GetWalls()[minIndex + 2]->GetPosition().X / 2);
			FVector GatePosition = FVector(GatePosition_X, minWall->GetPosition().Y, 0);
			RoomArray[i]->GenerateGate(GatePosition, FRotator::ZeroRotator);
			minWall->Destroy();
			RoomArray[i]->GetWalls()[minIndex + 2]->Destroy();
			
			RoomArray[i]->GetWalls().RemoveAt(minIndex,1,true);
			RoomArray[i]->GetWalls().RemoveAt(minIndex + 2,1,true);
		

			
		}else // x axis
		{
			float MinDistance = 500.f;
			ARoomWall* minWall;
			//Due to CleanUpDuplicateWalls(), only the previous Room contains the walls we are looking for
			for (const auto& wall : RoomArray[i]->GetWalls())
			{
				if (wall == nullptr) continue;
				//Get the vector with min distance to the next room
				if (MinDistance > (FVector(RoomArray[i + 1]->GetRoomPosition()) - wall->GetPosition()).Size())
				{
					MinDistance = (FVector(RoomArray[i + 1]->GetRoomPosition()) - wall->GetPosition()).Size();
					minWall = wall;
				}
			}
			int minIndex = RoomArray[i]->GetWalls().Find(minWall);
			//next is minIndex + 2
			// position is the X - dif/2 
			float GatePosition_Y = RoomArray[i]->GetDifBasedOnAxis(minWall->GetPosition().Y, RoomArray[i]->GetWalls()[minIndex + 2]->GetPosition().Y);
			// minWall->GetPosition().Y - (minWall->GetPosition().Y - RoomArray[i]->GetWalls()[minIndex + 2]->GetPosition().Y / 2);
			FVector GatePosition = FVector(minWall->GetPosition().X, GatePosition_Y, 0);
			RoomArray[i]->GenerateGate(GatePosition, FRotator(0, -90.f,0));
			minWall->Destroy();
			RoomArray[i]->GetWalls()[minIndex + 2]->Destroy();

			RoomArray[i]->GetWalls().RemoveAt(minIndex,1,true);
			RoomArray[i]->GetWalls().RemoveAt(minIndex + 2,1,true);
		}
		
	}
	GEngine->ForceGarbageCollection(true);
	LogInfo("Cleaned up walls");
}

bool AProceduralGenerator::isRoomOverlapping(ARoom* newRoom)
{

	FIntVector APos, BPos, ASize, BSize;
	BPos = newRoom->GetRoomPosition();
	BSize = FIntVector((newRoom->GetRoomSize().X * newRoom->GetRoomSizeUnits().X) + BPos.X, (newRoom->GetRoomSize().Y * newRoom->GetRoomSizeUnits().Y) + BPos.Y, 0);

	bool overlap = true;
	for (int i = 0; i < RoomArray.Num(); i++)
	{
		overlap = true;
		ARoom* A = RoomArray[i];
		APos = A->GetRoomPosition();
		ASize = FIntVector((A->GetRoomSize().X * A->GetRoomSizeUnits().X) + APos.X, (A->GetRoomSize().Y * A->GetRoomSizeUnits().Y) + APos.Y, 0);


		if (APos.X < BSize.X) overlap =  false;
		if (ASize.X > BPos.X) overlap = false;
		if (APos.Y < BSize.Y) overlap = false;
		if (ASize.Y > BPos.Y) overlap = false;

	}

	return overlap;
}





