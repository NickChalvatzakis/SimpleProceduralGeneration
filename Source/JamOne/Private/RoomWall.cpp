// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomWall.h"

#include "CustomLogs.h"

// Sets default values
ARoomWall::ARoomWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall Mesh"));
	//SetRootComponent(WallMesh);

	//static ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("StaticMesh'/Game/Mesh/WallCube.WallCube'"));
	//checkf(mesh.Succeeded(), TEXT("Failed to find mesh"));

	//WallMesh->SetStaticMesh(mesh.Object);
	////WallMesh->SetMaterial(0, Material_Blue.Object);
	//WallMesh->SetRelativeScale3D(FVector::OneVector);
}




void ARoomWall::SetPosition(FVector newPos)
{
	this->WallPosition = newPos;
}

FVector ARoomWall::GetPosition()
{
	return this->WallPosition;
}



