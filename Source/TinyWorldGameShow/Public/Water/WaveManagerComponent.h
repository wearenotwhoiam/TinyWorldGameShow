// RnDumb Fun - GameDev.TV Game Jam 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WaveConfig.h"
#include "WaveManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TINYWORLDGAMESHOW_API UWaveManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWaveManagerComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Gets the combined wave height at a 2D world position
	UFUNCTION(BlueprintCallable, Category = "Wave")
	float GetWaveHeightAt(FVector2D WorldPosition, float Time) const;

	// Expose waves to the editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Settings")
	TArray<FWaveConfig> Waves;

	// Reference to your material parameter collection
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Settings")
	class UMaterialParameterCollection* WaveParameterCollection;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void UpdateMaterialParameters(float Time);
};
