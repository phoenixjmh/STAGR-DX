#include "Application.h"

int main()
{
    Application app;

    while (!app.GetWindow()->ShouldClose())
    {
        app.Run();
    }
    app.Close();
}