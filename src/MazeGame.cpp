#include <Core.hpp>
#include <Application/Application.hpp>
#include <Utils/Utils.hpp>
#include <GameMechanics/GameplayState.hpp>
#include <GameMechanics/GameMenu.hpp>
#include <GameMechanics/MainMenu.hpp>

int main() {
    GameplayData data;

    ApplicationSpecs specs = {
        .appTitle = "Test",
        .windowsSpecs = {
            .width = 1000,
            .height = 600,
            .resizable = true,
            .fullscreen = true,
            .vSync = true,
            .fps = 60
        }
    };

    specs.sharedCTX = &data;

    Application app(specs);

    //app.getStateStack().resizeStack(2);

    app.pushState<MainMenu>();

    app.pushInactive<PauseMenu>();
    app.pushInactive<GameplayState>();

    app.run();

    return 0;
}
