// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#define DECLARE_ABSTRACT_METATYPE                                                                                      \
  public:                                                                                                              \
    virtual FName GetClassName() const = 0;                                                                            \
    static FName ClassName();                                                                                          \
                                                                                                                       \
  private:

#define IMPLEMENT_ABSTRACT_METATYPE(Class)                                                                             \
    FName Class::ClassName() {                                                                                         \
        static const FName ClassName = TEXT(#Class);                                                                   \
        return ClassName;                                                                                              \
    }
#define DECLARE_DERIVED_METATYPE                                                                                       \
  public:                                                                                                              \
    FName GetClassName() const override;                                                                               \
    static FName ClassName();                                                                                          \
                                                                                                                       \
  private:

#define IMPLEMENT_DERIVED_METATYPE(Class)                                                                              \
    FName Class::GetClassName() const { return ClassName(); }                                                          \
    FName Class::ClassName() {                                                                                         \
        static const FName ClassName = TEXT(#Class);                                                                   \
        return ClassName;                                                                                              \
    }
