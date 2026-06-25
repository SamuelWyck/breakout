// #include "./sdlUtils/userInterface/elements/button.h"
// #include "./sdlUtils/userInterface/elements/elementGap.h"

// #include <SDL3/SDL.h>
// #include <string>
// #include <vector>
// #include <format>
// #include <SDL3_image/SDL_image.h>
// #include "./framework/framework.h"
// #include "./sdlUtils/userInterface/mouse.h"
// #include <iostream>
// #include "./sdlUtils/color.h"
// #include "./sdlUtils/userInterface/generalMenu.h"
// #include "./sdlUtils/userInterface/selectMenu.h"



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


//     std::vector<SDL_Texture*> btnImages{};
//     std::vector<Button*> btns{};

//     for (int i {0}; i < 26; i += 1) {
//         SDL_Texture* btnImg {Framework::fonts.scoreFont.renderTexture(
//             Framework::display.renderer(), std::to_string(i), SDL_Color{255, 0, 0, 255}
//         )};
//         SDL_Texture* btnHvrImg {Framework::fonts.scoreFont.renderTexture(
//             Framework::display.renderer(), std::to_string(i), SDL_Color{0, 255, 0, 255}
//         )};
//         btnImages.push_back(btnImg);
//         btnImages.push_back(btnHvrImg);

//         btns.push_back(new Button{0, 0, btnImg, btnHvrImg});
//     }

//     SDL_Texture* exitBtnImg {Framework::fonts.scoreFont.renderTexture(
//         Framework::display.renderer(), "EXIT", SDL_Color{0, 0, 255, 255}
//     )};
//     Button* exitBtn{new Button{0, 0, exitBtnImg}};
//     btnImages.push_back(exitBtnImg);

//     SDL_Texture* upBtnImg {Framework::fonts.scoreFont.renderTexture(
//         Framework::display.renderer(), "PAGE UP", SDL_Color{0, 0, 255, 255}
//     )};
//     Button* upBtn{new Button{0, 0, upBtnImg}};
//     btnImages.push_back(upBtnImg);

//     SDL_Texture* downBtnImg {Framework::fonts.scoreFont.renderTexture(
//         Framework::display.renderer(), "PAGE DOWN", SDL_Color{0, 0, 255, 255}
//     )};
//     Button* downBtn{new Button{0, 0, downBtnImg}};
//     btnImages.push_back(downBtnImg);

//     SDL_Texture* lockImg {Framework::fonts.scoreFont.renderTexture(
//         Framework::display.renderer(), "X", SDL_Color{0, 0, 255, 255}
//     )};
//     btnImages.push_back(lockImg);

//     SelectMenu selectM{
//         &mouse, 
//         0,
//         Framework::display.canvasWidth() / 2.0f,
//         Framework::display.canvasHeight() / 2.0f,
//         3, 4, 20, btns, exitBtn, upBtn, downBtn,
//         bgImg, nullptr, SDL_GetBasePath() + std::string{"/savedId.txt"}
//     };
//     selectM.setHighestUnlockedBtnId(17);



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
//             MenuReturn val {selectM.run(Framework::display.renderer(), 240, currRender)};
//             if (val) {
//                 std::cout << val->first << " " << val->second << "\n";
//             }
//         }


//         SDL_FPoint mousePos {mouse.getPos()};

//         SDL_SetRenderDrawColor(Framework::display.renderer(), 150, 150, 150, 255);
//         SDL_RenderClear(Framework::display.renderer());
        
//         button.update(Framework::display.renderer(), mousePos, mousePressed, mouseReleased);

//         if (button.clicked()) {
//             toggle = !toggle;
//         }

//         int red {(toggle) ? 0 : 255};
//         int green {(toggle) ? 255 : 0};

//         SDL_SetRenderDrawColor(Framework::display.renderer(), red, green, 0, 255);
//         SDL_RenderFillRect(Framework::display.renderer(), &rect.getSDLFRect());

//         selectM.runUpdate(Framework::display.renderer(), mousePos, mousePressed, mouseReleased);
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

//     for (SDL_Texture* img : btnImages) {
//         SDL_DestroyTexture(img);
//     }

//     return 0;
// };