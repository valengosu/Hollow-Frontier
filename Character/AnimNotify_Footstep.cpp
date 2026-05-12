#include "AnimNotify_Footstep.h"
#include "Kismet/GameplayStatics.h"

void UAnimNotify_Footstep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                  const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (FootstepSound == nullptr)
		return;
	
	UGameplayStatics::PlaySoundAtLocation(MeshComp->GetWorld(),FootstepSound, MeshComp->GetComponentLocation());
}
