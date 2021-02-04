#ifndef AETHER_DISPLAY_HPP
#define AETHER_DISPLAY_HPP

#include "Aether/Overlay.hpp"
#include <stack>

namespace Aether {
    /**
     * @brief A display represents a root element.
     *
     * It stores and handles different screens and
     * polling/passing events.
     */
    class Display : public Element {
        /**
         * @brief Enum class containing all stack operations
         */
        enum class StackOp {
            Push, /**< Stack Push Operation */
            Pop, /**< Stack Pop Operation */
            None /**< No Stack Operation */
        };

        private:
            /** @brief Indicator if whether the app should continue running the main loop */
            bool loop_;
            /** @brief Indicator on whether the FPS should be displayed */
            bool fps_;
            /** @brief Colour to clear screen with */
            Colour bg;
            /** @brief Texture (image) to clear screen with */
            SDL_Texture * bgImg;
            /** @brief Highlight animation funtion to use */
            std::function<Colour(uint32_t)> hiAnim;
            /** @brief Button that is being held, if any */
            Button heldButton;
            /** @brief Time that the button was held for (in ms), if any button is currently held */
            int heldTime;
            /** @brief Time delay before button is considered held instead of new button press */
            int holdDelay_;
            /** @brief Vector of overlays, drawn from start -> end but only last one gets events! */
            std::vector<Overlay *> overlays;
            /** @brief Pointer to current screen to draw */
            Screen * screen;
            /** @brief Pointer to screen to change to after loop */
            Screen * nextScreen;
            /** @brief Stack of screens */
            std::stack<Screen *> screenStack;
            /** @brief Indicator if stack operation was performed */
            StackOp stackOp;
            /** @brief Indicator on whether display is fading */
            bool fading;
            /** @brief The current alpha value for display fade */
            short fadeAlpha;
            /** @brief Indicator on whether display should fade in on entry */
            bool fadeIn;
            /** @brief Indicator on whether display should fade out on exit */
            bool fadeOut;

            // These functions are private members of a display
            // as they are called by loop()
            using Element::addElement;
            using Element::handleEvent;
            using Element::update;
            using Element::render;

        public:
            /**
             * @brief Construct a new Display object
             *
             * Initializes SDL with Vsync on
             */
            Display();

            /**
             * @brief Toggle whether FPS should be showed
             *
             * @param b state to change show FPS status to
             */
            void setShowFPS(bool b);

            /**
             * @brief Set colour to clear screen with
             *
             * @param r red value of color
             * @param g green value of color
             * @param b blue value of color
             */
            void setBackgroundColour(uint8_t r, uint8_t g, uint8_t b);

            /**
             * @brief Set image to use as background (returns if successful)
             *
             * @param path Path to background image
             * @return true if successfully set
             * @return false otherwise
             */
            bool setBackgroundImage(std::string path);

            /**
             * @brief Set colours to highlight elements with
             *
             * @param bg Color to clear screen with
             * @param sel Color for highlighted element
             */
            void setHighlightColours(Colour bg, Colour sel);

            /**
             * @brief Set the highlight animation fir display
             *
             * @param f the function to set the display's animation function as
             */
            void setHighlightAnimation(std::function<Colour(uint32_t)> f);

            /**
             * @brief Set font to use with display
             *
             * @param p File path for font file to use
             */
            void setFont(std::string p);

            /**
             * @brief Getter function for hold delay
             *
             * @return Time between repeated button events (in ms)
             */
            int holdDelay();

            /**
             * @brief Setter function for hold delay
             *
             * @param d new time between repeated button events (in ms)
             */
            void setHoldDelay(int d);

            /**
             * @brief Add new overlay to current display
             *
             * @param o overlay to add
             */
            void addOverlay(Overlay * o);

            /**
             * @brief Calls onUnload for current screen and removes screen (screen is not deleted)
             *
             * @note Set another screen before loop() or loop() will return false due to no screen set
             */
            void dropScreen();

            /**
             * @brief Set next screen for display
             *
             * @param s next screen pointer
             */
            void setScreen(Screen * s);

            /**
             * @brief Push the current screen to display's screen stack
             *
             * @note onUnload (for current screen) is not called!
             * @note Main loop returns false if another screen is not set before next call of loop()
             */
            void pushScreen();

            /**
             * @brief Pops top screen from the screen stack and sets it as current screen
             *
             * @note onLoad (for popped screen) is not called!
             * @note Current screen will not be deleted - it is up to you!
             */
            void popScreen();

            /**
             * @brief Set display to fade in
             */
            void setFadeIn();

            /**
             * @brief Set display to fade out on exit
             */
            void setFadeOut();

            /**
             * @brief Executes the main loop (events + rendering)
             *
             * @return true while the app's main loop is running
             * @return false when the app's main loop is terminated
             */
            bool loop();

            /**
             * @brief Indicates app to exit by turning loop value to false
             */
            void exit();

            /**
             * @brief Destroy the Display object
             */
            ~Display();
    };
};

#endif