// Fill out your copyright notice in the Description page of Project Settings.

#include "ShootyShot.h"
#include "ChefCharacter.h"
#include "ShootyShotProjectile.h"
#include "Animation/AnimInstance.h"
//#include "GameFramework/InputSettings.h"

// Sets default values
AChefCharacter::AChefCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);



    // Create a CameraComponent	
    FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCameraComponent->AttachParent = GetCapsuleComponent();
    FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 64.f); // Position the camera
    FirstPersonCameraComponent->bUsePawnControlRotation = true;



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

// Called when the game starts or when spawned
void AChefCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChefCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );


    if (PhysicsHandleActive)
    {
        //UE_LOG(LogTemp, Warning, TEXT("Setting Position"));

        FVector HandleLocation = FirstPersonCameraComponent->GetComponentRotation().Vector() * OtherItemLocation + GetActorLocation() + FVector(0.0, 0.0, 50);//(GetControlRotation().Vector() * OtherItemLocation) + FVector(0, 0, 5);
        PhysicsHandle->SetTargetLocationAndRotation(HandleLocation, GetControlRotation());
    }

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
	bool trace = GetWorld()->LineTraceSingleByChannel(*RV_Hit, start, end, ECC_Pawn, *RV_TraceParams);

	//UE_LOG(LogTemp, Warning, TEXT("You fired a ray and hit: %s"), *RV_Hit->Actor->GetName());
	return RV_Hit->Actor->GetName();
}

void AChefCharacter::AddImpulse(UPrimitiveComponent* PhysicsObject, FVector ImpulseToAdd)
{
	if (PhysicsObject != NULL)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Launching"));
		PhysicsObject->AddImpulse(ImpulseToAdd);

		//succeeded = true;
	}
}

void AChefCharacter::HandleGrabbingStuff(FRotator rotation, bool & succeeded, bool & grabbing,/* FString& linetrace,*/ FVector& ImpulseToAdd, UPrimitiveComponent*& ObjectToFling) {
    FVector forwardVec = rotation.RotateVector(FVector(1.0f,0.0f,0.0f));
    //FVector forwardVec = rotation.Vector;

    if (PhysicsHandleActive)
    {
        grabbing = false;
        succeeded = false;

        PhysicsHandleActive = false;
        //Out.Component->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
        PhysicsHandle->ReleaseComponent();
        ImpulseToAdd = forwardVec * 500000;
		ObjectToFling = PhysicsObject;
		succeeded = true;
        /*if (PhysicsObject != NULL)
        {
            //UE_LOG(LogTemp, Warning, TEXT("Launching"));
            PhysicsObject->AddImpulse(ImpulseToAdd);

            succeeded = true;
        }*/
		//linetrace = "None";
    }
    else
    {
        FHitResult Out;
        FVector Start = FirstPersonCameraComponent->GetComponentLocation();
        FVector End = forwardVec * PickupDistance + Start;
        //AActor* ActorToIgnore;
        //if(ActorToIgnore)
        //UE_LOG(LogTemp, Warning, TEXT("Ignoring"));//, *ActorToIgnore->GetName());

        FCollisionQueryParams Params(FName(TEXT("Food Trace")), true, NULL);

        //Params.AddIgnoredActor(ActorToIgnore);

        //Params->bTraceComplex = true;
        //Params->bTraceAsyncScene = true;
        //Params->bReturnPhysicalMaterial = true;

		
        if (GetWorld()->LineTraceSingleByChannel(Out, Start, End, ECC_Visibility))
        {
			//linetrace = "Succeeded";
            PhysicsHandleActive = true;

            PhysicsHandle->SetTargetLocation(GetTransform().GetLocation());
            PhysicsObject = Out.GetComponent();
            PhysicsHandle->GrabComponent(PhysicsObject, Out.BoneName, Out.Location, true);
            //Out.Component->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

            succeeded = true;
            grabbing = true;
        }
        else {
            succeeded = false;
            grabbing = false;
			//linetrace = "Succeeded";
        }


        OtherItemLocation = (GetActorLocation() - Out.Location).Size();
        OtherRotation = GetActorRotation();

    }

}
