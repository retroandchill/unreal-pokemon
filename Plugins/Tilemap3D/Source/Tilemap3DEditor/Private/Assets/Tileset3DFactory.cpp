// "Unreal Pokémon" created by Retro & Chill.

#include "Assets/Tileset3DFactory.h"
#include "Tileset/Tileset3D.h"

UTileset3DFactory::UTileset3DFactory() {
    SupportedClass = UTileset3D::StaticClass();
    bCreateNew = true;
}

UObject *UTileset3DFactory::FactoryCreateNew(UClass *InClass, UObject *InParent, FName InName, EObjectFlags Flags,
                                             UObject *Context, FFeedbackContext *Warn) {
    return NewObject<UTileset3D>(InParent, InClass, InName, Flags, Context);
}