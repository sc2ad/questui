#include "beatsaber-hook/shared/utils/utils.h"
#include "custom-types/shared/register.hpp"

#include "QuestUI.hpp"
#include "ModSettingsInfos.hpp"
#include "CustomTypes/CustomUIKeyboard.hpp"
#include "CustomTypes/ViewControllers/KeyboardViewController.hpp"
#include "CustomTypes/FlowCoordinators/ModSettingsFlowCoordinator.hpp"

#include "BeatSaberUI.hpp"

#include "GlobalNamespace/MainMenuViewController.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Events/UnityAction.hpp"

#include "customlogger.hpp"

using namespace QuestUI;

const Logger& getLogger() {
    static const Logger logger(ModInfo{"questui", VERSION}, LoggerOptions(false, false));
    return logger;
}

void OnModSettingsButtonClick(UnityEngine::UI::Button* button) {
    getLogger().info("ModSettingsButtonClick");
    static ModSettingsFlowCoordinator* flowCoordinator = nullptr;
    if(!flowCoordinator){
        flowCoordinator = BeatSaberUI::CreateFlowCoordinator<ModSettingsFlowCoordinator*>();
    }
    BeatSaberUI::getMainFlowCoordinator()->PresentFlowCoordinator(flowCoordinator, nullptr, false, false);
}

MAKE_HOOK_OFFSETLESS(MainMenuViewController_DidActivate, void, GlobalNamespace::MainMenuViewController* self, bool firstActivation, HMUI::ViewController::ActivationType activationType) {
    MainMenuViewController_DidActivate(self, firstActivation, activationType);
    if(firstActivation) {
        if(ModSettingsInfos::get().size() > 0) {
            UnityEngine::UI::Button* settingsButton = (UnityEngine::UI::Button*)RET_V_UNLESS(il2cpp_utils::GetFieldValue(self, "_settingsButton"));
            UnityEngine::UI::Button* button = UnityEngine::Object::Instantiate(settingsButton);
            button->set_name(il2cpp_utils::createcsstr("Mod Settings"));
            button->get_transform()->SetParent(settingsButton->get_transform()->GetParent(), false);
            button->get_transform()->SetAsFirstSibling();
            button->get_onClick()->AddListener(il2cpp_utils::MakeAction<UnityEngine::Events::UnityAction>(il2cpp_functions::class_get_type(classof(UnityEngine::Events::UnityAction*)), (Il2CppObject*)nullptr, OnModSettingsButtonClick));
            BeatSaberUI::SetButtonText(button, "Mod Settings");
        }
    }
}

void QuestUI::Init() {
    static bool init = false;
    if(!init) {
        init = true;
        il2cpp_functions::Init();
        custom_types::Register::RegisterType<CustomUIKeyboard>();
        custom_types::Register::RegisterType<KeyboardViewController>();
        custom_types::Register::RegisterType<ModSettingsButtonsViewController>();
        custom_types::Register::RegisterType<ModSettingsFlowCoordinator>();
        INSTALL_HOOK_OFFSETLESS(MainMenuViewController_DidActivate, il2cpp_utils::FindMethodUnsafe("", "MainMenuViewController", "DidActivate", 2));
    }
}

void Register::RegisterModSettings(ModInfo modInfo, Il2CppReflectionType* viewController) {
    ModSettingsInfos::add(ModSettingsInfos::ModSettingsInfo{ modInfo, viewController, nullptr });
}