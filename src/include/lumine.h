#ifndef LUMINE_LUMINE_H
#define LUMINE_LUMINE_H

#define COLS_LEN 110
#define LINE_LEN 28

class screen
{
    void display_thread();

    bool _exit = false;
    bool _is_exited = false;
public:
    screen();
    ~screen();

    char v_memory [LINE_LEN] [COLS_LEN + 1] { };

    void detach();
};

#endif //LUMINE_LUMINE_H
