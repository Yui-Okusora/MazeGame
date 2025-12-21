#include <Core.hpp>
#include <Application/Application.hpp>
#include <Utils/Utils.hpp>
#include <GameMechanics/GameplayState.hpp>
#include <GameMechanics/GameMenu.hpp>
#include <GameMechanics/MainMenu.hpp>
#include <GameMechanics/SettingsMenu.hpp>
#include <GameMechanics/PauseMenu.hpp>
#include <GameMechanics/GameWL.hpp>
#include <GameMechanics/SaveLoad.hpp>

int main() {
    GameplayData data;

    ApplicationSpecs specs = {
        .appTitle = "Deadline",
        .windowsSpecs = {
            .width = 1200,
            .height = 805,
            .resizable = true,
            .fullscreen = false,
            .vSync = true,
            .fps = 60
        }
    };

    specs.sharedCTX = &data;

    Application app(specs);

    app.getStateStack().resizeStack(2);

    app.pushState<MainMenu>();

    app.pushInactive<PauseMenu>();
    app.pushInactive<GameMenu>();
    app.pushInactive<GameplayState>();
    app.pushInactive<SettingsMenu>();
    app.pushInactive<GameWL>();
    app.pushInactive<SaveLoad>();

    app.run();

    return 0;
}
