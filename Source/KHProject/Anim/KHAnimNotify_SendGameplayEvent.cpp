// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/KHAnimNotify_SendGameplayEvent.h"

#include "AbilitySystemBlueprintLibrary.h"


void UKHAnimNotify_SendGameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                             const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
	{
		return;
	}

	// 1. [핵심] 이 스켈레탈 메시 컴포넌트를 소유한 액터(캐릭터)를 가져옵니다.
	AActor* OwnerActor = MeshComp->GetOwner();
	if (OwnerActor == nullptr)
	{
		return;
	}

	// 2. [핵심] 소유자 액터에게 C++로 설정한 'EventTag'를 전송합니다.
	// (FGameplayEventData는 필요시 추가 정보를 담아 보낼 수 있습니다)
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		OwnerActor,       // 이벤트를 받을 액터
		EventTag,         // [중요] UPROPERTY로 선언한 바로 그 태그
		FGameplayEventData() // (선택) 페이로드 데이터
	);
}
