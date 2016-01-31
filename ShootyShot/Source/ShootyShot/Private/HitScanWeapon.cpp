// Fill out your copyright notice in the Description page of Project Settings.

#include "ShootyShot.h"
#include "HitScanWeapon.h"


// Sets default values for this component's properties
UHitScanWeapon::UHitScanWeapon()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHitScanWeapon::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHitScanWeapon::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

bool UHitScanWeapon::DoTrace(FHitResult * RV_Hit, FCollisionQueryParams * RV_TraceParams)
{
	//get camera transform
	FVector CameraLoc = GetOwner()->GetActorLocation();
	FRotator CameraRot = GetOwner()->GetActorRotation();

	FVector start = CameraLoc;
	FVector end = CameraLoc + (CameraRot.Vector() * castDistance);
	
	RV_TraceParams->bTraceComplex = true;
	RV_TraceParams->bTraceAsyncScene = true;
	RV_TraceParams->bReturnPhysicalMaterial = true;

	//do the line cast
	bool trace = GetWorld()->LineTraceSingle(*RV_Hit, start, end, ECC_Pawn, *RV_TraceParams);

	return trace;
}

float UHitScanWeapon::Fire()
{
	return 0.0f;
}

