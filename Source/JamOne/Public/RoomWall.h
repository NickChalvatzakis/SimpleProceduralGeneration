// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomWall.generated.h"

UCLASS()
class JAMONE_API ARoomWall : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Wall Data")
	FVector WallPosition;

	
public:	
	// Sets default values for this actor's properties
	ARoomWall();


	UFUNCTION(BlueprintCallable)
	void SetPosition(FVector newPos);
	UFUNCTION(BlueprintCallable)
	FVector GetPosition();


	


};
