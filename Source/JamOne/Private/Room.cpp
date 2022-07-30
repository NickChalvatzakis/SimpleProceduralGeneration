// Fill out your copyright notice in the Description page of Project Settings.


#include "Room.h"

#include "CustomLogs.h"
#include "Kismet/GameplayStatics.h"
#include "UtilityLibrary.h"


// Sets default values
ARoom::ARoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Floor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tile"));
	SetRootComponent(Floor);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("StaticMesh'/Game/Mesh/Tile.Tile'"));
	checkf(mesh.Succeeded(), TEXT("Failed to find mesh"));

	Floor->SetStaticMesh(mesh.Object);
	//Floor->SetMaterial(0, Material_Blue.Object);
	Floor->SetRelativeScale3D(FVector::OneVector);
	this->RoomSizeInUnits = FIntVector(100,100,0);
}

void ARoom::BeginDestroy()
{
	DestroyWalls();
	Super::BeginDestroy();
}


void ARoom::SetRoomSizeUnits(int32 sizeX, int32 sizeY)
{
	this->RoomSizeInUnits = FIntVector(sizeX, sizeY, 0);
}

FIntVector ARoom::GetRoomSizeUnits()
{
	return this->RoomSizeInUnits;
}


void ARoom::GenerateRoom(int32 MinRoomSize, int32 MaxRoomSize, int32 MapSize)
{
	RoomSize = FIntVector(FMath::RandRange(MinRoomSize, MaxRoomSize) , FMath::RandRange(MinRoomSize, MaxRoomSize) , 0);
	//const uint8 posX = MapSize - RoomSize.X, posY = MapSize - RoomSize.Y;

	RoomPosition = FIntVector(0, 0, 0);

}

void ARoom::GenerateNextRoom(int32 MinRoomSize, int32 MaxRoomSize, int32 MapSize, ARoom* prevRoom, int32 seed)
{
	
	const uint8 AxisSelector = BernoulliRandomVariable<uint8>(0,1); // 0 for x axis and 1 for y axis
	LogInfo("Axis " + FString::FromInt(AxisSelector));
	const int8 EdgeSelector = BernoulliRandomVariable<int8>(-1, 1); // -1 and 1 to choose which side of the axis we want to work on
	LogInfo("Edge " + FString::FromInt(EdgeSelector));
	SetAxis(AxisSelector);
	if (AxisSelector == 0)
	{
		RoomSize = FIntVector(1, FMath::RandRange(MinRoomSize, MaxRoomSize), 0);

		float RandomPlacementOnEdge = FMath::RandRange(-1, 1) * (prevRoom->GetRoomSize().Y * GetRoomSizeUnits().Y) ;
		//LogInfo("RandomPlacement " + FString::FromInt(RandomPlacementOnEdge));
		//offset by half, if the size is odd else offset by a full tile
		const float Offset = EdgeSelector * ((prevRoom->GetRoomSize().X % 2 == 0) ? ( GetRoomSizeUnits().X * 3/2) : GetRoomSizeUnits().X);
		RoomPosition = FIntVector(prevRoom->GetRoomPosition().X + (EdgeSelector *  Offset * prevRoom->GetRoomSize().Y) , prevRoom->GetRoomPosition().Y + RandomPlacementOnEdge, 0 );
		LogInfo(RoomPosition.ToString());
		
		LogInfo("X axis");
	}
	else
	{
		RoomSize = FIntVector(FMath::RandRange(MinRoomSize, MaxRoomSize), 1, 0);

		float RandomPlacementOnEdge = FMath::RandRange(-1, 1) * (prevRoom->GetRoomSize().X * GetRoomSizeUnits().X) ;
		//LogInfo("RandomPlacement " + FString::FromInt(RandomPlacementOnEdge));

		//offset by half, if the size is odd else offset by a full tile
		const float Offset = EdgeSelector * ((prevRoom->GetRoomSize().Y % 2 == 0) ? (  GetRoomSizeUnits().Y * 3/2) : GetRoomSizeUnits().Y);
		RoomPosition = FIntVector(prevRoom->GetRoomPosition().X + RandomPlacementOnEdge,  prevRoom->GetRoomPosition().Y + (EdgeSelector *  Offset * RoomSize.Y) , 0);
		LogInfo(RoomPosition.ToString());
		LogInfo("Y axis");
	}

}


void ARoom::SetRoomID(int32 id)
{
	this->RoomID = id;
}

void ARoom::SetAxis(uint8 axis)
{
	this->Axis = axis;
}

int32 ARoom::GetRoomID()
{
	return this->RoomID;
}

int32 ARoom::GetAxis()
{
	return this->Axis;
}

FIntVector ARoom::GetRoomPosition()
{
	return this->RoomPosition;
}

FIntVector ARoom::GetRoomSize()
{
	return this->RoomSize;
}

void ARoom::DestroyWalls()
{
	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARoomWall::StaticClass(), FoundActors);
	for (const auto& wall : FoundActors)
	{
		wall->Destroy();
	}
	this->Walls.Empty(0);
}


