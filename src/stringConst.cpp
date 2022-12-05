
#include <Arduino.h>
char relayMqttTopicBase2[] = "h2/c01/";  // converted to pas string in s.cpp

#define N 0
#define NE 1
#define E 2
#define SE 3
#define S 4
#define SW 0
#define W 6
#define NW 7
#define FLOOR(S) (S << 3)
#define RELAY_No FLOOR

#define building "House"

#define NoRelays 8


const char rooms[] =
    "outside"    // 0, 7
    "hall"       // 7, 4
    "junk"       // 11, 4
    "office"     // 15, 6
    "kitchen"    // 21, 7
    "vestibule"  // 28, 9
    ;
const char rooms_str_len[][2]{{0, 7}, {7, 4}, {11, 4}, {15, 6}, {21, 7}, {28, 9}};  // Start index then string length.
#define Outside 1                                                                   // the start array index for the word outside.
#define Hall 2
#define Junk 3
#define Office 4
#define Kitchen 5
#define Vestibule 6

/* // upstairs main house.
const char MqttTopics[][3] = {
    // room name, where the room is, where in the room.
    {Outside, S, E}, //  "outside/s/e" outside on the east wall and the light on the east end of the wall.
    {Outside, E, S}, // "outside/e/s"
    {Outside, S, W},
    {Hall, E + FLOOR(2), S}, // floor is house floor here. i.e. The upstairs hall.
    {Office, FLOOR(2), 0},   // upstairs Office
    {Junk, FLOOR(2), E},     // upstairs Junk room, used to be office.
    {Junk, FLOOR(2), E + RELAY_No(2)},
    {Junk, FLOOR(2), W},
};
 */

// upstairs main house.
const char MqttTopics[][3] = {
    // room name, where the room is, where in the room.
    {Outside, W, S},                //  "Outside getta's south end of west wall.
    {Outside, W, S + RELAY_No(2)},  // Outside getta's. 2nd light on west wall from the south end.
    {Outside, W, S + RELAY_No(3)},
    {Hall, E + FLOOR(2), S},  // floor is house floor here. i.e. The upstairs hall.
    {Office, FLOOR(2), 0},    // upstairs Office
    {Junk, FLOOR(2), E},      // upstairs Junk room, used to be office.
    {Junk, E + FLOOR(2), E + RELAY_No(2)},
    {Junk, FLOOR(2), W},
};

byte direction(byte x, byte r[]) {
    if (x > 7) return 0;
    if (x == N) {
        r[0] = 'n';
    } else if (x == NE) {
        r[0] = 'n';
        r[1] = 'e';
    } else if (x == E) {
        r[0] = 'e';
    } else if (x == SE) {
        r[0] = 's';
        r[1] = 'e';
    } else if (x == S) {
        r[0] = 's';
    } else if (x == SW) {
        r[0] = 's';
        r[1] = 'w';
    } else if (x == W) {
        r[0] = 'w';
    } else if (x == NW) {
        r[0] = 'n';
        r[1] = 'w';
    }
    return (1 + (x bitand 0b1));  // even values of x return a 2 char long string.
}

/**
 * @brief return the name of the room/etc. as an array of char.
 *
 * @param x room string index
 * @param r Pointer to Array of char, string. Used as array, no length or zero byte.
 * @param l the sizeof(r) the array.
 * @return byte number of chars copied to r.
 */
byte getRoomStr(byte x, byte r[], byte l) {
    byte i, si, sl;
    si = rooms_str_len[x - 1][0];  // start index
    sl = rooms_str_len[x - 1][1];  // word length.
    if (sl > l) { sl = l; }
    for (i = 0; i < sl; i++) {
        r[i] = rooms[si + i];
    }
    return i;
}

