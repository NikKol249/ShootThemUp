// Shoot Them Up Game. All Right Reserved

#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/pawn.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Camera/CameraShakeBase.h"
//#include "Dev/STUFireDamageType.h"
//#include "Dev/STUIceDamageType.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

USTUHealthComponent::USTUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	check(MaxHealth > 0);
	
	SetHealth(MaxHealth);
	
	AActor *ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
	}
}

void USTUHealthComponent::OnTakeAnyDamage(AActor *DamagedActor, float Damage, const UDamageType *DamageType,
                                              AController *InstigatedBy, AActor *DamageCauser)
{
    if (Damage <= 0.0f || IsDead() || !GetWorld())
        return;
    Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
    SetHealth(Health - Damage);

    GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

	if (IsDead())
    {
        OnDeath.Broadcast();
	}
    else if (AutoHeal)
    {
        GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHealthComponent::HealUpdate, HealUpdateTime,
                                               true, HealDelay);
    }

    /*
	if (DamageType)
	{
		if (DamageType->IsA<USTUFireDamageType>())
		{
            UE_LOG(LogHealthComponent, Display, TEXT("So HOOOOOOOT !!!"))
        }
		else if (DamageType->IsA<USTUIceDamageType>())
		{
            UE_LOG(LogHealthComponent, Display, TEXT("So COOOOOOLLLLLD !!!"))
		}
	}
	*/

	PlayCameraShake();
}

void USTUHealthComponent::HealUpdate()
{
    SetHealth(Health + HealModifier);

	if (IsHealthFull() && GetWorld())
	{
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	}
}

void USTUHealthComponent::SetHealth(float NewHealth)
{
	const auto NextHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	const auto HealthDelta = NextHealth - Health;
	
    Health = NextHealth;
    OnHealthChanged.Broadcast(Health, HealthDelta);
}

bool USTUHealthComponent::TryToAddHealth(float HealthAmount)
{
	if(IsDead() || IsHealthFull()) return false;

	SetHealth(Health + HealthAmount);
	return true;
}

bool USTUHealthComponent::IsHealthFull() const
{
	return FMath::IsNearlyEqual(Health, MaxHealth);
}

void USTUHealthComponent::PlayCameraShake()
{
	if(IsDead()) return;

	const auto Player = Cast<APawn>(GetOwner());
	if(!Player) return;

	const auto Controller = Player->GetController<APlayerController>();
	if(!Controller) return;

	Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}