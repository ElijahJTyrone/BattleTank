// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTurret.h"

void UTankTurret::RotateTurret(float RotationSpeed)
{
	auto ClampedSpeed = FMath::Clamp<float>(RotationSpeed, -1, 1);
	auto RotationChange = ClampedSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	auto RawNewRotation = RelativeRotation.Yaw + RotationChange;
	SetRelativeRotation(FRotator(0, RawNewRotation, 0));
}


