// Shoot Them Up Game. All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "STUProjectile.h"
#include "Weapon/STUBaseWeapon.h"
#include "STUShotgunWeapon.generated.h"

class ASTUProjectile;

UCLASS()
class SHOOTTHEMUP_API ASTUShotgunWeapon : public ASTUBaseWeapon
{
	GENERATED_BODY()

public:
	virtual void StartFire() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ASTUProjectile>ProjectileClass;
	
	virtual void MakeShot() override;
	
};
