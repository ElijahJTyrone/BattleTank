// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "BattleTank.h"
#include "Tank.h"  //To implement OnDeath
#include "TankAimingComponent.h"




void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.bCanEverTick = true;
}

void ATankAIController::Tick(float deltaTime)
{	
	Super::Tick(deltaTime);

	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	auto ControlledTank = GetPawn();

	if (!ensure(PlayerTank && ControlledTank)) { return; }
	
		MoveToActor(PlayerTank, AcceptanceRadius);
		auto AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
		AimingComponent->AimAt(PlayerTank->GetActorLocation());
		if (AimingComponent->GetFiringStatus() == EFiringStatus::Locked)
		{
			AimingComponent->Fire();
		}
		

}

void ATankAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn)
	{
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) { return; }
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::PosssessedTankDeath);
	}
}

void ATankAIController::PosssessedTankDeath()
{
	if(!GetPawn()) { return; }
	GetPawn()->DetachFromControllerPendingDestroy();
}
