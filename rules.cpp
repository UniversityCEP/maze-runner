#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include <vector>
#include "levels.hpp"

using namespace std;
using namespace sf;

// Helper: put a text object in the horizontal center of the window
void centerText(Text& text, float windowWidth, float y)
{
    FloatRect bounds = text.getLocalBounds();
    text.setOrigin({bounds.position.x + bounds.size.x / 2.f, 0.f});
    text.setPosition({windowWidth / 2.f, y});
}

// Helper: center a text object inside a rectangle (used for buttons)
void centerTextInBox(Text& text, const RectangleShape& box)
{
    FloatRect b = text.getLocalBounds();
    text.setOrigin({b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f});
    text.setPosition({box.getPosition().x + box.getSize().x / 2.f,
                      box.getPosition().y + box.getSize().y / 2.f});
}

//make a text object in one line instead of five
Text makeText(const Font& font, const string& str,
              unsigned int size, Color color, float x, float y)
{
    Text t(font, str, size);
    t.setFillColor(color);
    t.setPosition({x, y});
    return t;
}

void HowToPlay()
{
    const float WIDTH  = 800.f;
    const float HEIGHT = 600.f;

    RenderWindow window(VideoMode({800, 600}), "How To Play");

    // ---------- Colors ----------
    Color green(125, 160, 95);      // background
    Color brown(95, 70, 60);        // text + buttons
    Color darkBrown(65, 45, 38);    // keycap shadow
    Color beige(222, 203, 170);     // panel
    Color white(255, 255, 255);

    // ---------- Font ----------
    Font font("assets/font.ttf");

    // ---------- Title ----------
    Text title(font, "How To Play", 40);
    title.setFillColor(brown);
    centerText(title, WIDTH, 28.f);

    // ---------- Panel ----------
    RectangleShape panel({640.f, 405.f});
    panel.setFillColor(beige);
    panel.setPosition({80.f, 95.f});

    // ---------- Normal text (headings + sentences) ----------
    vector<Text> texts;

    // GOAL section
    texts.push_back(makeText(font, "THE GOAL", 24, brown, 110.f, 112.f));
    texts.push_back(makeText(font, "Dash through the twisting hedges and", 20, brown, 110.f, 148.f));
    texts.push_back(makeText(font, "find the exit before you get lost!",   20, brown, 110.f, 176.f));

    // CONTROLS section
    texts.push_back(makeText(font, "HOW TO MOVE", 24, brown, 110.f, 222.f));
    texts.push_back(makeText(font, "...or use the Arrow Keys if you prefer.", 20, brown, 110.f, 318.f));

    // TIPS section
    texts.push_back(makeText(font, "RUNNER TIPS", 24, brown, 110.f, 360.f));
    texts.push_back(makeText(font, "- Hit a dead end? Turn back and try another path.", 18, brown, 110.f, 396.f));
    texts.push_back(makeText(font, "- Think ahead, the shortest path is not always obvious!", 18, brown, 110.f, 426.f));
    texts.push_back(makeText(font, "- Beat a level to unlock the next adventure.", 18, brown, 110.f, 456.f));

    // ---------- WASD keycaps ----------
    vector<string> keyLetters = {"^", "<-", "v", "->"};
    vector<string> keyActions = {"Up", "Left", "Down", "Right"};

    vector<RectangleShape> keyShadows;   // dark shape behind the cap (3D look)
    vector<RectangleShape> keyCaps;      // the brown square
    vector<Text> keyTexts;               // bold white letter
    vector<Text> actionTexts;            // "Up", "Left"...

    float keyStartX  = 110.f;
    float keyY       = 262.f;
    float keySpacing = 148.f;

    for (int i = 0; i < 4; i++)
    {
        float x = keyStartX + i * keySpacing;

        // shadow (a little lower than the cap)
        RectangleShape shadow({44.f, 44.f});
        shadow.setFillColor(darkBrown);
        shadow.setPosition({x, keyY + 4.f});
        keyShadows.push_back(shadow);

        // cap
        RectangleShape cap({44.f, 44.f});
        cap.setFillColor(brown);
        cap.setPosition({x, keyY});
        keyCaps.push_back(cap);

        // bold letter, centered inside the cap
        Text letter(font, keyLetters[i], 24);
        letter.setStyle(Text::Bold);
        letter.setFillColor(white);
        FloatRect b = letter.getLocalBounds();
        letter.setOrigin({b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f});
        letter.setPosition({x + 20.f, keyY + 20.f});
        keyTexts.push_back(letter);

        // action label to the right of the cap
        actionTexts.push_back(makeText(font, keyActions[i], 16, brown, x + 48.f, keyY + 10.f));


    }

    // ---------- BACK (left) and NEXT (right) boxes (just for looks) ----------
    // The panel spans x = 70 to 570, so the buttons line up with its edges.
    RectangleShape backBox({140.f, 50.f});
    backBox.setFillColor(brown);
    backBox.setPosition({80.f, 520.f});          // left side

    Text backText(font, "BACK", 22);
    backText.setFillColor(white);
    centerTextInBox(backText, backBox);

    RectangleShape nextBox({140.f, 50.f});
    nextBox.setFillColor(brown);
    nextBox.setPosition({580.f, 520.f});         // right side (570 - 120)

    Text nextText(font, "NEXT", 22);
    nextText.setFillColor(white);
    centerTextInBox(nextText, nextBox);

    // ---------- Main loop ----------
    while (window.isOpen())
    {
        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();

            if(const auto* mouseEvent=event->getIf<Event::MouseButtonPressed>()){
                if(mouseEvent->button==Mouse::Button::Left){
                    Vector2f mousePosition({
                        static_cast<float>(mouseEvent->position.x),
                        static_cast<float>(mouseEvent->position.y)
                    });

                    if(backBox.getGlobalBounds().contains(mousePosition)){
                        window.close();
                    }
                    if(nextBox.getGlobalBounds().contains(mousePosition)){
                        LevelWindow(window);
                    }
                }
        }
        }
        window.clear(green);

        window.draw(title);
        window.draw(panel);

        for (const Text& t : texts)
            window.draw(t);

        // draw the keycaps (shadow first, then cap, then letter)
        for (int i = 0; i < 4; i++)
        {
            window.draw(keyShadows[i]);
            window.draw(keyCaps[i]);
            window.draw(keyTexts[i]);
            window.draw(actionTexts[i]);
        }

        window.draw(backBox);
        window.draw(backText);
        window.draw(nextBox);
        window.draw(nextText);

        window.display();
    }
}