// Fill out your copyright notice in the Description page of Project Settings.

#include "SquadronBattles.h"
#include "Damageable.h"

UDamageable::UDamageable(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void IDamageable::ApplyDamage(float Amount)
{

}

void IDamageable::ServerApplyDamage(float Amount)
{

}

void IDamageable::ServerApplyDamage_Implementation(float Amount)
{

}

bool IDamageable::ServerApplyDamage_Validate(float Amount)
{
	return true;
}