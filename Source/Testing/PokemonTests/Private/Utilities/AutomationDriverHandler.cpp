// "Unreal Pokémon" created by Retro & Chill.


#include "Utilities/AutomationDriverHandler.h"

#include "IAutomationDriverModule.h"


FAutomationDriverHandler::FAutomationDriverHandler() {
	IAutomationDriverModule::Get().Enable();
}

FAutomationDriverHandler::~FAutomationDriverHandler() {
	IAutomationDriverModule::Get().Disable();
}

FAutomationDriverPtr FAutomationDriverHandler::CreateDriver() const {
	return IAutomationDriverModule::Get().CreateDriver();
}
