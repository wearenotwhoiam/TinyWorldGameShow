#pragma once

#include "CoreMinimal.h"
#include "WaveConfig.generated.h"

USTRUCT(BlueprintType)
struct FWaveConfig
{
    GENERATED_BODY()

    // Controls the vertical height of the wave.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    float Amplitude;

    // Determines the frequency of the oscillation.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    float Frequency;

    // Controls how fast the wave moves.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    float Speed;

    // The wave's travel direction in the XY plane.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    FVector2D Direction = FVector2D(0.f, 0.f);

};
