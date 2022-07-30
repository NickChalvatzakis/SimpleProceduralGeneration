// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/* Generate a bernoulli random variable (Coin flip) */
template <typename T> 
T BernoulliRandomVariable(T x, T y)
{
	float b = FMath::FRandRange(0, 1);
	if (b >= 0.5f) return x;
	return y;
}



///* Check if the array has succeeded adding an item */
//template <typename T>
//bool ArraySucceeded(TArray<T> arr, int previousNum)
//{
//	return (arr.Num() > previousNum) ? true : false;
//}
//


////FVector Min(const FVector& A, const FVector& B)
////{
////	return (A < B) ? A : B;
////}
//
//FIntVector Min(const FIntVector& A, const FIntVector& B)
//{
//	return FIntVector(FMath::Min(A.X, B.X), FMath::Min(A.Y, B.Y), 0);
//}
//
////FVector Max(const FVector& A, const FVector& B)
////{
////
////}
//FIntVector Max(const FIntVector& A, const FIntVector& B)
//{
//	return FIntVector(FMath::Max(A.X, B.X), FMath::Max(A.Y, B.Y), 0);
//}