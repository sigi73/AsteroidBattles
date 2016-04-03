// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Damageable.h"
#include "BaseShip.generated.h"

UCLASS()
class SQUADRONBATTLES_API ABaseShip : public APawn, public IDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseShip();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifeTimeProps) const;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal,
		FVector NormalImpulse, const FHitResult& Hit) override;


	void ThrustInput(float Magnitude);
	void VerticalInput(float Magnitude);
	void HorizontalInput(float Magnitude);
	void RollInput(float Magnitude);


	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void FireWeapon();

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerFireWeapon();
	virtual void ServerFireWeapon_Implementation();
	virtual bool ServerFireWeapon_Validate();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	float FiringOffset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	TSubclassOf<class ABaseProjectile>ProjectileClass;



	
	virtual void ApplyDamage_Implementation(float Amount) override;

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerApplyDamage(float Amount);
	virtual void ServerApplyDamage_Implementation(float Amount);
	virtual bool ServerApplyDamage_Validate(float Amount);
	
	

protected:
	UPROPERTY(Category = "Movement", EditAnywhere)
	float Acceleration;

	UPROPERTY(Category = "Movement", EditAnywhere)
	float TurnSpeed;

	UPROPERTY(Category = "Movement", EditAnywhere)
	float MaxSpeed;

	UPROPERTY(Category = "Movement", EditAnywhere)
	float MinSpeed;

	UPROPERTY(Replicated)
	float CurrentForwardSpeed;
	UPROPERTY(Replicated)
	FRotator CurrentTurningSpeed;
	


	void MoveShip(FVector LocationOffset, FRotator RotationOffset);
	
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerMoveShip(FVector LocationOffset, FRotator RotationOffset);
	virtual void ServerMoveShip_Implementation(FVector LocationOffset, FRotator RotationOffset);
	virtual bool ServerMoveShip_Validate(FVector LocationOffset, FRotator RotationOffset);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Ship")
	float Health;
};
