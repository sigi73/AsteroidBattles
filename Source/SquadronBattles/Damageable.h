// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Interface.h"
#include "Damageable.generated.h"

/**
 * 
 */
UINTERFACE(Blueprintable, meta=(CannotImplementInterfaceInBlueprint))
class UDamageable : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class SQUADRONBATTLES_API IDamageable
{
	GENERATED_IINTERFACE_BODY()

	public:
	
	UFUNCTION(BlueprintCallable, Category = "Stuff and things")
	void ApplyDamage(float Amount);

	UFUNCTION(BlueprintCallable, Reliable, Server, WithValidation, Category = "Stuff and things")
	void ServerApplyDamage(float Amount);
	void ServerApplyDamage_Implementation(float Amount);
	bool ServerApplyDamage_Validate(float Amount);
};
