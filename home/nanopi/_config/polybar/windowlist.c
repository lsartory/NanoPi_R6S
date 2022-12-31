// gcc -Wall -Wextra windowlist.c -lxcb -o windowlist

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <unistd.h>
#include <xcb/xcb.h>

struct shared_t
{
    volatile int running;
    xcb_window_t windows[256];
    char line[8192];
};
static struct shared_t *_shared;

static xcb_atom_t get_atom(xcb_connection_t *c, char *atom_name)
{
    xcb_atom_t atom = 0;
    xcb_intern_atom_cookie_t atom_cookie;
    xcb_intern_atom_reply_t *rep;

    atom_cookie = xcb_intern_atom(c, 0, strlen(atom_name), atom_name);
    rep = xcb_intern_atom_reply(c, atom_cookie, NULL);
    if (rep != NULL)
    {
        atom = rep->atom;
        free(rep);
    }
    return atom;
}

int main(int argc, char *argv[])
{
    int shm_id;
    xcb_connection_t *xcon;
    int xfd;
    xcb_screen_t *screen;
    xcb_atom_t net_active_window;
    xcb_atom_t net_client_list;
    xcb_atom_t net_wm_visible_name;

    shm_id = shmget(0xEB488FA5, sizeof(struct shared_t), IPC_CREAT | 0660);
    if (shm_id < 0)
    {
        perror("shmget");
        return 1;
    }
    _shared = shmat(shm_id, NULL, 0);
    if (_shared == (void *)-1)
    {
        perror("shmat");
        return 2;
    }
    if (argc > 1 && strcmp(argv[1], "stop") == 0)
    {
        _shared->running = 0;
        return 0;
    }
    else if (_shared->running != 0 || (argc > 1 && strcmp(argv[1], "get") == 0))
    {
        fputs(_shared->line, stdout);
        return 0;
    }
    else
        _shared->running = 1;

    xcon = xcb_connect(NULL, NULL);
    if (xcb_connection_has_error(xcon))
    {
        _shared->running = 0;
        return 3;
    }
    xfd = xcb_get_file_descriptor(xcon);
    screen = xcb_setup_roots_iterator(xcb_get_setup(xcon)).data;

    net_active_window   = get_atom(xcon, "_NET_ACTIVE_WINDOW");
    net_client_list     = get_atom(xcon, "_NET_CLIENT_LIST");
    net_wm_visible_name = get_atom(xcon, "_NET_WM_NAME");

    sleep(1);

    int mask = XCB_EVENT_MASK_STRUCTURE_NOTIFY | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY | XCB_EVENT_MASK_PROPERTY_CHANGE;
    if (xcb_request_check(xcon, xcb_change_window_attributes_checked(xcon, screen->root, XCB_CW_EVENT_MASK, &mask)))
    {
        _shared->running = 0;
        return 4;
    }

    while (_shared->running != 0)
    {
        xcb_get_property_reply_t *gpr;
        int window_count;
        xcb_window_t active_window;
        xcb_window_t *windows;
        fd_set fds;
        struct timespec timeout = { 1, 0 };
        int offset = 0;

        gpr = xcb_get_property_reply(xcon, xcb_get_property(xcon, 0, screen->root, net_active_window, XCB_ATOM_WINDOW, 0, sizeof(xcb_window_t)), NULL);
        if (gpr == NULL)
        {
            sleep(1);
            continue;
        }
        active_window = *(xcb_window_t *)xcb_get_property_value(gpr);
        free(gpr);

        gpr = xcb_get_property_reply(xcon, xcb_get_property(xcon, 0, screen->root, net_client_list, XCB_ATOM_WINDOW, 0, sizeof(_shared->windows) / sizeof(*_shared->windows)), NULL);
        if (gpr == NULL)
        {
            sleep(1);
            continue;
        }
        window_count = xcb_get_property_value_length(gpr) / sizeof(*windows);
        windows = (xcb_window_t *)xcb_get_property_value(gpr);

        _shared->line[0] = 0;
        for (int i = 0; i < window_count; i++)
        {
            _shared->windows[i] = windows[i];
            xcb_get_property_reply_t *gpr2 = xcb_get_property_reply(xcon, xcb_get_property(xcon, 0, windows[i], net_wm_visible_name, XCB_GET_PROPERTY_TYPE_ANY, 0, 1024), NULL);
            if (gpr2)
            {
                int len = xcb_get_property_value_length(gpr2);
                if (len != 0)
                    offset += snprintf(_shared->line + offset, sizeof(_shared->line) - offset, "%s%s%%{A1: bspc node -f 0x%08x & disown:}%.*s%%{A}%s", (i == 0 ? "" : " | "), (active_window != windows[i] ? "" : "%{F#00AAFF}%{+u}%{u#00AAFF}"), windows[i], len, (char *)xcb_get_property_value(gpr2), (active_window != windows[i] ? "" : "%{-u}%{F-}"));
                free(gpr2);
            }
        }
        _shared->line[sizeof(_shared->line) - 1] = 0;
        free(gpr);
        if (system("polybar-msg hook windowlist 1 > /dev/null") != 0)
        {
            _shared->running = 0;
            return 0;
        }

        FD_ZERO(&fds);
        FD_SET(xfd, &fds);
        pselect(xfd + 1, &fds, NULL, NULL, &timeout, NULL);
        for (;;)
        {
            xcb_generic_event_t *ev = xcb_poll_for_event(xcon);
            if (ev == NULL)
                break;
            free(ev);
        }
        // TODO: get window name change events?
    }

    return 0;
}
