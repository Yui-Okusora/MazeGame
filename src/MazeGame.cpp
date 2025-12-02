#include <Core.hpp>
#include <Application/Application.hpp>
#include <Utils/Utils.hpp>
#include <GameMechanics/GameplayState.hpp>

class PauseMenu : public State
{
public:
    PauseMenu(IApplication* _app)
    {
        app = _app;
        label = "PauseMenu";
    }

    void onEnter()
    {

    }

    void handleInput(const KeyInputState& in) override
    {
        if (in.keyPressed[GLFW_KEY_P])
        {
            app->getStateStack().queueTransit(this, "GameplayState");
        }
    }

    void update()
    {

    }

    void render() override
    {
        gl2d::Renderer2D& renderer = app->getRenderer();

        renderer.clearScreen({ 1, 1, 1, 1 });
    }
};

int main() {
    ApplicationSpecs specs = {
        .appTitle = "Test",
        .windowsSpecs = {
            .width = 800,
            .height = 600,
            .resizable = true,
            .vSync = true
        }
    };

    Application app(specs);

    //app.getStateStack().resizeStack(2);

    app.pushState<GameplayState>();

    app.getStateStack().addInactive(std::make_unique<PauseMenu>(&app));

    app.run();

    return 0;
}
