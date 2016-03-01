// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSRTSPrototype.h"
#include "AllyMember.h"
//#include "RTSControllerCPP.h"
#include "ShooterPickup_Ammo.h"

AShooterPickup_Ammo::AShooterPickup_Ammo(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AmmoClips = 2;
	DefClipSize = 20;
}

/*bool AShooterPickup_Ammo::IsForWeapon(UClass* WeaponClass)
{
	return WeaponType->IsChildOf(WeaponClass);
}*/

bool AShooterPickup_Ammo::CanBePickedUp(AAllyMember* TestPawn) const
{
	//AShooterWeapon* TestWeapon = (TestPawn ? TestPawn->FindWeapon(WeaponType) : NULL);
	//if (bIsActive && TestWeapon)
	//{
	//	return TestWeapon->GetCurrentAmmo() < TestWeapon->GetMaxAmmo();
	//}
	return TestPawn->getCurrentAmmo() <= TestPawn->GetMaxAmmo();
	//return false;
}

void AShooterPickup_Ammo::GivePickupTo(class AAllyMember* Pawn)
{
	//AShooterWeapon* Weapon = (Pawn ? Pawn->FindWeapon(WeaponType) : NULL);
	//if (Weapon)
	//{
		//int32 Qty = AmmoClips * Weapon->GetAmmoPerClip();
		//Weapon->GiveAmmo(Qty);

		// Fire event for collected ammo
		if (Pawn)
		{
			GEngine->AddOnScreenDebugMessage(2, 1, FColor::Green, "I picked up ammo!");
			int32 giveAmmo = DefClipSize * AmmoClips;
			Pawn->addAllyCurrentAmmo(giveAmmo);
			//const auto Events = Online::GetEventsInterface();
		//	const auto Identity = Online::GetIdentityInterface();

			//if (Events.IsValid() && Identity.IsValid())
			//{
			//	ARTSControllerCPP* PC = Cast<ARTSControllerCPP>(Pawn->Controller);
			//	if (PC)
				//{
					//ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PC->Player);

					//if (LocalPlayer)
					//{
						//const int32 UserIndex = LocalPlayer->GetControllerId();
						//TSharedPtr<const FUniqueNetId> UniqueID = Identity->GetUniquePlayerId(UserIndex);
						//if (UniqueID.IsValid())
						//{
						//	FVector Location = Pawn->GetActorLocation();

							//FOnlineEventParms Params;

							//Params.Add(TEXT("SectionId"), FVariantData((int32)0)); // unused
						//	Params.Add(TEXT("GameplayModeId"), FVariantData((int32)1)); // @todo determine game mode (ffa v tdm)
						//	Params.Add(TEXT("DifficultyLevelId"), FVariantData((int32)0)); // unused

						//	Params.Add(TEXT("ItemId"), FVariantData((int32)Weapon->GetAmmoType() + 1)); // @todo come up with a better way to determine item id, currently health is 0 and ammo counts from 1
						//	Params.Add(TEXT("AcquisitionMethodId"), FVariantData((int32)0)); // unused
						//	Params.Add(TEXT("LocationX"), FVariantData(Location.X));
						//	Params.Add(TEXT("LocationY"), FVariantData(Location.Y));
						//	Params.Add(TEXT("LocationZ"), FVariantData(Location.Z));
							//Params.Add(TEXT("ItemQty"), FVariantData((int32)Qty));

							//Events->TriggerEvent(*UniqueID, TEXT("CollectPowerup"), Params);
						//}
					//}
				//}
			//}
		}
	//}
}



