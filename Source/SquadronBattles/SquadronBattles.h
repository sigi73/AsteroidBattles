// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"

UENUM(BlueprintType)
enum class ETurnType : uint8
{
	TURN_Stationary  UMETA(DisplayName = "Stationary"),
	TURN_Moving      UMETA(DisplayName = "Moving")
};