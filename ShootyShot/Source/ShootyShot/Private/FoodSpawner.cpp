// Fill out your copyright notice in the Description page of Project Settings.

#include "ShootyShot.h"
#include "FoodSpawner.h"


// Sets default values
AFoodSpawner::AFoodSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent 

}

// Called when the game starts or when spawned
void AFoodSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFoodSpawner::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

