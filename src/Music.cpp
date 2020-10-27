#include <Arduino.h>
#include <map>
#include <vector>
#include <functional>

#define SONG_COUNT 4

const int UP_SONG[][2] PROGMEM = {{659, 125}, {784, 125}, {1319, 125}, {1047, 125}, {1175, 125}, {1568, 125}, {0, 250}};
const int COIN_SONG[][2] PROGMEM = {{988, 100}, {1319, 300}};
const int COFFIN_SONG[][2] PROGMEM = {{185, 483}, {185, 241}, {277, 241}, {247, 483}, {220, 483}, {208, 483}, {208, 241}, {220, 241}, {247, 483}, {220, 241}, {208, 241}, {185, 483}, {185, 241}, {440, 241}, {415, 241}, {440, 241}, {415, 241}, {440, 241}, {185, 483}, {185, 241}, {440, 241}, {415, 241}, {440, 241}, {415, 241}, {440, 241}, {185, 483}, {185, 241}, {277, 241}, {247, 483}, {220, 483}, {208, 483}, {208, 241}, {220, 241}, {247, 483}, {220, 241}, {208, 241}, {185, 483}, {185, 241}, {440, 241}, {415, 241}, {440, 241}, {415, 241}, {440, 241}, {185, 483}, {185, 241}, {440, 241}, {415, 241}, {440, 241}, {415, 241}, {440, 241}, {220, 241}, {220, 241}, {220, 241}, {220, 241}, {277, 241}, {277, 241}, {277, 241}, {277, 241}, {247, 241}, {247, 241}, {247, 241}, {247, 241}, {330, 241}, {330, 241}, {330, 241}, {330, 241}, {370, 241}, {370, 241}, {370, 241}, {370, 241}, {370, 241}, {370, 241}, {370, 241}, {370, 241}, {370, 241}, {370, 241}, {370, 241}, {370, 241}, {247, 241}, {220, 241}, {208, 241}, {165, 241}};
const int EVANGELION_SONG[][2] PROGMEM = {{262, 476}, {311, 476}, {349, 357}, {311, 357}, {349, 238}, {349, 238}, {349, 238}, {466, 238}, {415, 238}, {392, 119}, {349, 238}, {392, 595}, {392, 476}, {466, 476}, {523, 357}, {349, 357}, {311, 238}, {466, 238}, {466, 238}, {392, 238}, {466, 238}, {466, 357}, {523, 595}, {0, 476}, {622, 238}, {466, 119}, {466, 595}, {0, 238}, {622, 238}, {622, 357}, {698, 357}, {466, 238}, {466, 476}, {0, 238}, {466, 238}, {784, 357}, {831, 357}, {784, 238}, {698, 357}, {622, 357}, {698, 238}, {784, 357}, {831, 357}, {784, 238}, {523, 476}, {0, 238}, {523, 119}, {587, 119}, {622, 357}, {622, 357}, {587, 238}, {587, 476}, {0, 238}, {622, 119}, {698, 119}, {831, 357}, {784, 357}, {698, 238}, {622, 476}, {0, 238}, {784, 238}, {784, 357}, {698, 357}, {659, 238}, {698, 476}, {523, 476}, {523, 952}, {587, 952}};
const int SONG_LENGTHS[SONG_COUNT] PROGMEM = {7, 2, 82, 68};
const char *const SONG_TITLES[SONG_COUNT] PROGMEM = {"1-UP", "Coin", "Coffin", "Evangelion"};
const std::array<const int (*)[2], SONG_COUNT> SONGS = {UP_SONG, COIN_SONG, COFFIN_SONG, EVANGELION_SONG};

class Music
{
public:
    static const int BUZZER_PIN = D5;
    static const int COUNT = SONG_COUNT;
    static const char *const getTitle(const int i)
    {
        return SONG_TITLES[i];
    }
    static void play(const char title[])
    {
        for (int i = 0; i < SONG_COUNT; i++)
            if (strcmp(title, SONG_TITLES[i]) == 0)
            {
                play(i);
                return;
            }
    }
    static void play(const int song)
    {
        Serial.println(SONG_TITLES[song]);
        playSong(SONGS[song % SONG_COUNT], SONG_LENGTHS[song % SONG_COUNT]);
    }
    static void playRandom()
    {
        play(random(SONG_COUNT));
    }
    static void playCoin()
    {
        playSong(COIN_SONG, sizeof(COIN_SONG) / sizeof(COIN_SONG[0]));
    }

private:
    static void playSong(const int(song)[][2], int songLength)
    {
        for (int note = 0; note < songLength; note++)
        {
            const int noteDuration = song[note][1];
            tone(BUZZER_PIN, song[note][0], noteDuration);
            delay(noteDuration);
            noTone(BUZZER_PIN);
        }
    }
};
