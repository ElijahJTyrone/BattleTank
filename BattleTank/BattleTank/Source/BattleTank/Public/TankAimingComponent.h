// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"



UENUM()
enum class EFiringStatus : uint8
{
	Reloading,
	Aiming,
	Locked,
	OutOfAmmo
};

class UTankBarrel; //Forward Declaration
class UTankTurret;
class AProjectile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);		

	void AimAt(FVector HitLocation);

	UFUNCTION(BlueprintCallable)
	void Fire();

	EFiringStatus GetFiringStatus() const;

	UFUNCTION(BlueprintCallable, Category = "Setup")
	int32 GetRoundsLeft() const;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "State")
	EFiringStatus FiringStatus = EFiringStatus::Reloading;


private:
	// Sets default values for this component's properties
	UTankAimingComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	virtual void BeginPlay() override;

	void MoveBarrelTowards(FVector AimDirection);

	bool IsBarrelMoving();

	UTankBarrel* Barrel = nullptr;

	UTankTurret* Turret = nullptr;	
	
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float LaunchSpeed = 4000;	

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AProjectile> ProjectileBlueprint;	

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float ReloadTime = 3;
	double LastFireTime = 0;

	FVector DesiredAimDirection;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	int32 RoundsLeft = 3;
};
