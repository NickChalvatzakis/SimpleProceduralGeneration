// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomLogs.h"

DEFINE_LOG_CATEGORY(JamOneLog);

void LogInfo(FString message, bool showOnScreen, float duration)
{
	UE_LOG(JamOneLog, Log, TEXT("%s"), *message);

	if(showOnScreen)
	{
		GEngine->AddOnScreenDebugMessage(-1, duration, FColor::White, message);
	}
}

void LogWarning(FString message, bool showOnScreen, float duration)
{
	UE_LOG(JamOneLog, Warning, TEXT("%s"), *message);

	if (showOnScreen)
	{
		GEngine->AddOnScreenDebugMessage(-1, duration, FColor::Yellow, message);
	}
}

void LogError(FString message, bool showOnScreen, float duration)
{
	UE_LOG(JamOneLog, Error, TEXT("%s"), *message);

	if (showOnScreen)
	{
		GEngine->AddOnScreenDebugMessage(-1, duration, FColor::Red, message);
	}
}