byte inline byteToStr(byte x, byte r[], byte l) {
    // Serial.print(F("byteToStr(byte x,byte r[], byte l), x="));
    // Serial.print(x);
    // Serial.print(F(", l="));
    // Serial.print(l);
    byte c;
    // char tC;
    // byte i;
    c = 1;
    if (x > 99) {
        if (l >= 3) {
            r[2] = '0' + (x / 100);
        }
        x %= 100;
        c = 2;
    }
    if (x > 9) {
        if (l >= 2) {
            r[1] = '0' + (x / 10);
        }
        x %= 10;
        if (c < 2) c = 2;
    }
    r[0] = '0' + x;
    // Serial.print(F("r[0.."));
    // Serial.print(c);
    // Serial.print(F("] = "));
    // for (i = 0; i < c; i++) {
    //     tC = r[i];
    //     Serial.print(tC);
    // }
    //    Serial.println();
    return c;
}

byte buildTopicForRelay(uint8_t relay, uint8_t r[], byte l) {
    byte i, w, x;
    // first get room name.
    w = MqttTopics[relay - 1][0];  // room word number. 1 is 'outside'
    i = getRoomStr(w, r + 1, l);   // MqttTopics[0] r = 'outside'
    if ((i + 2) >= l) {
        Serial.println(F("out of string space"));
        r[l - 1] = 0;
        r[0] = i;
        return l - 1;
    }

    // add floor and side in room or outside wall direction. Using north south etc.
    i++;
    r[i] = '/';                    // example r = 'outside/'
    w = MqttTopics[relay - 1][1];  // direction(bitand 0b111) and floor(>>3)
    x = w bitand 0b111;            // direction
    w >>= 3;                       // floor
    i += direction(x, r + i + 1);  // first direction. the side of a room or house etc.
    // MqttTopics[0]{Outside, W, S} r = 'outside/w'
    if (x <= 7) {
        i++;
        r[i] = '/';  // MqttTopics[0]  r = 'outside/w/'
    }
    if (w >= 2) {            // if floor is not ground level. 1 is ground floor, 0 is not specified.
        if ((i + 3) >= l) {  // need at least 3 char for '/fx' where x is a digit.
            r[i + 1] = 0;
            r[0] = i;
            return i;
        }
        // r[i] = '/';
        i++;
        r[i] = 'f';
        i += byteToStr(w, r + (i + 1), l - i);  // MqttTopics[3]{Hall, E + FLOOR(2), S} r =  'hall/e/f2/s'
        i++;
        r[i] = '/';
    }
    // Add the starting point( north etc. (bitand ob111)) in/on the room/wall above and moving away (>> 3) lights.
    w = MqttTopics[relay - 1][2];  // direction(bitand 0b111) and relay/light/plug etc (>>3)
    x = w bitand 0b111;
    w >>= 3;
    i += direction(x, r + i + 1);
    // MqttTopics[0]{Outside, W, S}  r = 'outside/w/s/'
    if (w >= 2) {            // If there is only 1 light etc. then no need to add the light number.
        if ((i + 1) >= l) {  // need at least 1 char for 'x' where x is a digit.
            r[l - 1] = 0;
            r[0] = i;
            return l - 1;
        }
        i += byteToStr(w, r + (i + 1), l - i);
        // MqttTopics[1]{Outside, W, S + RELAY_No(2)}  r = 'outside/w/s2'
    }

    r[0] = i;
    if (i >= l) {
        r[l] = 0;
        r[0] = l;
        return l;
    } else {
        r[i + 1] = 0;
    }
    return i;
}

/**
 * @brief print relay/output MQTT topics
 * 
 * @param r buff to build the string in
 * @param l max string length, buffer needs to be 2 bytes longer for length and zero bytes.
 */
void printRelayTopics(byte r[], byte l) {
    byte i;
    Serial.println(F("Printing rooms..."));
    for (i = 1; i <= NoRelays; i++) {
        byte l;
        l = buildTopicForRelay(i, r, l);
        {  // debug code.
            Serial.print(F("Print relay No.:"));
            Serial.println(i);
            Serial.print(F("\tTotal string length now:"));
            Serial.println(l);
            Serial.print(F("\tMQTT topic: "));
            byte z;
            for (z = 1; z <= l; z++) {
                if (r[z] <= 32) {
                    Serial.print('[');
                    Serial.print(r[z]);
                    Serial.print(']');
                } else {
                    Serial.print((char)r[z]);
                }
            }
            Serial.println("\nEnd.");
            // Serial.println("delay 2 seconds");
            // delay(2000);
        }
    }
}