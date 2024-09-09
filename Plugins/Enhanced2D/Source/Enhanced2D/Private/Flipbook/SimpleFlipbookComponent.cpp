// "Unreal Pokémon" created by Retro & Chill.


#include "Flipbook/SimpleFlipbookComponent.h"


void USimpleFlipbookComponent::Serialize(FArchive &Ar) {
    Super::Serialize(Ar);
}

void USimpleFlipbookComponent::PostLoad() {
    Super::PostLoad();
}

void USimpleFlipbookComponent::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction *ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USimpleFlipbookComponent::SendRenderDynamicData_Concurrent() {
    Super::SendRenderDynamicData_Concurrent();
}

const UObject * USimpleFlipbookComponent::AdditionalStatObject() const {
    return Super::AdditionalStatObject();
}

void USimpleFlipbookComponent::CheckForErrors() {
    Super::CheckForErrors();
}

bool USimpleFlipbookComponent::HasAnySockets() const {
    return Super::HasAnySockets();
}

bool USimpleFlipbookComponent::DoesSocketExist(FName InSocketName) const {
    return Super::DoesSocketExist(InSocketName);
}

FTransform USimpleFlipbookComponent::GetSocketTransform(FName InSocketName,
    ERelativeTransformSpace TransformSpace) const {
    return Super::GetSocketTransform(InSocketName, TransformSpace);
}

void USimpleFlipbookComponent::QuerySupportedSockets(TArray<FComponentSocketDescription> &OutSockets) const {
    Super::QuerySupportedSockets(OutSockets);
}

FPrimitiveSceneProxy * USimpleFlipbookComponent::CreateSceneProxy() {
    return Super::CreateSceneProxy();
}

FBoxSphereBounds USimpleFlipbookComponent::CalcBounds(const FTransform &LocalToWorld) const {
    return Super::CalcBounds(LocalToWorld);
}

void USimpleFlipbookComponent::GetUsedTextures(TArray<UTexture *> &OutTextures,
    EMaterialQualityLevel::Type QualityLevel) {
    Super::GetUsedTextures(OutTextures, QualityLevel);
}

UMaterialInterface * USimpleFlipbookComponent::GetMaterial(int32 MaterialIndex) const {
    return Super::GetMaterial(MaterialIndex);
}

void USimpleFlipbookComponent::GetUsedMaterials(TArray<UMaterialInterface *> &OutMaterials,
    bool bGetDebugMaterials) const {
    Super::GetUsedMaterials(OutMaterials, bGetDebugMaterials);
}

void USimpleFlipbookComponent::GetStreamingRenderAssetInfo(FStreamingTextureLevelContext &LevelContext,
    TArray<FStreamingRenderAssetPrimitiveInfo> &OutStreamingTextures) const {
    Super::GetStreamingRenderAssetInfo(LevelContext, OutStreamingTextures);
}

int32 USimpleFlipbookComponent::GetNumMaterials() const {
    return Super::GetNumMaterials();
}

UBodySetup * USimpleFlipbookComponent::GetBodySetup() {
    return Super::GetBodySetup();
}

bool USimpleFlipbookComponent::SetFlipbook(UPaperFlipbook *NewFlipbook) {
}

UPaperFlipbook * USimpleFlipbookComponent::GetFlipbook() {
}

void USimpleFlipbookComponent::SetSpriteColor(FLinearColor NewColor) {
}

void USimpleFlipbookComponent::Play() {
}

void USimpleFlipbookComponent::PlayFromStart() {
}

void USimpleFlipbookComponent::Reverse() {
}

void USimpleFlipbookComponent::ReverseFromEnd() {
}

void USimpleFlipbookComponent::Stop() {
}

bool USimpleFlipbookComponent::IsPlaying() const {
}

bool USimpleFlipbookComponent::IsReversing() const {
}

void USimpleFlipbookComponent::SetPlaybackPositionInFrames(int32 NewFramePosition, bool bFireEvents) {
}

int32 USimpleFlipbookComponent::GetPlaybackPositionInFrames() const {
}

void USimpleFlipbookComponent::SetPlaybackPosition(float NewPosition, bool bFireEvents) {
}

float USimpleFlipbookComponent::GetPlaybackPosition() const {
}

void USimpleFlipbookComponent::SetLooping(bool bNewLooping) {
}

bool USimpleFlipbookComponent::IsLooping() const {
}

void USimpleFlipbookComponent::SetPlayRate(float NewRate) {
}

float USimpleFlipbookComponent::GetPlayRate() const {
}

void USimpleFlipbookComponent::SetNewTime(float NewTime) {
}

float USimpleFlipbookComponent::GetFlipbookLength() const {
}

int32 USimpleFlipbookComponent::GetFlipbookLengthInFrames() const {
}

float USimpleFlipbookComponent::GetFlipbookFramerate() const {
}

void USimpleFlipbookComponent::OnRep_SourceFlipbook(UPaperFlipbook *OldFlipbook) {
}

void USimpleFlipbookComponent::CalculateCurrentFrame() {
}

UPaperSprite * USimpleFlipbookComponent::GetSpriteAtCachedIndex() const {
}

void USimpleFlipbookComponent::TickFlipbook(float DeltaTime) {
}

void USimpleFlipbookComponent::FlipbookChangedPhysicsState() {
}