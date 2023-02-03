
#include <windows.h>
#include <filter.h>

#define MENU_LL (WM_USER + 1)
#define MENU_RR (WM_USER + 2)

FILTER_DLL filter = {
    FILTER_FLAG_ALWAYS_ACTIVE,
    NULL,NULL,
    const_cast<char*>("LRショートカット"),
    NULL,NULL,NULL,
    NULL,NULL,
    NULL,NULL,NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    func_WndProc,
};
EXTERN_C FILTER_DLL __declspec(dllexport)* __stdcall GetFilterTable(void) {
    return &filter;
}

FILTER* exeditfp;

FILTER* get_exeditfp(FILTER* fp) {
    SYS_INFO si;
    fp->exfunc->get_sys_info(NULL, &si);

    for (int i = 0; i < si.filter_n; i++) {
        FILTER* tfp = (FILTER*)fp->exfunc->get_filterp(i);
        if (tfp->information != NULL) {
            if (!strcmp(tfp->information, "拡張編集(exedit) version 0.92 by ＫＥＮくん")) return tfp;
        }
    }
    return NULL;
}

HWND* ExtendedFilterClass_ptr;
HWND* ll_ptr;
HWND* rr_ptr;

BOOL func_WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam, void* editp, FILTER* fp) {
    switch (message) {
    case WM_FILTER_INIT:
        exeditfp = get_exeditfp(fp);
        if (exeditfp == NULL) {
            MessageBoxA(fp->hwnd, "拡張編集0.92が見つかりませんでした", fp->name, MB_OK);
            break;
        }

        ExtendedFilterClass_ptr = (HWND*)((int)exeditfp->dll_hinst + 0x1539c8);
        ll_ptr = (HWND*)((int)exeditfp->dll_hinst + 0x14decc);
        rr_ptr = (HWND*)((int)exeditfp->dll_hinst + 0x14966c);

        fp->exfunc->add_menu_item(fp, const_cast<char*>("開始フレームに移動"), fp->hwnd, MENU_LL, NULL, NULL);
        fp->exfunc->add_menu_item(fp, const_cast<char*>("終了フレームに移動"), fp->hwnd, MENU_RR, NULL, NULL);
        break;
    case WM_FILTER_COMMAND:
        switch (wparam) {
        case MENU_LL:
            SendMessageA(*ExtendedFilterClass_ptr, WM_COMMAND, 1102, (LPARAM)*ll_ptr);
            break;
        case MENU_RR:
            SendMessageA(*ExtendedFilterClass_ptr, WM_COMMAND, 1103, (LPARAM)*rr_ptr);
            break;
        }
    }
    return FALSE;
}
