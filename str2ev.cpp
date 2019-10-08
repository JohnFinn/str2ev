#include <unistd.h>
#include <linux/input.h>
#include <linux/input-event-codes.h>

#include <ctype.h>

#include <stdio.h>
#include <locale>
#include <iostream>

enum KEY_ACTION {
    Release = 0,
    Press = 1,
    Autorepeat = 2
};

int main(int argc, const char** argv)
{
    struct input_event ev;
    ev.type = EV_KEY;
    ev.value = Press;
    ev.code = KEY_U;

    std::locale::global(std::locale("en_US.utf8"));

    for (std::wstring current; getline(std::wcin, current);) {
        for (auto c : current)
            std::cout << std::hex << (int)c << ' ';
        std::cout << '\n';
    }
}
