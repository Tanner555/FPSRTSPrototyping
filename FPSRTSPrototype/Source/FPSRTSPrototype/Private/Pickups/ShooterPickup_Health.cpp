// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSRTSPrototype.h"
#include "AllyMember.h"
#include "ShooterPickup_Health.h"


AShooterPickup_Health::AShooterPickup_Health(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Health = 50;
}

bool AShooterPickup_Health::CanBePickedUp(class AAllyMember* TestPawn) const
{
	return TestPawn && (TestPawn->getAllyHealth() < TestPawn->getAllyMaxHealth());
}

void AShooterPickup_Health::GivePickupTo(class AAllyMember* Pawn)
{
	if (Pawn)
	{
		GEngine->AddOnScreenDebugMessage(2, 1, FColor::Green, "I picked up health!");
		Pawn->addAllyHealth(Health);
		//Pawn->setAllyHealth(FMath::Min(FMath::TruncToInt(Pawn->getAllyHealth() + Health), FMath::TruncToInt(Pawn->getAllyMaxHealth())));
		//Pawn->Health = FMath::Min(FMath::TruncToInt(Pawn->Health) + Health, Pawn->GetMaxHealth());

		// Fire event for collected health
		//const auto Events = Online::GetEventsInterface();
		//const auto Identity = Online::GetIdentityInterface();

	/*	if (Events.IsValid() && Identity.IsValid())
		{
			AShooterPlayerController* PC = Cast<AShooterPlayerController>(Pawn->Controller);
			if (PC)
			{
				ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PC->Player);

				if (LocalPlayer)
				{
					const int32 UserIndex = LocalPlayer->GetControllerId();
					TSharedPtr<const FUniqueNetId> UniqueID = Identity->GetUniquePlayerId(UserIndex);
					if (UniqueID.IsValid())
					{
						FVector Location = Pawn->GetActorLocation();

						FOnlineEventParms Params;

						Params.Add(TEXT("SectionId"), FVariantData((int32)0)); // unused
						Params.Add(TEXT("GameplayModeId"), FVariantData((int32)1)); // @todo determine game mode (ffa v tdm)
						Params.Add(TEXT("DifficultyLevelId"), FVariantData((int32)0)); // unused

						Params.Add(TEXT("ItemId"), FVariantData((int32)0)); // @todo come up with a better way to determine item id, currently health is 0 and ammo counts from 1
						Params.Add(TEXT("AcquisitionMethodId"), FVariantData((int32)0)); // unused
						Params.Add(TEXT("LocationX"), FVariantData(Location.X));
						Params.Add(TEXT("LocationY"), FVariantData(Location.Y));
						Params.Add(TEXT("LocationZ"), FVariantData(Location.Z));
						Params.Add(TEXT("ItemQty"), FVariantData((int32)Health));

						Events->TriggerEvent(*UniqueID, TEXT("CollectPowerup"), Params);
					}
				}
			}
		}*/
	}
}

