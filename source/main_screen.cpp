#include "main_screen.hpp"
#include "app.hpp"
#include "requests.hpp"

MainScreen::MainScreen()
{
    auto *titleText = new Aether::Text(TITLE_X, TITLE_Y, "SeedHack", TITLE_SIZE);
    titleText->setColour(Aether::Colour{200, 250, 200, 255});

    addElement(titleText);

    auto *subTitleText = new Aether::Text(TITLE_X + consts::GAP_SIZE + titleText->w(), 0, "Do no evil.", SUB_TITLE_SIZE);
    subTitleText->setY(TITLE_Y + titleText->h() - subTitleText->h() - SUB_TITLE_RAISE);
    subTitleText->setColour(SUB_TITLE_COLOR);
    addElement(subTitleText);

    // Controls
    auto *controls = new Aether::Controls();
    controls->addItem(new Aether::ControlItem(Aether::Button::A, "OK"));
    controls->addItem(new Aether::ControlItem(Aether::Button::X, "Refresh"));
    // onButtonPress(Aether::Button::X, [this]() { this->refreshFileList(); });
    controls->addItem(new Aether::ControlItem(Aether::Button::PLUS, "Exit"));
    onButtonPress(Aether::Button::PLUS, Application::exitApp);
    addElement(controls);
    requests::request();
}

MainScreen::~MainScreen()
{
    // the parent descructor will clean everything up for us as long is the element is added to the screen
}
