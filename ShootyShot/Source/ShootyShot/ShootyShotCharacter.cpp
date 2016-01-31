// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "ShootyShot.h"
#include "ShootyShotCharacter.h"
#include "ShootyShotProjectile.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/InputSettings.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AShootyShotCharacter

AShootyShotCharacter::AShootyShotCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->AttachParent = GetCapsuleComponent();
	FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->AttachParent = FirstPersonCameraComponent;
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	FP_Gun->AttachTo(Mesh1P, TEXT("GripPoint"), EAttachLocation::SnapToTargetIncludingScale, true);


	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 30.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P are set in the
	// derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	PhysicsHandleActive = false;
	Held = false;
	OtherItemLocation = 0.0;
	PickupDistance = 500;
	HandleLocation = FVector(0.0, 0.0, 0.0);
	OtherRotation = FRotator(0.0, 0.0, 0.0);
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
	//PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
	//PhysicsObject;
	//PhysicsObjectTypes;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AShootyShotCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	//InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AShootyShotCharacter::TouchStarted);
	if( EnableTouchscreenMovement(InputComponent) == false )
	{
		InputComponent->BindAction("Fire", IE_Pressed, this, &AShootyShotCharacter::OnFire);
	}
	
	InputComponent->BindAxis("MoveForward", this, &AShootyShotCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AShootyShotCharacter::MoveRight);
	
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AShootyShotCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AShootyShotCharacter::LookUpAtRate);
}

void AShootyShotCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PhysicsHandleActive)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Setting Position"));
		
		FVector HandleLocation = GetControlRotation().Vector() * OtherItemLocation + FVector(0, 0, 50);
		PhysicsHandle->SetTargetLocationAndRotation(HandleLocation, GetControlRotation());
	}
}



void AShootyShotCharacter::GrabThrow(AActor* ActorToIgnore)
{
	if (Held)
	{
		PhysicsHandleActive = false;
		//Out.Component->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		PhysicsHandle->ReleaseComponent();
	}
	else
	{
		FHitResult Out;
		FVector Start = FirstPersonCameraComponent->GetComponentLocation();
		FVector End = FirstPersonCameraComponent->GetForwardVector() * PickupDistance + Start;
		//AActor* ActorToIgnore = GetOwner();
		//if(ActorToIgnore)
		//UE_LOG(LogTemp, Warning, TEXT("Ignoring"));//, *ActorToIgnore->GetName());

		FCollisionQueryParams Params(FName(TEXT("Food Trace")), true, ActorToIgnore);

		Params.AddIgnoredActor(ActorToIgnore);

		//Params->bTraceComplex = true;
		//Params->bTraceAsyncScene = true;
		//Params->bReturnPhysicalMaterial = true;

		if (GetWorld()->LineTraceSingleByChannel(Out, Start, End, ECC_Visibility))
		{
			PhysicsHandleActive = true;
			UE_LOG(LogTemp, Warning, TEXT("Grabbed %s"), *Out.GetComponent()->GetName());

			PhysicsHandle->SetTargetLocation(GetTransform().GetLocation());
			PhysicsObject = Out.GetComponent();
			PhysicsHandle->GrabComponent(PhysicsObject, Out.BoneName, Out.Location, true);
			Out.Component->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		}

		OtherItemLocation = (GetActorLocation() - Out.Location).Size();
		OtherRotation = GetActorRotation();

	}
}

void AShootyShotCharacter::OnFire()
{ 
	/*if (Held)
	{
		PhysicsHandleActive = false;
		//Out.Component->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		PhysicsHandle->ReleaseComponent();
	}
	else
	{
		FHitResult Out;
		FVector Start = FirstPersonCameraComponent->GetComponentLocation();
		FVector End = FirstPersonCameraComponent->GetForwardVector() * PickupDistance + Start;
		AActor* ActorToIgnore = GetOwner();
		//if(ActorToIgnore)
		//UE_LOG(LogTemp, Warning, TEXT("Ignoring"));//, *ActorToIgnore->GetName());

		FCollisionQueryParams Params(FName(TEXT("Food Trace")), true, ActorToIgnore);

		Params.AddIgnoredActor(ActorToIgnore);
		
		//Params->bTraceComplex = true;
		//Params->bTraceAsyncScene = true;
		//Params->bReturnPhysicalMaterial = true;
		
		if(GetWorld()->LineTraceSingleByChannel(Out, Start, End, ECC_PhysicsBody, Params))
		{
			PhysicsHandleActive = true;
			UE_LOG(LogTemp, Warning, TEXT("Grabbed %s"), *Out.GetComponent()->GetName());

			PhysicsHandle->SetTargetLocation(GetTransform().GetLocation());
			PhysicsObject = Out.GetComponent();
			PhysicsHandle->GrabComponent(PhysicsObject, Out.BoneName, Out.Location, true);
			Out.Component->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		}

		OtherItemLocation = (GetActorLocation() - Out.Location).Size();
		OtherRotation = GetActorRotation();
		
	}*/

	/*// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		const FRotator SpawnRotation = GetControlRotation();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(GunOffset);

		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			// spawn the projectile at the muzzle
			World->SpawnActor<AShootyShotProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
		}
	}*/

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if(FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if(AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}

}

void AShootyShotCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if( TouchItem.bIsPressed == true )
	{
		return;
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AShootyShotCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	if( ( FingerIndex == TouchItem.FingerIndex ) && (TouchItem.bMoved == false) )
	{
		OnFire();
	}
	TouchItem.bIsPressed = false;
}

void AShootyShotCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if ((TouchItem.bIsPressed == true) && ( TouchItem.FingerIndex==FingerIndex))
	{
		if (TouchItem.bIsPressed)
		{
			if (GetWorld() != nullptr)
			{
				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
				if (ViewportClient != nullptr)
				{
					FVector MoveDelta = Location - TouchItem.Location;
					FVector2D ScreenSize;
					ViewportClient->GetViewportSize(ScreenSize);
					FVector2D ScaledDelta = FVector2D( MoveDelta.X, MoveDelta.Y) / ScreenSize;									
					if (ScaledDelta.X != 0.0f)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.X * BaseTurnRate;
						AddControllerYawInput(Value);
					}
					if (ScaledDelta.Y != 0.0f)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.Y* BaseTurnRate;
						AddControllerPitchInput(Value);
					}
					TouchItem.Location = Location;
				}
				TouchItem.Location = Location;
			}
		}
	}
}

void AShootyShotCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AShootyShotCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AShootyShotCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AShootyShotCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool AShootyShotCharacter::EnableTouchscreenMovement(class UInputComponent* InputComponent)
{
	bool bResult = false;
	if(FPlatformMisc::GetUseVirtualJoysticks() || GetDefault<UInputSettings>()->bUseMouseForTouch )
	{
		bResult = true;
		InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AShootyShotCharacter::BeginTouch);
		InputComponent->BindTouch(EInputEvent::IE_Released, this, &AShootyShotCharacter::EndTouch);
		InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AShootyShotCharacter::TouchUpdate);
	}
	return bResult;
}
