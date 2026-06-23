// #include "./sdlUtils/userInterface/elements/button.h"
// #include "./sdlUtils/userInterface/elements/slider.h"
// #include "./sdlUtils/userInterface/elements/textDisplay.h"
// #include "./sdlUtils/userInterface/elements/liveTextDisplay.h"
// #include "./sdlUtils/userInterface/elements/elementGap.h"

// #include <SDL3/SDL.h>
// #include <string>
// #include <format>
// #include <SDL3_image/SDL_image.h>
// #include "./framework/framework.h"
// #include "./sdlUtils/userInterface/mouse.h"
// #include <iostream>
// #include "./sdlUtils/color.h"
// #include "./sdlUtils/userInterface/buttonMenu.h"
// #include "./sdlUtils/userInterface/generalMenu.h"
// #include "./sdlUtils/userInterface/scrollView/scrollView.h"
// #include "./sdlUtils/userInterface/controlMenu/controlInput.h"
// #include "./sdlUtils/userInterface/controlMenu/controlMenu.h"
// #include "./controller/playerController.h"



// int main() {

//     SDL_Texture* bgImg {IMG_LoadTexture(Framework::display.renderer(), std::string{SDL_GetBasePath() + std::string{"audio_menu_bg.png"}}.data())};

//     Mouse mouse{
//         static_cast<float>(Framework::display.screenWidth()),
//         static_cast<float>(Framework::display.screenHeight()),
//         static_cast<float>(Framework::display.canvasWidth()),
//         static_cast<float>(Framework::display.canvasHeight()),
//         Framework::display.window(),
//         Framework::images.ballImg,
//         Framework::images.ballImg->w / 2.0f,
//         Framework::images.ballImg->h / 2.0f,
//     };
//     mouse.clampMouseToCanvas(true);

//     Button button{400, 400, Framework::images.playBtnImg, Framework::images.playBtnHvrImg};

//     // std::function<void(float)> sliderCb{
//     //     [](float val) -> void {
//     //         std::cout << val << "\n";
//     //     }
//     // };

//     // Slider slider{100, 100, sliderCb, imgBar, imgSlide};
//     // slider.setValue(1);


//     TextDisplay hardText{800, 400, "testing", &Framework::fonts.scoreFont, Color{255, 0, 0, 255}};


//     // std::function<std::string()> liveTextCb{
//     //     [&slider]() -> std::string {
//     //         float val {slider.value()};
//     //         return std::format("{:.2f}", val);
//     //     }
//     // };
//     Slider* musicSlider{new Slider{
//         0, 0, 
//         [](float) -> void {
//         },
//         SDL_Color{255, 255, 255, 255},
//         SDL_Color{0, 0, 0, 255},
//         250, 40
//     }};


//     // auto initCb {
//     //     [musicSlider]() -> void {
//     //         musicSlider->setValue(1);
//     //     }
//     // };


//     auto musicTextCb {
//         [musicSlider]() ->std::string {
//             float val {musicSlider->value()};
//             return std::format("{:.2f}", val);
//         }
//     };


//     // LiveTextDisplay liveText{500, 100, &Framework::fonts.scoreFont, Color{0, 255, 0, 255}, liveTextCb};
//     // std::vector<IMenuElement*> elements{
//     //     new TextDisplay{0, 0, "Audio", &Framework::fonts.scoreFont, Color{255, 255, 255, 255}},
//     //     new ElementGap{0},
//     //     new TextDisplay{0, 0, "Music", &Framework::fonts.scoreFont, Color{255, 255, 255, 255}},
//     //     new LiveTextDisplay{0, 0, &Framework::fonts.scoreFont, Color{255, 255, 255, 255}, musicTextCb},
//     //     musicSlider,
//     //     new ElementGap{0},
//     //     new Button{0, 0, Framework::images.exitBtnImg, Framework::images.exitBtnHvrImg},
//     //     new TextDisplay{0, 0, "Audio", &Framework::fonts.scoreFont, Color{255, 255, 255, 255}},
//     //     new ElementGap{0},
//     //     new TextDisplay{0, 0, "Music", &Framework::fonts.scoreFont, Color{255, 255, 255, 255}},
//     //     new LiveTextDisplay{0, 0, &Framework::fonts.scoreFont, Color{255, 255, 255, 255}, musicTextCb},
//     //     new ElementGap{0},
//     //     new Button{0, 0, Framework::images.exitBtnImg, Framework::images.exitBtnHvrImg}
//     // };


//     // ScrollView scroll{200, 100, 300, 250, 20, 20, elements, 20, 125, SDL_Color{255, 0, 0, 0}, SDL_Color{}};


//     // std::vector<MenuCb> genMenuCbs{
//     //     [](SDL_Renderer*, SDL_Surface*) -> MenuReturn {
//     //         return {{1, -1}};
//     //     }
//     // };

//     // AudioManager& audio{Framework::audioManager};

//     // GeneralMenu genMenu{
//     //     Framework::display.canvasWidth() / 2.0f, 100, 20,
//     //     elements, genMenuCbs, &mouse, bgImg, 0, 0,
//     //     nullptr, initCb, 
//     //     [&audio]() -> void {
//     //         SoundEffect sound{audio.getSoundEffect("BALL", "BALL_SOUND")};
//     //         sound.playOnce();
//     //     },
//     //     [&audio]() -> void {
//     //         SoundEffect sound{audio.getSoundEffect("BALL", "BALL_SOUND")};
//     //         sound.playOnce();
//     //     }
//     // };



