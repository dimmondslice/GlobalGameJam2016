// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "FoodItem.generated.h"

UCLASS()
class SHOOTYSHOT_API AFoodItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFoodItem();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere)
	USceneComponent* visibleComp;

    /*
    UFUNCTION(BlueprintNativeEvent, Category = "Network Physics")
    void NetworkAddImpulse();
    */
};
