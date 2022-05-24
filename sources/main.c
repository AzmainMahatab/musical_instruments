#include "raylib.h"
#include "string.h"

#define APP_NAME "Musical Instruments"

#define SCREEN_WIDTH (1100)
#define SCREEN_HEIGHT (600)

#define MAX_PER_ROW 7
#define MAX_PER_COL 2


#define NUMBER_OF_INSTRUMENTS 10
#define NUMBER_OF_TUNES 3

#define NAME_FONT_SIZE 20
#define KEY_FONT_SIZE 20
#define APP_SUBTITLE_FONT_SIZE 21


const int imageWidth = SCREEN_WIDTH / (1.6 * MAX_PER_ROW);
const int imageHeight = SCREEN_HEIGHT / (1.6 * MAX_PER_COL);
int firstRowX = imageWidth / 2;
const int firstRowY = imageHeight / 4;

int secondRowX = (SCREEN_WIDTH - imageWidth * (1.5 * (NUMBER_OF_INSTRUMENTS - MAX_PER_ROW) - .5)) / 2;
int secondRowY = imageHeight * 1.7;

int defaultSpacing = 20;


typedef struct {
    char name[100];
    char keyName[100];
    Sound sound;
    Texture2D image;
    KeyboardKey key;
    Rectangle btnBound;
} Instrument;

typedef struct {
    Sound sound;
    KeyboardKey key;
} PureTune;

static const char *tuneFiles[] = {
        "tabla_loop.mp3",
        "ek_tara_loop_a.mp3",
        "ek_tara_loop_b.mp3",
};

static const KeyboardKey tuneKeys[] = {
        KEY_COMMA,
        KEY_PERIOD,
        KEY_SLASH,
};

static const char *instrumentFiles[] = {
        "crash",
        "kick",
        "snare",
        "tom1",
        "tom2",
        "tom3",
        "tom4",
        "tabla_a",
        "tabla_b",
        "ek_tara"
};

static const char *instrumentsNames[] = {
        "Crash",
        "Kick",
        "Snare",
        "Tom A",
        "Tom B",
        "Tom C",
        "Tom D",
        "Tabla A",
        "Tabla B",
        "Ek Tara",
};
static const char *instrumentKeyNames[] = {
        "A",
        "S",
        "D",
        "F",
        "G",
        "H",
        "J",
        "C",
        "V",
        "B",
};

//static const char *subTitle = "Musical Instruments";

static const KeyboardKey instrumentKeys[] = {
        KEY_A,
        KEY_S,
        KEY_D,
        KEY_F,
        KEY_G,
        KEY_H,
        KEY_J,
        KEY_C,
        KEY_V,
        KEY_B,
};


int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, APP_NAME);
    SetTargetFPS(60);


    InitAudioDevice();

    Instrument instruments[NUMBER_OF_INSTRUMENTS];



//    int imageWidth = (SCREEN_HEIGHT / 2) / NUMBER_OF_INSTRUMENTS;

    for (int i = 0; i < NUMBER_OF_INSTRUMENTS; i++) {

        Instrument instrument;

        // Set Names
        strcpy(instrument.name, instrumentsNames[i]);
        strcpy(instrument.keyName, instrumentKeyNames[i]);


        // Prepare textures
        char tempImageFileName[100] = ASSETS_PATH"images/";
        strcat(tempImageFileName, instrumentFiles[i]);
        strcat(tempImageFileName, ".png");
        Image tempImage = LoadImage(tempImageFileName);
        ImageResize(&tempImage, imageWidth, imageHeight);
//        textures[i] = LoadTextureFromImage(tempImage);
        instrument.image = LoadTextureFromImage(tempImage);
        UnloadImage(tempImage);

        // Prepare Sounds
        char tempSoundsFileName[100] = ASSETS_PATH"sounds/";
        strcat(tempSoundsFileName, instrumentFiles[i]);
        strcat(tempSoundsFileName, ".mp3");
//        sounds[i] = LoadSound(tempSoundsFileName);
        instrument.sound = LoadSound(tempSoundsFileName);
        SetSoundVolume(instrument.sound, 1.0f);

        // Set Keys
        instrument.key = instrumentKeys[i];

        float x;
        float y;
        if (i < MAX_PER_ROW) {
            x = (float) (firstRowX + instrument.image.width * 1.5 * i);
            y = (float) firstRowY;
        } else {
            x = (float) (secondRowX + instrument.image.width * 1.5 * (i - 7));
            y = (float) secondRowY;
        }
        instrument.btnBound = (Rectangle) {
                x,
                y,
                (float) instrument.image.width,
                (float) instrument.image.height,
        };

        instruments[i] = instrument;
    }

    // Set Special Tunes
    PureTune tunes[NUMBER_OF_TUNES];
    for (int i = 0; i < NUMBER_OF_TUNES; i++) {
        PureTune tune;
        char tempSoundsFileName[100] = ASSETS_PATH"sounds/";
        strcat(tempSoundsFileName, tuneFiles[i]);
        tune.sound = LoadSound(tempSoundsFileName);
        SetSoundVolume(tune.sound, 1.0f);
        tune.key = tuneKeys[i];
        tunes[i] = tune;
    }


