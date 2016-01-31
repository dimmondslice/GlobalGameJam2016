// Fill out your copyright notice in the Description page of Project Settings.

#include "ShootyShot.h"
#include "Pot.h"





// Sets default values
APot::APot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere1 = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere1"));
	Sphere1->InitSphereRadius(250.0f);
	Sphere1->AttachParent = RootComponent;

	Sphere1->OnComponentBeginOverlap.AddDynamic(this, &APot::OnOverlapBegin);
}

// Called when the game starts or when spawned
void APot::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APot::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void APot::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("triggered"));
}

void OnActorBeginOverlap()
{
	UE_LOG(LogTemp, Warning, TEXT("triggered"));
}

