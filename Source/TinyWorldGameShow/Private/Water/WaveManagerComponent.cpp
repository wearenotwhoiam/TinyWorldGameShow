// RnDumb Fun - GameDev.TV Game Jam 2025


#include "Water/WaveManagerComponent.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Materials/MaterialParameterCollection.h"
#include "Kismet/KismetMaterialLibrary.h"

#include "ContestantDebugHelper.h"

// Sets default values for this component's properties
UWaveManagerComponent::UWaveManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UWaveManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}
// Called every frame
void UWaveManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float CurrentTime = GetWorld()->GetTimeSeconds();
	UpdateMaterialParameters(CurrentTime);
}

float UWaveManagerComponent::GetWaveHeightAt(FVector2D WorldPosition, float Time) const
{
    if (Waves.Num() == 0)
    {
        Debug::Print(TEXT("No waves defined! Returning 0.f"));
        return 0.f;
    }
    float TotalHeight = 0.f;

    for (const FWaveConfig& Wave : Waves)
    {
        FVector2D Dir = Wave.Direction.GetSafeNormal();
        float Dot = FVector2D::DotProduct(Dir, WorldPosition);
        float Phase = Dot * Wave.Frequency + Wave.Speed * Time;
        float Height = Wave.Amplitude * FMath::Sin(Phase);
        TotalHeight += Height;
    }

    return TotalHeight;
}

void UWaveManagerComponent::UpdateMaterialParameters(float Time)
{
    if (!WaveParameterCollection) return;

    UMaterialParameterCollectionInstance* MPC = GetWorld()->GetParameterCollectionInstance(WaveParameterCollection);
    if (!MPC) return;

    
    MPC->SetScalarParameterValue(FName("WaveTime"), Time);

    const int MaxWaves = 4;
    for (int32 i = 0; i < MaxWaves; ++i)
    {
        FString Index = FString::FromInt(i);

        FName AmpParam = FName("WaveAmplitude" + Index);
        FName FreqParam = FName("WaveFrequency" + Index);
        FName SpeedParam = FName("WaveSpeed" + Index);
        FName DirParam = FName("WaveDirection" + Index);

        if (Waves.IsValidIndex(i))
        {
            const FWaveConfig& Wave = Waves[i];
            MPC->SetScalarParameterValue(AmpParam, Wave.Amplitude);
            MPC->SetScalarParameterValue(FreqParam, Wave.Frequency);
            MPC->SetScalarParameterValue(SpeedParam, Wave.Speed);
            FVector Dir3D = FVector(Wave.Direction.X, Wave.Direction.Y, 0.f);
            MPC->SetVectorParameterValue(DirParam, FLinearColor(Dir3D));
        }
        else
        {
            MPC->SetScalarParameterValue(AmpParam, 0.f);
            MPC->SetScalarParameterValue(FreqParam, 0.f);
            MPC->SetScalarParameterValue(SpeedParam, 0.f);
            MPC->SetVectorParameterValue(DirParam, FLinearColor::Black);
        }
    }
}


