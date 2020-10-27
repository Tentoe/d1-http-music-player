#include <Arduino.h>
#include <map>
#include <vector>
#include <functional>

#define SONG_COUNT 2

const int UP_SONG[][2] PROGMEM = {{659, 125}, {784, 125}, {1319, 125}, {1047, 125}, {1175, 125}, {1568, 125}, {0, 250}};
const int COIN_SONG[][2] PROGMEM = {{988, 100}, {1319, 300}};

const int SONG_LENGTHS[SONG_COUNT] PROGMEM = {7, 2};
const char *const SONG_TITLES[SONG_COUNT] PROGMEM = {"1-UP", "Coin"};
const std::array<const int (*)[2], SONG_COUNT> SONGS = {UP_SONG, COIN_SONG};

class Music
{
public:
    static const int BUZZER_PIN = D5;
    static const int COUNT = 2;
    static const char *const getTitle(const int i){
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
