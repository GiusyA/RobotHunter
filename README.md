> [!IMPORTANT]
• The game has been released on [Steam](https://store.steampowered.com/app/3358210/StormCore/) under the name *StormCore*<br/><br/>
• This project isn't representative of the skills of the game designers I worked with. For example, the handcar underwent several reworks and I mixed parts of these different versions. I find them interesting from a programming standpoint, but they don't necessarily work well together.<br/><br/>
• Most of the code in this project was done by me, except for some classes on which I worked with other programmers. There is a :busts_in_silhouette: next to their names in the table of contents

## Table of contents
- CustomActor
  - HandcarActor
  - InteractableActor
    - CameraInteractableActor
      - HandcarBrakeInteractable
      - HandcarNitroInteractable
    - DoubleInteractableActor
      - HandcarHandleInteractable
    - RailLeverActor[^1]
  - SplineActor
- CustomActorComponent
  - CustomCameraComponent
  - FSMComponent
    - PlayerFSMComponent
  - InteractionComponent
- HandcarPropertiesDA
- InputConfigDA :busts_in_silhouette:
- CustomFSM
  - CustomState
    - CustomPlayerState
  - CustomTransition
    - CustomPlayerTransition
- CustomGameModeBase[^2]
- CustomPlayer :busts_in_silhouette:
- CustomHUD :busts_in_silhouette:
- CustomUserWidget
  - HandcarNitroFuelWidget
  - HandcarNitroWidget :busts_in_silhouette:
- CustomLerp
- CustomEnum :busts_in_silhouette:
- CustomMacros :busts_in_silhouette:

## Coming soon
- EnemyDetectionComponent
- BonusComponent
- StatusComponent
- HandcarStopInteractable (auto mode when the handcar is called to the stop)
  
[^1]: Should've been called RailLeverInteractable
[^2]: Currently empty
