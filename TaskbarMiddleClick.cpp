#define UNICODE
#define _UNICODE
#include <windows.h>
#include <string>
#include <algorithm>
#include <cwctype>

static HHOOK g_hook = nullptr;

static bool contains(const std::wstring& s, const wchar_t* part) {
    std::wstring a = s, b = part;
    std::transform(a.begin(), a.end(), a.begin(), towlower);
    std::transform(b.begin(), b.end(), b.begin(), towlower);
    return a.find(b) != std::wstring::npos;
}

static std::wstring className(HWND h) {
    wchar_t buf[256] = {};
    GetClassNameW(h, buf, 255);
    return buf;
}

static bool isTaskbarWindowAt(POINT pt, HWND& root) {
    HWND bars[2] = {
        FindWindowW(L"Shell_TrayWnd", nullptr),
        FindWindowW(L"Shell_SecondaryTrayWnd", nullptr)
    };
    for (HWND bar : bars) {
        if (!bar) continue;
        RECT r{};
        if (GetWindowRect(bar, &r) &&
            pt.x >= r.left && pt.x < r.right &&
            pt.y >= r.top && pt.y < r.bottom) {
            root = bar;
            return true;
        }
    }
    return false;
}

static bool isLikelyBlankTaskbarPoint(POINT pt, HWND root) {
    HWND h = WindowFromPoint(pt);
    if (!h) return true;
    if (h == root) return true;

    HWND directChild = h;
    HWND parent = GetParent(directChild);
    while (parent && parent != root) {
        directChild = parent;
        parent = GetParent(directChild);
    }
    if (parent != root) return false;

    std::wstring cls = className(directChild);
    std::wstring hitCls = className(h);
    const wchar_t* interactive[] = {
        L"MSTaskListWClass", L"TrayNotifyWnd", L"Start", L"Search",
        L"Button", L"Clock", L"Input", L"TaskListThumbnailWnd",
        L"Xaml", L"Windows.UI.Core", L"ReBarWindow32", L"ToolbarWindow32"
    };
    for (const wchar_t* token : interactive) {
        if (contains(cls, token) || contains(hitCls, token)) return false;
    }
    return true;
}

static void showDesktop() {
    INPUT in[4]{};
    in[0].type = INPUT_KEYBOARD; in[0].ki.wVk = VK_LWIN;
    in[1].type = INPUT_KEYBOARD; in[1].ki.wVk = 'D';
    in[2].type = INPUT_KEYBOARD; in[2].ki.wVk = 'D'; in[2].ki.dwFlags = KEYEVENTF_KEYUP;
    in[3].type = INPUT_KEYBOARD; in[3].ki.wVk = VK_LWIN; in[3].ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(4, in, sizeof(INPUT));
}

static LRESULT CALLBACK mouseProc(int code, WPARAM wp, LPARAM lp) {
    if (code == HC_ACTION && wp == WM_MBUTTONDOWN) {
        const MSLLHOOKSTRUCT* data = reinterpret_cast<const MSLLHOOKSTRUCT*>(lp);
        HWND taskbar = nullptr;
        if (isTaskbarWindowAt(data->pt, taskbar) &&
            isLikelyBlankTaskbarPoint(data->pt, taskbar)) {
            showDesktop();
        }
    }
    return CallNextHookEx(g_hook, code, wp, lp);
}

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int) {
    g_hook = SetWindowsHookExW(WH_MOUSE_LL, mouseProc, GetModuleHandleW(nullptr), 0);
    if (!g_hook) {
        MessageBoxW(nullptr, L"Não foi possível instalar o monitor do mouse.",
                    L"TaskbarMiddleClick", MB_OK | MB_ICONERROR);
        return 1;
    }

    MSG msg{};
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    UnhookWindowsHookEx(g_hook);
    return 0;
}
