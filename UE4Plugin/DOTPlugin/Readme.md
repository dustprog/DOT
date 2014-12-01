Plugin is made for Unreal Engine 4.6

Installation: Copy the DOTPlugin folder into the Plugin folder of either the project or the engine.

General notes:

- Advertisement (AAdvertisement) derives from Actor
- Entitiy (AEntity) derives from Character
- World (ADOTWorld) derives from GameMode (Note: AWorld is being used by the engine and cannot be used)
- Since classes are used that derive from AActor and ACharacter the "new" keyword cannot be used. To create a new instance
  the appropriate SpawnActor() function is used
- Copying an instance also requires the use SpawnActor
- The engine doesn't support SharedPointers on UObjects and classes derived from because of the garbage collections system.
  SharedPointers are being replaced by hard references. Possible to use TWeakObjectPtr if needed.
- all instances of std::vector were replaced with the appropriate UE counterpart - the TArray, for better readibility most of 
  the for-loops are rewritten to use the ranged-based loop syntax
- all instances of std::string were replaced with FString
- instances of int/short were replaced with the appropriate types int32/int16. int should be avoided as the size of it depends
  on the used compiler
- VECTOR2DF was replaced with FVector2D

Class specific notes:

AAdvertisement:
- UClasses do not support multiple inheritance except for Interfaces. Therefor AAdvertisementInterface has to follow the 
  engine interface guidelines. This comes with certain restrictions, e.g. the UPROPERTY macro is not supported for interfaces
- Ambigous variable "Name" was replaced by "Name"
- Ambigous function "GetOwner" was replaced by "GetOwnership"