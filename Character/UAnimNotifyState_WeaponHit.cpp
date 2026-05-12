#include "UAnimNotifyState_WeaponHit.h"

#include "FoliageInstancedStaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "SurvivalCharacter.h"
#include "Components/AudioComponent.h"
#include "Engine/StaticMeshActor.h"
#include "TreeSystem/TreeSubsystem.h"

void UUAnimNotifyState_WeaponHit::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	
	ASurvivalCharacter* Char = Cast<ASurvivalCharacter>(MeshComp->GetOwner());
	if (Char == nullptr)
		return;
	
	if (Char->HoldItem == nullptr)
		return;
		
	UStaticMeshComponent* Mesh = Char->HoldItem->GetStaticMeshComponent();
	if (Mesh->DoesSocketExist("AxeSocket") == false)
		return;
	
	FVector Pos = Mesh->GetSocketLocation("AxeSocket");
	Char->PrevSocketLocation.Emplace(Mesh, Pos);
	Char->CurrentAttackHitSet.Empty();
}

void UUAnimNotifyState_WeaponHit::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	ASurvivalCharacter* Char = Cast<ASurvivalCharacter>(MeshComp->GetOwner());
	if (Char == nullptr)
		return;
	
	if (Char->HoldItem == nullptr)
		return;
	
	UStaticMeshComponent* Mesh = Char->HoldItem->GetStaticMeshComponent();
	if (Mesh->DoesSocketExist("AxeSocket") == false)
		return;
	
	FVector CurrentPos = Mesh->GetSocketLocation("AxeSocket");	
	FVector PrevPos = *Char->PrevSocketLocation.Find(Mesh);
	
	FHitResult Hit;
	//FCollisionShape Shape = FCollisionShape::MakeSphere(5.f);
	//bool bHit = GetWorld()->SweepSingleByChannel(Hit,PrevPos,CurrentPos,FQuat::Identity,ECC_Visibility, Shape);
	bool bHit = MeshComp->GetWorld()->LineTraceSingleByChannel(Hit,PrevPos,CurrentPos, ECC_Visibility);
	
	Char->PrevSocketLocation[Mesh] = CurrentPos;
	
	if (bHit == false)
		return;
	
	AActor* HitActor = Hit.GetActor();
	
	UFoliageInstancedStaticMeshComponent* Foliage = Cast<UFoliageInstancedStaticMeshComponent>(Hit.GetComponent());
	if (Foliage != nullptr)
	{
		int32 InstanceIndex = Hit.Item;
		
		FTransform Transform;
		Foliage->GetInstanceTransform(InstanceIndex, Transform, true);
		
		UTreeSubsystem* TreeSys = MeshComp->GetOwner()->GetGameInstance()->GetSubsystem<UTreeSubsystem>();
		HitActor = TreeSys->CreateTreeActor(Transform, Foliage->GetStaticMesh(), Foliage);
		Foliage->RemoveInstance(InstanceIndex);	
	}
	
	if (Char->CurrentAttackHitSet.Contains(HitActor) == true)
		return;
	
	if (IInteractable* Chop = Cast<IInteractable>(HitActor))
	{
		Char->CurrentAttackHitSet.Add(HitActor);
		
		Chop->OnChop(MeshComp->GetOwner());
		
		if (UNiagaraComponent* Niagara = Char->HoldItem->FindComponentByClass<UNiagaraComponent>())
		{
			Niagara->SetWorldLocation(Hit.ImpactPoint);
			Niagara->Activate(true);	
		}
		
		if (UAudioComponent* Audio = Char->HoldItem->FindComponentByClass<UAudioComponent>())
		{
			Audio->Stop();
			Audio->Play(0.05);
		}
	}
}

void UUAnimNotifyState_WeaponHit::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	
	ASurvivalCharacter* Char = Cast<ASurvivalCharacter>(MeshComp->GetOwner());
	if (Char == nullptr)
		return;
	
	if (Char->HoldItem == nullptr)
		return;
		
	UStaticMeshComponent* Mesh = Char->HoldItem->GetStaticMeshComponent();
	if (Mesh->DoesSocketExist("AxeSocket") == false)
		return;
	
	Char->PrevSocketLocation.Remove(Mesh);
	Char->CurrentAttackHitSet.Empty();
}
