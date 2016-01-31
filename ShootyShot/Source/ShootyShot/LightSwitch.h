// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "LightSwitch.generated.h"

UCLASS()
class SHOOTYSHOT_API ALightSwitch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALightSwitch();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleAnywhere, Category = "Switch Components")
	class UPointLightComponent* PointLight1;

	UPROPERTY(VisibleAnywhere, Category = "Switch Components")
	class UCapsuleComponent* Caps;

	UFUNCTION()
		void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void ToggleLight();

	UPROPERTY(VisibleAnywhere, Category = "SwitchVariables")
		float DesiredIntensity;

	UPROPERTY()
		TArray<FString> RemainingIngredients;
};
