// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "Tank.h"
#include "TankAimingComponent.h"

void ATankPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn)
	{
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) { return; }
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::PosssessedTankDeath);
	}
}

void ATankPlayerController::PosssessedTankDeath()
{
	StartSpectatingOnly();
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }
	
	FoundAimingComponent(AimingComponent);
		
}

void ATankPlayerController::Tick(float deltaTime)
{
	Super::Tick( deltaTime );
	AimTowardsCrosshair();
}


void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetPawn()) { return; } //If not possessing

	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }

	FVector HitLocation;
	bool bGotHitLocation = GetSightRayHitLocation(HitLocation);	
	if (bGotHitLocation)
	{
		AimingComponent->AimAt(HitLocation);
	}
	
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation) const
{
	int32 viewPortSizeX, viewPortSizeY;
	GetViewportSize(viewPortSizeX, viewPortSizeY);
	auto ScreenLocation = FVector2D(viewPortSizeX * CrossHairXLocation, viewPortSizeY * CrossHairYLocation);
	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection))
	{		
		return GetLookVectorHitLocation(LookDirection, HitLocation);
	}	
	return false;
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const
{
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (LookDirection * LineTraceRange);
	if (GetWorld()->LineTraceSingleByChannel(
			HitResult, StartLocation, 
		EndLocation, ECollisionChannel::ECC_Visibility))
	{
		HitLocation = HitResult.Location;
		return true;
	}
	HitLocation = FVector(0);
	return false; //Line trace didn't succeed
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{
	FVector CameraWorldLocation;	
	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldLocation, LookDirection);
	
}