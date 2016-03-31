// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Interface.h"
#include "Damageable.generated.h"

/**
 * 
 */
//UINTERFACE(Blueprintable, meta=(CannotImplementInterfaceInBlueprint))
UINTERFACE(Blueprintable, MinimalAPI)
class UDamageable : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IDamageable
{
	GENERATED_IINTERFACE_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Testing")
	void ApplyDamage(float Amount);
};
