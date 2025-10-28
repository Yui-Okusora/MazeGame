#include <Core.hpp>
#include <Application/Application.hpp>

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

    app.run();

    return 0;
}