//     // std::vector<Button*> btns{};
//     // btns.push_back(new Button{button});
//     // btns.push_back(new Button{button});

//     // std::vector<MenuCb> cbs{};
//     // cbs.push_back({
//     //     [&genMenu](SDL_Renderer* renderer, SDL_Surface* currentCanvas) -> MenuReturn {
//     //         return genMenu.run(renderer, currentCanvas);
//     //     }
//     // });
//     // cbs.push_back({
//     //     [](SDL_Renderer*, SDL_Surface*) -> MenuReturn {
//     //         return {{1, -1}};
//     //     }
//     // });


//     // ButtonMenu menu{
//     //     btns, cbs, 400, 400, 200, &mouse, nullptr, 0, 0,
//     //     [&audio]() -> void {
//     //         SoundEffect sound{audio.getSoundEffect("BALL", "BALL_SOUND")};
//     //         sound.playOnce();
//     //     }
//     // };


//     ControlInput input{
//         600, 600, 10, 200, 
//         "test", "w", 
//         &Framework::fonts.scoreFont, 
//         Color{0, 255, 0, 255}, 
//         Color{0, 0, 255, 255}, 
//         Framework::display.renderer()
//     };

//     ControlInput input2{input};
//     input2.setCenter(Framework::display.canvasWidth() / 2.0f, Framework::display.canvasHeight() / 2.0f);


//     std::unordered_map<std::string, UserInput> controls{
//         {"LEFT", UserInput{SDL_SCANCODE_A}},
//         {"RIGHT", UserInput{SDL_SCANCODE_D}},
//         {"UP", UserInput{SDL_SCANCODE_W}},
//         {"DOWN", UserInput{SDL_SCANCODE_S}},
//         {"SHRINK", UserInput{SDL_SCANCODE_SPACE}}
//     };

//     PlayerController pCon{controls, {"LEFT", "RIGHT", "UP", "DOWN"}};

//     auto saveCb{
//         [&pCon](const std::unordered_map<std::string, UserInput>& controls) -> void {
//             pCon.updateControls(controls);
//             std::cout << "yes";
//         }
//     };

//     ControlMenu conMenu{
//         controls, 
//         &mouse, 
//         saveCb,
//         250, 30, 20, 10, 30,
//         &Framework::fonts.scoreFont,
//         Color{255, 0, 0, 255}, Color{0, 255, 0, 255},
//         bgImg, Framework::display.renderer(), 
//         Color{255, 0, 0, 255}, Color{0, 0, 0, 0}
//     };


//     FRect rect{600, 400, 50, 50};
//     bool toggle {false};

//     bool running {true};
//     bool prepMenuTexture {false};
//     bool enterMenu {false};
//     SDL_Surface* currRender {nullptr};


//     while (running) {
//         bool mousePressed {false};
//         bool mouseReleased {false};

//         SDL_Event event{};
//         while (SDL_PollEvent(&event)) {
//             if (event.type == SDL_EVENT_QUIT) {
//                 running = false;
//             } else if (event.type == SDL_EVENT_KEY_DOWN) {
//                 if (event.key.scancode == SDL_SCANCODE_BACKSPACE) {
//                     running = false;
//                 } else if (event.key.scancode == SDL_SCANCODE_ESCAPE) {
//                     prepMenuTexture = true;
//                 }
//             } else if (event.type == SDL_EVENT_MOUSE_MOTION) {
//                 mouse.update(event.motion.xrel, event.motion.yrel);
//             } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
//                 if (event.button.button == SDL_BUTTON_LEFT) {
//                     mousePressed = true;
//                 }
//             } else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
//                 if (event.button.button == SDL_BUTTON_LEFT) {
//                     mouseReleased = true;
//                 }
//             }
//         }


//         if (enterMenu) {
//             enterMenu = false;
//             // menu.run(Framework::display.renderer(), currRender);
//             conMenu.run(Framework::display.renderer(), nullptr);
//         }


//         SDL_FPoint mousePos {mouse.getPos()};

//         SDL_SetRenderDrawColor(Framework::display.renderer(), 150, 150, 150, 255);
//         SDL_RenderClear(Framework::display.renderer());
//         // genMenu.runUpdate(Framework::display.renderer(), mousePos, mousePressed, mouseReleased);
        
//         button.update(Framework::display.renderer(), mousePos, mousePressed, mouseReleased);

//         if (button.clicked()) {
//             toggle = !toggle;
//         }

//         int red {(toggle) ? 0 : 255};
//         int green {(toggle) ? 255 : 0};

//         SDL_SetRenderDrawColor(Framework::display.renderer(), red, green, 0, 255);
//         SDL_RenderFillRect(Framework::display.renderer(), &rect.getSDLFRect());
//         // slider.update(Framework::display.renderer(), mousePos, mousePressed, mouseReleased);
//         hardText.update(Framework::display.renderer());
//         // liveText.update(Framework::display.renderer());
//         // scroll.update(Framework::display.renderer(), mousePos, mousePressed, mouseReleased);
//         input2.update(Framework::display.renderer(), mousePos, mousePressed, mouseReleased);
//         mouse.draw(Framework::display.renderer());

//         if (prepMenuTexture) {
//             SDL_DestroySurface(currRender);
//             currRender = SDL_RenderReadPixels(Framework::display.renderer(), nullptr);
//             prepMenuTexture = false;
//             enterMenu = true;
//         }

//         SDL_RenderPresent(Framework::display.renderer());
//     }

//     SDL_DestroyTexture(bgImg);
//     SDL_DestroySurface(currRender);

//     return 0;
// };