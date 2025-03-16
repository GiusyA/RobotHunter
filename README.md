## Disclaimers
- This project should not be considered for an evaluation of the competences of the game designers I worked with on the original project.
  For instance, the handcar went through multiples versions and this project mingle parts of differents versions. I chose these parts because I found them interesting programming wise but they don't necessarily work well together.
  
- I wasn't in charge of some classes in the original project but still needed them in this project for everything to work. I tried to redo most of these classes myself but you should keep in mind that I wasn't the only one working on them initially when you are looking at them.
  Concerned classes: CustomPlayer, HandcarNitroWidget, InputConfigDA, CustomMacros.<br>
<br>

## Table of contents
- CustomActor  
   - HandcarActor
   - InteractableActor
     - CameraInteractableActor
       - HandcarNitroInteractable
     - DoubleInteractableActor
       - HandcarHandleInteractable
     - RailLeverActor
   - SplineActor
     - RailActor
- CustomActorComponent
  - CustomCameraComponent
  - FSMComponent
    - PlayerFSMComponent
  - InteractionComponent
- HandcarPropertiesDA
- InputConfigDA
- CustomFSM
- CustomState
  - CustomPlayerState
- CustomTransition
  - CustomPlayerTransition
- CustomGameModeBase (empty)
- CustomPlayer
- CustomHUD
- CustomUserWidget
  - HandcarNitroWidget
- CustomEnums
- CustomMacros<br>
<br>

## In the next updates
- HandcarNitro fuel consumption
- HandcarBrake
- HandcarChangeWay
- Handcar auto mode with HandcarStop
- DetectionComponent
- StatusComponent with Status
- Flowers (StormFlower, ColdFlower)
