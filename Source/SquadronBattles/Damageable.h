// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Interface.h"
#include "Damageable.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI)
class UDamageable : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class SQUADRONBATTLES_API IDamageable
{
	GENERATED_IINTERFACE_BODY()

	public:
	
	/*UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void ApplyDamage(float Amount);

	UFUNCTION(Reliable, Server, WithValidation)
	virtual void ServerApplyDamage(float Amount);
	virtual void ServerApplyDamage_Implementation(float Amount);
	virtual bool ServerApplyDamage_Validate(float Amount);*/
};
