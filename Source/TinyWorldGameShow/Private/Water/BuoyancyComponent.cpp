// RnDumb Fun - GameDev.TV Game Jam 2025


#include "Water/BuoyancyComponent.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "Water/WaveManagerComponent.h"

// Sets default values for this component's properties
UBuoyancyComponent::UBuoyancyComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UBuoyancyComponent::BeginPlay()
{
	Super::BeginPlay();

	if (WaveManagerActor)
	{
		WaveManager = WaveManagerActor->FindComponentByClass<UWaveManagerComponent>();
	}
	OwnerRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	if (OwnerRoot)
	{
		OwnerRoot->SetSimulatePhysics(true);
	}
}


// Called every frame
void UBuoyancyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector ActorLoc = GetOwner()->GetActorLocation();
	FVector2D SamplePoint = FVector2D(ActorLoc.X + BuoyancyPointOffset.X, ActorLoc.Y + BuoyancyPointOffset.Y);

	float Time = GetWorld()->GetTimeSeconds();
	float WaveHeight = WaveManager->GetWaveHeightAt(SamplePoint, Time);

	const float SubmersionDepth = WaveHeight - ActorLoc.Z;

	if (SubmersionDepth > 0)
	{
		if (!bIsSubmerged)
		{
			bIsSubmerged = true;
			TimeSinceEnteredWater = 0.f;
		}
		else
		{
			TimeSinceEnteredWater += DeltaTime;
		}
	}
	else
	{
		bIsSubmerged = false;
		TimeSinceEnteredWater = 0.f;
	}

	// Apply float force at this point
	ApplyFloatForce(ActorLoc, WaveHeight);
}

void UBuoyancyComponent::ApplyFloatForce(const FVector& SampleLocation, float WaveHeight)
{
	if (!OwnerRoot) return;

	const float ActorZ = SampleLocation.Z;
	const float SubmersionDepth = WaveHeight - ActorZ;

	if (SubmersionDepth <= 0.f) return;
	FVector Velocity = OwnerRoot->GetComponentVelocity();
	float VerticalVelocity = FVector::DotProduct(Velocity, FVector::UpVector);

	float SpringForce = SubmersionDepth * BuoyancyStrength;
	float DampingForce = -VerticalVelocity * DampingStrength;

	float BuoyancyAlpha = FMath::Clamp(TimeSinceEnteredWater / EntryRampTime, 0.f, 1.f);
	SpringForce *= BuoyancyAlpha;
	float TotalForce = FMath::Clamp(SpringForce + DampingForce, -MaxBuoyancyForce, MaxBuoyancyForce);
	FVector FinalForce = FVector::UpVector * TotalForce;

	OwnerRoot->AddForce(FinalForce);

	//#if WITH_EDITOR
	//	DrawDebugLine(
	//		GetWorld(),
	//		SampleLocation,
	//		SampleLocation + FinalForce * 0.001f,
	//		FColor::Cyan,
	//		false, 0.1f, 0, 1.f
	//	);
	//#endif
}