void ARoom::GenerateWalls()
{
	//if (WallPositions.Num() > 0) {
	//	WallPositions.Empty(0);
	//	DestroyWalls();
	//}
	float Offset_X = GetRoomPosition().X + GetRoomSize().X * GetRoomSizeUnits().X / 2 - 5.f;
	float Offset_Y = GetRoomPosition().Y + GetRoomSize().Y * GetRoomSizeUnits().Y / 2 - 5.f; 
	int32 TilesToPlace_X = GetRoomSize().X * (GetRoomSizeUnits().X / 10);
	int32 TilesToPlace_Y = GetRoomSize().Y * (GetRoomSizeUnits().Y / 10);

	for (int i = 0; i < TilesToPlace_X + 1; i++)
	{
		ARoomWall* tempRoom = NewObject<ARoomWall>();
		tempRoom->SetPosition(FVector(Offset_X, Offset_Y, 0));

		ARoomWall* southWall = GetWorld()->SpawnActor<ARoomWall>(Wall, tempRoom->GetPosition(), FRotator::ZeroRotator);
		southWall->SetPosition(tempRoom->GetPosition());
		Walls.AddUnique(southWall);

		tempRoom->SetPosition(FVector(Offset_X, GetDifBasedOnAxis(Offset_Y, false), 0));
	
		ARoomWall* northWall = GetWorld()->SpawnActor<ARoomWall>(Wall, tempRoom->GetPosition(), FRotator::ZeroRotator);
		northWall->SetPosition(tempRoom->GetPosition());
		Walls.AddUnique(northWall);


		Offset_X -= GetRoomSizeUnits().X / 10;


	}

	Offset_X = GetRoomPosition().X + GetRoomSize().X * GetRoomSizeUnits().X / 2 - 5.f;
	Offset_Y = GetRoomPosition().Y + GetRoomSize().Y * GetRoomSizeUnits().Y / 2 - 5.f;
	for (int i = 0; i < TilesToPlace_Y + 1; i++)
	{
		ARoomWall* tempRoom = NewObject<ARoomWall>();
		tempRoom->SetPosition(FVector(Offset_X, Offset_Y, 0));

		ARoomWall* westWall = GetWorld()->SpawnActor<ARoomWall>(Wall, tempRoom->GetPosition(), FRotator::ZeroRotator);
		westWall->SetPosition(tempRoom->GetPosition());
		Walls.AddUnique(westWall);

		tempRoom->SetPosition(FVector(GetDifBasedOnAxis(Offset_X, true), Offset_Y, 0));

		ARoomWall* eastWall = GetWorld()->SpawnActor<ARoomWall>(Wall, tempRoom->GetPosition(), FRotator::ZeroRotator);
		eastWall->SetPosition(tempRoom->GetPosition());
		Walls.AddUnique(eastWall);

		Offset_Y -= GetRoomSizeUnits().X / 10;
	}

	//GEngine->ForceGarbageCollection(true);



	/*southWall->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	northWall->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);*/
}


void ARoom::GenerateGate(FVector GatePosition, FRotator GateRotation)
{
	SpawnedGate = GetWorld()->SpawnActor<ARoomGate>(Gate, GatePosition, GateRotation);
}

float ARoom::GetDifBasedOnAxis(float Offset, bool isX)
{
	/*if(isX) return (GetRoomPosition().X >= 0 ? Offset - (GetRoomSize().X * GetRoomSizeUnits().X ): (GetRoomSize().X * GetRoomSizeUnits().X) - Offset);
	return GetRoomPosition().Y >= 0 ? Offset - (GetRoomSize().Y * GetRoomSizeUnits().Y) : (GetRoomSize().Y * GetRoomSizeUnits().Y) - Offset;*/
	if (isX)  return Offset - (GetRoomSize().X * GetRoomSizeUnits().X);
	return Offset - (GetRoomSize().Y * GetRoomSizeUnits().Y);
}
float ARoom::GetDifBasedOnAxis(float A, float B)
{
	return A - (A - B) / 2;
	//return A >= 0 ? A - (A - B) / 2 : A + (A + B) /2;
	//return A.Y >= 0 ? A.Y - (A.Y - B.Y) / 2 : A.Y + (A.Y + B.Y) / 2;
}


ARoom* ARoom::Instantiate(UWorld* world)
{
	if(this == nullptr)
	{
		LogError("Failed to instantiate room");
		return nullptr;
	}

	ARoom* tempRoom;
	tempRoom = world->SpawnActor<ARoom>(ARoom::StaticClass(), FVector(GetRoomPosition()), FRotator::ZeroRotator);
	tempRoom->RoomPosition = this->RoomPosition;
	tempRoom->RoomSize = this->RoomSize;
	tempRoom->RoomID = this->RoomID;
	tempRoom->Axis = this->Axis;
	tempRoom->SetActorScale3D(FVector(tempRoom->GetRoomSize()));


	return tempRoom;
}

TArray<ARoomWall*> ARoom::GetWalls()
{
	return this->Walls;
}
