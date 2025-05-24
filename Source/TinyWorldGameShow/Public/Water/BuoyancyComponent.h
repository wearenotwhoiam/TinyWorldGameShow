// RnDumb Fun - GameDev.TV Game Jam 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuoyancyComponent.generated.h"

class UWaveManagerComponent;
class UPrimitiveComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TINYWORLDGAMESHOW_API UBuoyancyComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBuoyancyComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category = "Buoyancy")
	float BuoyancyStrength = 150000.f;

	UPROPERTY(EditAnywhere, Category = "Buoyancy")
	float DampingStrength = 8000.f;

	UPROPERTY(EditAnywhere, Category = "Buoyancy")
	float MaxBuoyancyForce = 200000.f;
	UPROPERTY(EditAnywhere, Category = "Buoyancy")
	float EntryRampTime = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Buoyancy")
	FVector2D BuoyancyPointOffset{ FVector2D::ZeroVector };

	UPROPERTY(EditAnywhere, Category = "Buoyancy")
	AActor* WaveManagerActor;

	UFUNCTION()
	void ApplyFloatForce(const FVector& SampleLocation, float WaveHeight);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UWaveManagerComponent* WaveManager;

	UPrimitiveComponent* OwnerRoot;

	float TimeSinceEnteredWater{ 0.f };
	bool bIsSubmerged = {false};
};
