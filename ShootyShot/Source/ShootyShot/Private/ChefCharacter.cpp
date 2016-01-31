// Fill out your copyright notice in the Description page of Project Settings.

#include "ShootyShot.h"
#include "ChefCharacter.h"


// Sets default values
AChefCharacter::AChefCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChefCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChefCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AChefCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

FString AChefCharacter::DoTrace()
{
	//UE_LOG(LogTemp, Warning, TEXT("hey it didn't crash"));

	
	FHitResult* RV_Hit = new FHitResult();
	FCollisionQueryParams* RV_TraceParams = new FCollisionQueryParams();

	//get camera transform
	FVector CameraLoc = GetActorLocation();
	FRotator CameraRot = GetActorRotation();

	FVector start = CameraLoc;
	FVector end = CameraLoc + (CameraRot.Vector() * castDistance);

	RV_TraceParams->bTraceComplex = true;
	RV_TraceParams->bTraceAsyncScene = true;
	RV_TraceParams->bReturnPhysicalMaterial = true;

	//do the line cast
	bool trace = GetWorld()->LineTraceSingle(*RV_Hit, start, end, ECC_Pawn, *RV_TraceParams);

	//UE_LOG(LogTemp, Warning, TEXT("You fired a ray and hit: %s"), *RV_Hit->Actor->GetName());
	return RV_Hit->Actor->GetName();
}