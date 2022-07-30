// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(JamOneLog, Log, All);

void LogInfo(FString message, bool showOnScreen = true, float duration = 5.0);
void LogWarning(FString message, bool showOnScreen = true, float duration = 5.0);
void LogError(FString message, bool showOnScreen = true, float duration = 5.0);