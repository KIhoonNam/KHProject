
### 1\. Gameplay Ability System (GAS) 기반 캐릭터 설계

이 프로젝트는 확장성과 유지보수성을 위해 캐릭터의 모든 액션과 상태를 GAS로 구현했습니다.

  * **AttributeSet:** 체력(Health), 탄약(Ammo) 등을 관리하며 `PreAttributeChange`와 `PostGameplayEffectExecute`를 통해 데미지 처리 및 클램핑 로직을 수행합니다.
  * **Abilities:**
      * **사격 (Fire):** LineTrace를 이용한 히트스캔 방식. 무기별 반동 및 탄 퍼짐 로직 적용.
      * **재장전 (Reload):** 무기 타입(Rifle/Shotgun)에 따라 다른 로직 수행. 샷건의 경우 `GameplayEvent`를 활용하여 한 발씩 장전하는 로직 구현 (OneBulletReload).
      * **부활 (Revive):** 빈사 상태(Status.Downed)인 아군을 감지하여 채널링(Channeling) 후 상태 이상을 제거.
      * **AI 공격:** 몬스터의 공격 또한 Ability로 구현하여 태그 기반으로 쿨타임 및 피격 판정을 관리.

### 2\. 멀티플레이어 세션 & 로비 시스템

OnlineSubsystem을 활용하여 P2P 세션 관리 시스템을 구축했습니다.

  * **GameInstance:** 세션 생성(Host), 검색(Find), 참가(Join) 및 파괴 로직을 캡슐화.
  * **Lobby System:** KHGameState_Lobby를 통해 플레이어 입장/퇴장 및 준비 상태를 실시간으로 동기화.
  * **Seamless Travel:** 모든 플레이어가 준비되면 로비에서 메인 게임 레벨로 심리스 트래블 지원.

### 3\. 데이터 주도형 (Data-Driven) 설계

하드코딩을 지양하고 기획 데이터 변경에 유연하게 대응하도록 설계했습니다.

  * **DataTable 활용:**
      * `DT_WeaponData`: 무기별 데미지, 쿨다운, 탄창 수, 오토 사격 여부, 스켈레탈 메시 등을 정의.
      * `DT_Wave`: 웨이브별 스폰할 몬스터 타입, 총 마릿수, 스폰 주기 등을 정의하여 레벨 디자인 용이성 확보.
      * `DT_PlayerAnim`: 무기 타입에 따른 애니메이션 몽타주 매핑.

### 4\. AI 시스템

Behavior Tree와 GAS를 연동하여 지능적인 몬스터를 구현했습니다.

  * **AI Perception:** 시각 감각을 통해 플레이어를 탐지.
  * **Behavior Tree Service:** 타겟과의 거리를 실시간으로 계산하여 공격 범위 진입 여부 판단.
  * **GAS 연동:** BTTask에서 직접 공격 로직을 수행하는 것이 아니라, TryActivateAbilitiesByTag를 통해 GAS 어빌리티를 발동시켜 태그 기반의 상태 제어 수행.

### 5\. UI 시스템 (MVVM 패턴 지향)

  * **Delegate 바인딩:** `Tick`에서 UI를 갱신하지 않고, GAS Attribute 값 변경 시 호출되는 델리게이트에 UI를 바인딩하여 성능 최적화.
  * **동적 슬롯:** 로비에 접속한 플레이어 수에 따라 위젯 슬롯을 동적으로 생성 및 갱신.
