#include <unistd.h>
#include <linux/input.h>
#include <linux/input-event-codes.h>

#include <ctype.h>

#include <stdio.h>
#include <locale>
#include <iostream>
#include <sstream>

enum KEY_ACTION {
    Release = 0,
    Press = 1,
    Autorepeat = 2
};

int keys[] = {
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
};

void fake_do_key(KEY_ACTION action, int key)
{
    struct input_event ev;

    ev.type = EV_MSC;
    ev.code = MSC_SCAN;
    ev.value = 0x70004;
    gettimeofday(&ev.time, 0);
    write(1, &ev, sizeof(ev));

    ev.type = EV_KEY;
    ev.value = action;
    ev.code = key;
    write(1, &ev, sizeof(ev));

    ev.type = EV_SYN;
    ev.code = SYN_REPORT;
    ev.value = SYN_REPORT;
    write(1, &ev, sizeof(ev));
}

void fake_type_key(int key) {
    fake_do_key(Press, key);
    fake_do_key(Release, key);
}

void fake_type(wchar_t symbol)
{
    fake_do_key(Press, KEY_LEFTSHIFT);
    fake_do_key(Press, KEY_LEFTCTRL);
    fake_type_key(KEY_U);
    fake_do_key(Release, KEY_LEFTSHIFT);
    fake_do_key(Release, KEY_LEFTCTRL);

    // TODO handle endiannes
    char* bytes = reinterpret_cast<char*>(&symbol);
    bool start_zeros = true;
    for (int i = 0; i < sizeof(wchar_t); ++i) {
        char current_byte = bytes[sizeof(wchar_t) - 1 - i];
        if (start_zeros) {
            if (current_byte == 0)
                continue;
            else
                start_zeros = false;
        }
        char half1 = current_byte >> 4;
        char half2 = current_byte & 0x0f;
        fake_type_key(keys[half1]);
        fake_type_key(keys[half2]);
        std::cerr << std::hex << (int)half1 << ' ' << std::hex << (int)half2 << '\n';
    }

    fake_type_key(KEY_ENTER);
}

int main(int argc, const char** argv)
{
    for (std::wstring current; getline(std::wcin, current);) {
        for (auto c : current) {
            fake_type(c);
        }
    }
}
