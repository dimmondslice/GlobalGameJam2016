// Fill out your copyright notice in the Description page of Project Settings.

#include "ShootyShot.h"
#include "LightSwitch.h"


// Sets default values
ALightSwitch::ALightSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DesiredIntensity = 3000.0f;
	RemainingIngredients.Add(TEXT("Egg"));
	RemainingIngredients.Add(TEXT("Banana"));


	PointLight1 = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight1"));
	PointLight1->Intensity = DesiredIntensity;
	PointLight1->bVisible = true;
	RootComponent = PointLight1;

	Caps = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule1"));
	Caps->InitCapsuleSize(250.0f, 5.0f);
	Caps->AttachParent = RootComponent;

	Caps->OnComponentBeginOverlap.AddDynamic(this, &ALightSwitch::OnOverlapBegin);       // set up a notification for when this component overlaps something
	Caps->OnComponentEndOverlap.AddDynamic(this, &ALightSwitch::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ALightSwitch::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALightSwitch::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ALightSwitch::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Collected a(n): %s"), *OtherActor->GetClass()->GetName());
		if (RemainingIngredients.Contains(OtherActor->GetClass()->GetName()))
		{
			RemainingIngredients.RemoveSingle(OtherActor->GetClass()->GetName());
		}
		UE_LOG(LogTemp, Warning, TEXT("Number of Ingredients Remaining: %d"), RemainingIngredients.Num());
		OtherActor->Destroy();

	}
}

void ALightSwitch::OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		ToggleLight();
	}
}

void ALightSwitch::ToggleLight()
{
	PointLight1->ToggleVisibility();
}

