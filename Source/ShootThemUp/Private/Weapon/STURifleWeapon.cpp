// Shoot Them Up Game. All Right Reserved


#include "Weapon/STURifleWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(RifleWeaponLog, All, All)

void ASTURifleWeapon::StartFire()
{
	UE_LOG(RifleWeaponLog, Display, TEXT("Fire!"));
	
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
	MakeShot();
}

void ASTURifleWeapon::StopFire()
{
	UE_LOG(RifleWeaponLog, Display, TEXT("Fire!"));

	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void ASTURifleWeapon::MakeShot()
{
	UE_LOG(LogTemp, Display, TEXT("Make Shot!"));
	if (!GetWorld() || IsAmmoEmpty())
	{
		StopFire();
		return;
	};

	FVector TraceStart, TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd))
	{
		StopFire();
		return;
	}

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	if (HitResult.bBlockingHit)
	{
		MakeDamage(HitResult);

		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint,
			FColor::Red, false, 3.0f, 0, 3.0f);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red,
			false, 5.0f);

		UE_LOG(RifleWeaponLog, Display, TEXT("Bone: %s"),
			*HitResult.BoneName.ToString());
	}
	else
	{
		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red,
			false, 3.0f, 0, 3.0f);
	}
	DecreaseAmmo();
}

bool ASTURifleWeapon::GetTraceData(FVector& TraceStart,
	FVector&                               TraceEnd) const
{
	FVector  ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
		return false;

	TraceStart = ViewLocation;
	const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
	const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void ASTURifleWeapon::MakeDamage(const FHitResult& HitResult)
{
	const auto DamageActor = HitResult.GetActor();
	if(!DamageActor) return;

	DamageActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
}