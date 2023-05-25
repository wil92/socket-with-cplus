//
// Created by Guillermo on 25.05.23.
//

#ifndef MESSAGE_TEST_BASE64_CPP
#define MESSAGE_TEST_BASE64_CPP

#include <map>

struct base64 {
    constexpr static char alp[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";

    static void encode(const char *text, int len, char *encoded, int *resultLen) {
        *resultLen = 0;
        for (int i = 0; i < len; i += 3) {
            int buff = (int) text[i] << 16, split = 18, bits = 8;
            if (i + 1 < len) {
                buff += (int) (text[i + 1]) << 8;
                bits = 16;
            }
            if (i + 2 < len) {
                buff += (int) (text[i + 2]);
                bits = 24;
            }
            for (int j = 0; j < 4; ++j) {
                encoded[(*resultLen)++] = j * 6 <= bits ? alp[(buff >> split) & 63] : '=';
                split -= 6;
            }
        }
        encoded[*resultLen] = '\0';
    }

    static void decode(const char *text, int len, char *encoded, int *resultLen) {
        *resultLen = 0;

        std::map<char, int> m;
        for (int i = 0; i < 65; ++i) {
            m.insert({alp[i], i});
        }

        for (int i = 0; i < len; i += 4) {
            int buff = (m[text[i]] << 18) + (m[text[i + 1]] << 12), split = 16, toTake = 3;
            if (text[i + 3] != '=') {
                buff += m[text[i + 3]];
            } else {
                toTake = 2;
            }
            if (text[i + 2] != '=') {
                buff += m[text[i + 2]] << 6;
            } else {
                toTake = 1;
            }
            for (int j = 0; j < toTake; ++j) {
                encoded[(*resultLen)++] = (char) ((buff >> split) & 511);
                split -= 8;
            }
        }
        encoded[*resultLen] = '\0';
    }
};

#endif //MESSAGE_TEST_BASE64_CPP