// Vector2 mousePoint = {0.0f, 0.0f};
    while (!WindowShouldClose()) {

// Keyboard Input

        //For Instruments
        for (int i = 0; i < NUMBER_OF_INSTRUMENTS; ++i) {
            if (IsKeyPressed(instruments[i].key)) {
                PlaySoundMulti(instruments[i].sound);
            }
        }
        // For Tunes
        for (int i = 0; i < NUMBER_OF_TUNES; ++i) {
            if (IsKeyPressed(tunes[i].key)) {
                PlaySound(tunes[i].sound);
            }
        }


//Mouse Input
        for (int i = 0; i < NUMBER_OF_INSTRUMENTS; ++i) {
            Vector2 mousePoint = GetMousePosition();
            bool btnAction = false;

            // Check button state
            if (CheckCollisionPointRec(mousePoint, instruments[i].btnBound)) {
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) btnAction = true;
            }
            if (btnAction) {
                PlaySoundMulti(instruments[i].sound);
            }
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        int nameY;
        int keyY;
        for (int i = 0; i < NUMBER_OF_INSTRUMENTS; i++) {
            DrawTexture(
                    instruments[i].image,
                    (int) instruments[i].btnBound.x,
                    (int) instruments[i].btnBound.y,
                    WHITE
            );
            const Vector2 nameTextSize = MeasureTextEx(
                    GetFontDefault(),
                    instruments[i].name,
                    NAME_FONT_SIZE,
                    1
            );

            const Vector2 keyTextSize = MeasureTextEx(
                    GetFontDefault(),
                    instruments[i].keyName,
                    KEY_FONT_SIZE,
                    1
            );

            int nameX = instruments[i].btnBound.x + (instruments[i].image.width - nameTextSize.x) / 2;
            int keyX = instruments[i].btnBound.x + (instruments[i].image.width - keyTextSize.x) / 2;
            if (i == 0) {
                nameY = (int) firstRowY + (int) instruments[0].image.height + (int) nameTextSize.y;
                keyY = (int) firstRowY + (int) instruments[i].image.height + (int) keyTextSize.y + defaultSpacing;
            } else if (i == MAX_PER_ROW) {
                nameY = (int) secondRowY + (int) instruments[0].image.height + (int) nameTextSize.y;
                keyY = (int) secondRowY + (int) instruments[i].image.height + (int) keyTextSize.y + defaultSpacing;
            }
            DrawText(
                    instruments[i].name,
                    nameX,
                    nameY,
                    NAME_FONT_SIZE,
                    BLACK
            );

            DrawText(
                    instrumentKeyNames[i],
                    keyX,
                    keyY,
                    KEY_FONT_SIZE,
                    BLACK
            );
        }
        char *esterText = "< . ?";
        const Vector2 esterEgg = MeasureTextEx(
                GetFontDefault(),
                esterText,
                APP_SUBTITLE_FONT_SIZE,
                1
        );

        DrawText(
                esterText,
                SCREEN_WIDTH * 0.9,
                keyY,
                APP_SUBTITLE_FONT_SIZE,
                BLACK
        );


        const Vector2 appTitle = MeasureTextEx(
                GetFontDefault(),
                APP_NAME,
                APP_SUBTITLE_FONT_SIZE,
                1
        );
        DrawText(
                APP_NAME,
                (SCREEN_WIDTH - appTitle.x) / 2,
                keyY + defaultSpacing,
                APP_SUBTITLE_FONT_SIZE,
                BLACK
        );

        EndDrawing();
    }

    for (int i = 0; i < NUMBER_OF_INSTRUMENTS; ++i) {
        UnloadTexture(instruments[i].image);
        UnloadSound(instruments[i].sound);
    }

    for (int i = 0; i < NUMBER_OF_TUNES; ++i) {
        UnloadSound(tunes[i].sound);
    }

    CloseWindow();
    return 0;
}