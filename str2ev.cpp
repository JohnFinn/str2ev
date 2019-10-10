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
    sleep(1);
    fake_do_key(Release, key);
}

void fake_type(wchar_t symbol)
{
    fake_do_key(Press, KEY_LEFTSHIFT);
    fake_do_key(Press, KEY_LEFTCTRL);
    fake_type_key(KEY_U);

    char* bytes = reinterpret_cast<char*>(&symbol);
    for (int i = 0; i <= sizeof(wchar_t); ++i) {
        char current_byte = bytes[i];
        char half1 = current_byte >> 4;
        char half2 = current_byte & 0x0f;
        fake_type_key(keys[half1]);
        fake_type_key(keys[half2]);
    }
    fake_do_key(Release, KEY_LEFTSHIFT);
    fake_do_key(Release, KEY_LEFTCTRL);
}


std::ostream& operator<<(std::ostream& stream, const struct timeval& tv)
{
    return stream << tv.tv_sec << "s" << tv.tv_usec << "us";
}

const char* input_event_type_to_str(__u16 type)
{
    switch (type)
    {
    case EV_SYN: return "EV_SYN";
    case EV_KEY: return "EV_KEY";
    case EV_REL: return "EV_REL";
    case EV_ABS: return "EV_ABS";
    case EV_MSC: return "EV_MSC";
    case EV_SW:  return "EV_SW";
    case EV_LED: return "EV_LED";
    case EV_SND: return "EV_SND";
    case EV_REP: return "EV_REP";
    case EV_FF:  return "EV_FF";
    case EV_PWR: return "EV_PWR";
    case EV_FF_STATUS: return "EV_FF_STATUS";
    default: return "BAD";
    }
}

std::ostream& operator<<(std::ostream& stream, const struct input_event& ev)
{
    return stream
    << " type="  << input_event_type_to_str(ev.type)
    << " code=0x"  << std::hex << ev.code 
    << " value=0x" << std::hex << ev.value 
    /* << " time=" << ev.time */
    ;
}

int main(int argc, const char** argv)
{

    std::locale::global(std::locale("en_US.utf8"));
    fake_type_key(KEY_A);
    // struct input_event ev;
    // while (read(0, &ev, sizeof(ev))) std::cout << ev << '\n';



    // for (std::wstring current; getline(std::wcin, current);) {
    //     for (auto c : current) {
    //         fake_type(c);
    //     }
    // }
}
