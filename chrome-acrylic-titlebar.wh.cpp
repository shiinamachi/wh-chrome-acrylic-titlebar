// ==WindhawkMod==
// @id              chrome-acrylic-titlebar
// @name            Chrome Acrylic Titlebar (prototype)
// @description     Show the Windows Acrylic backdrop behind Chrome's tab strip
// @version         0.2.0
// @author          sakuranight-win11
// @license         MIT
// @architecture    x86-64
// @include         chrome.exe
// @compilerOptions -ldwmapi -lshell32
// ==/WindhawkMod==

// ==WindhawkModReadme==
/*
# Chrome Acrylic Titlebar (시제품)

Chrome 탭 줄(제목 표시줄) 뒤를 비워서 Windows가 그리는 Acrylic 배경이 보이게 합니다.
웹 페이지 영역은 건드리지 않습니다.

## 동작 방식

Chrome 158은 창에 Mica 배경을 지정하지만 탭 줄을 프레임 색으로 불투명하게 칠해서 가립니다.
이 모드는 Chrome 내부 함수 두 개를 후킹합니다.

- `BrowserDesktopWindowTreeHostWin::ShouldWindowContentsBeTransparent`: `true`를 돌려줘서
  Chrome이 투명 배경으로 화면을 출력하게 합니다.
- `BrowserFrameViewWin::OnPaint`: 제목 표시줄 칠하기를 건너뜁니다.

비활성 탭은 각자 배경을 칠하므로, `Tab::OnThemeChanged`가 테마 속성
`SHOULD_FILL_BACKGROUND_TAB_COLOR`를 읽을 때 0을 돌려줘서 유리 프레임처럼 칠하지 않게 합니다.

그리고 Chrome이 지정하는 Mica(`DWMSBT_MAINWINDOW`)를 Acrylic(`DWMSBT_TRANSIENTWINDOW`)으로 바꿉니다.

## 처음 실행할 때와 Chrome을 업데이트한 뒤

내부 함수 위치를 찾으려면 지금 설치된 `chrome.dll`의 PDB가 필요합니다. Windhawk가 Google 심볼 서버에서
받습니다. Chrome 158 기준으로 압축 약 1.1 GB, 풀면 약 5.8 GB입니다.

1. 모드를 켜고 Chrome을 실행합니다. 심볼을 받는 동안 Chrome은 효과 없이 평소처럼 뜹니다.
2. 다운로드가 끝날 때까지 몇 분 기다립니다. 모드 로그에 `Symbols are ready`가 찍힙니다.
3. Chrome을 완전히 종료(백그라운드 실행 포함)하고 다시 실행합니다.

Windhawk는 찾은 함수 위치를 캐시하므로, 그다음부터는 바로 적용됩니다. Chrome이 업데이트되면
`chrome.dll`이 바뀌므로 위 과정을 반복합니다. 캐시가 없을 때는 Chrome 시작이 최대 4초 늦어집니다.
받은 PDB는 `C:\ProgramData\Windhawk\Engine\Symbols`에 버전마다 쌓이니 지난 버전은 지워도 됩니다.

## 한계

- 시제품입니다. Chrome 158.0.8127.37에서 함수 이름이 정확한지, 다른 함수와 합쳐지지 않았는지를
  PDB로 확인했습니다. 다른 버전에서 함수가 없어지면 모드는 아무것도 바꾸지 않습니다.
- Acrylic은 Windows가 그리므로 색은 시스템 다크·라이트 테마를 따르고, 비활성 창에서는 단색이 됩니다.
- **주소창 줄도 투명하게**를 켜도 활성 탭은 원래 색으로 그려집니다.
- **겹치는 Windows 창 단추 숨기기**는 브라우저 창에서만 `WS_SYSMENU`를 빼므로 Alt+Space 메뉴 같은
  시스템 메뉴 동작이 달라질 수 있습니다. 창 스타일을 보는 도구(komorebi 등)의 동작도 확인해야 합니다.
- 문제가 생기면 모드를 끄고 Chrome을 다시 시작하면 원래대로 돌아갑니다.

창 단추 숨기기와 심볼을 비동기로 찾는 방식은
[Brave Acrylic](https://github.com/bardelyne/brave-acrylic) (MIT, bardelyne)을 참고했습니다.
*/
// ==/WindhawkModReadme==

// ==WindhawkModSettings==
/*
- backdrop: acrylic
  $name: 배경 효과
  $description: 탭 줄 뒤에 보일 Windows 배경 효과입니다. 새로 여는 창부터 적용됩니다.
  $options:
  - acrylic: Acrylic
  - tabbed: Mica Alt
  - mica: Mica
- inactiveTabs: true
  $name: 비활성 탭 배경 지우기
  $description: 비활성 탭이 자기 배경을 칠하지 않게 해서 탭 뒤로도 배경 효과가 보이게 합니다. 활성 탭, 선택한 탭, 마우스를 올린 탭은 그대로 칠합니다. Chrome을 다시 시작해야 적용됩니다.
- toolbar: false
  $name: 주소창 줄도 투명하게
  $description: 주소창 줄과 북마크 바의 배경도 지웁니다. 활성 탭은 원래 색으로 남습니다.
- hideNativeCaptionButtons: true
  $name: 겹치는 Windows 창 단추 숨기기
  $description: 투명해진 탭 줄 아래에 Windows가 그리는 최소화·최대화·닫기 단추를 숨깁니다. WS_SYSMENU를 빼므로 Alt+Space 같은 시스템 메뉴 동작이 달라질 수 있습니다.
- downloadSymbols: true
  $name: Google 심볼 서버에서 심볼 받기
  $description: 캐시에 없을 때 chrome.dll의 PDB를 Google 심볼 서버에서 받습니다(압축 약 1.1 GB, 풀면 약 5.8 GB). 끄면 이미 받아 둔 심볼만 씁니다. Chrome을 다시 시작해야 적용됩니다.
*/
// ==/WindhawkModSettings==

#include <windows.h>
#include <dwmapi.h>
#include <shellapi.h>

#include <atomic>
#include <cstring>
#include <cwchar>
#include <string_view>

#include <windhawk_utils.h>

namespace {

constexpr DWORD kBackdropTypeAttribute = 38;  // DWMWA_SYSTEMBACKDROP_TYPE
constexpr UINT kBackdropMica = 2;             // DWMSBT_MAINWINDOW
constexpr UINT kBackdropAcrylic = 3;          // DWMSBT_TRANSIENTWINDOW
constexpr UINT kBackdropMicaAlt = 4;          // DWMSBT_TABBEDWINDOW

// Chrome continues without the effect if symbols take longer than this.
constexpr DWORD kSymbolWaitMs = 4000;
constexpr wchar_t kChromeSymbolServer[] =
    L"https://chromium-browser-symsrv.commondatastorage.googleapis.com";

// Decorated names, checked against the Chrome 158.0.8127.37 PDB. None of them
// shares its address with another function (no identical-code folding).
enum Slot : unsigned {
    kTransparent,
    kFrameOnPaint,
    kOnPaintBackground,
    kGetClassName,
    kDrawColor,
    kTabOnThemeChanged,
    kGetDisplayProperty,
    kSlotCount,
};
constexpr std::wstring_view kSymbols[kSlotCount] = {
    // private: virtual bool BrowserDesktopWindowTreeHostWin::ShouldWindowContentsBeTransparent() const
    L"?ShouldWindowContentsBeTransparent@BrowserDesktopWindowTreeHostWin@@EEBA_NXZ",
    // protected: virtual void BrowserFrameViewWin::OnPaint(gfx::Canvas*)
    L"?OnPaint@BrowserFrameViewWin@@MEAAXPEAVCanvas@gfx@@@Z",
    // protected: virtual void views::View::OnPaintBackground(gfx::Canvas*)
    L"?OnPaintBackground@View@views@@MEAAXPEAVCanvas@gfx@@@Z",
    // public: std::string_view views::View::GetClassName() const (windows.h renames it)
    L"?GetClassNameW@View@views@@QEBA?AV?$basic_string_view@DU?$char_traits@D@__Cr@std@@@__Cr@std@@XZ",
    // public: void gfx::Canvas::DrawColor(SkColor, SkBlendMode)
    L"?DrawColor@Canvas@gfx@@QEAAXIW4SkBlendMode@@@Z",
    // public: virtual void Tab::OnThemeChanged()
    L"?OnThemeChanged@Tab@@UEAAXXZ",
    // public: virtual int ThemeService::BrowserThemeProvider::GetDisplayProperty(int) const
    L"?GetDisplayProperty@BrowserThemeProvider@ThemeService@@UEBAHH@Z",
};

// ThemeProperties::SHOULD_FILL_BACKGROUND_TAB_COLOR in Chrome 158. Tab reads it
// once in OnThemeChanged; when it is 0, inactive tabs skip their background
// fill (as on a glass frame) while active, selected and hovered tabs still paint.
constexpr int kShouldFillBackgroundTabColor = 1032;

// Native x64 ABI shapes. A member function returning std::string_view writes
// it through a hidden pointer passed after `this`.
struct NativeClassName {
    const char* data;
    size_t size;
};
using Transparent_t = bool (*)(const void* host);
using Paint_t = void (*)(void* view, void* canvas);
using GetClassName_t = NativeClassName* (*)(const void* view, NativeClassName* out);
using DrawColor_t = void (*)(void* canvas, unsigned int color, int mode);
using Event_t = void (*)(void* self);
using DisplayProperty_t = int (*)(const void* provider, int id);
constexpr int kSkBlendModeClear = 0;

// Settings.
std::atomic<UINT> g_backdrop{kBackdropAcrylic};
std::atomic<bool> g_inactiveTabs{true};
std::atomic<bool> g_toolbar{false};
std::atomic<bool> g_hideCaptions{true};
std::atomic<bool> g_downloadSymbols{true};

// Set once the chrome.dll hooks are applied, before any browser window exists.
std::atomic<bool> g_active{false};
std::atomic<bool> g_toolbarAvailable{false};
std::atomic<bool> g_chromeAttempted{false};
bool g_isBrowserProcess = false;

Transparent_t g_origTransparent = nullptr;
Paint_t g_origFrameOnPaint = nullptr;
Paint_t g_origOnPaintBackground = nullptr;
GetClassName_t g_getClassName = nullptr;
DrawColor_t g_drawColor = nullptr;
Event_t g_origTabOnThemeChanged = nullptr;
DisplayProperty_t g_origGetDisplayProperty = nullptr;
thread_local unsigned t_tabThemeScope = 0;

decltype(&DwmSetWindowAttribute) g_origDwmSetWindowAttribute = nullptr;
decltype(&LoadLibraryExW) g_origLoadLibraryExW = nullptr;

// ---------------------------------------------------------------------------
// Settings

void LoadSettings() {
    PCWSTR backdrop = Wh_GetStringSetting(L"backdrop");
    UINT type = kBackdropAcrylic;
    if (backdrop && !wcscmp(backdrop, L"mica")) {
        type = kBackdropMica;
    } else if (backdrop && !wcscmp(backdrop, L"tabbed")) {
        type = kBackdropMicaAlt;
    }
    Wh_FreeStringSetting(backdrop);
    g_backdrop.store(type);
    g_inactiveTabs.store(Wh_GetIntSetting(L"inactiveTabs") != 0);
    g_toolbar.store(Wh_GetIntSetting(L"toolbar") != 0);
    g_hideCaptions.store(Wh_GetIntSetting(L"hideNativeCaptionButtons") != 0);
    g_downloadSymbols.store(Wh_GetIntSetting(L"downloadSymbols") != 0);
}

// ---------------------------------------------------------------------------
// Browser window identification
//
// Only browser windows (BrowserDesktopWindowTreeHostWin) ask the hooked
// ShouldWindowContentsBeTransparent while they are created, right after their
// HWND was created on the same thread. That HWND gets a property, and only
// marked windows get the backdrop and caption-button changes. Dialogs, task
// manager and other Chrome windows keep their native frame untouched.

constexpr wchar_t kBrowserFrameProperty[] = L"Windhawk.ChromeAcrylicTitlebar.Frame";
constexpr ULONGLONG kCandidateMaxAgeMs = 2000;
thread_local HWND t_candidate = nullptr;
thread_local ULONGLONG t_candidateTick = 0;

LONG_PTR RealStyle(HWND window);
void ApplyNativeCaption(HWND window);

bool IsBrowserFrameWindow(HWND window) {
    return window && GetPropW(window, kBrowserFrameProperty) != nullptr;
}

bool IsFrameCandidate(HWND window, DWORD style) {
    constexpr DWORD kRequired =
        WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
    if ((style & kRequired) != kRequired || (style & WS_CHILD) ||
        GetAncestor(window, GA_ROOT) != window || GetWindow(window, GW_OWNER)) {
        return false;
    }
    wchar_t name[32]{};
    return GetClassNameW(window, name, ARRAYSIZE(name)) &&
           !wcscmp(name, L"Chrome_WidgetWin_1");
}

void MarkBrowserFrame(HWND window) {
    if (!SetPropW(window, kBrowserFrameProperty, reinterpret_cast<HANDLE>(1))) {
        return;
    }
    // Chrome already asked for Mica while creating the window.
    const UINT backdrop = g_backdrop.load();
    g_origDwmSetWindowAttribute(window, kBackdropTypeAttribute, &backdrop,
                                sizeof(backdrop));
    ApplyNativeCaption(window);
}

// ---------------------------------------------------------------------------
// chrome.dll hooks

bool Transparent_Hook(const void* host) {
    if (!g_active.load(std::memory_order_acquire)) {
        return g_origTransparent(host);
    }
    const HWND candidate = t_candidate;
    if (candidate) {
        t_candidate = nullptr;
        if (GetTickCount64() - t_candidateTick < kCandidateMaxAgeMs &&
            IsWindow(candidate)) {
            MarkBrowserFrame(candidate);
        }
    }
    return true;
}

void FrameOnPaint_Hook(void* view, void* canvas) {
    // Leave the titlebar unpainted so the DWM backdrop shows through. Child
    // views (tabs, caption buttons, toolbar) paint afterwards as usual.
    if (g_active.load(std::memory_order_acquire)) {
        return;
    }
    g_origFrameOnPaint(view, canvas);
}

bool IsToolbarRow(void* view) {
    NativeClassName name{};
    g_getClassName(view, &name);
    if (!name.data || name.size == 0 || name.size > 64) {
        return false;
    }
    const std::string_view cls(name.data, name.size);
    return cls == "ToolbarView" || cls == "BookmarkBarView";
}

void OnPaintBackground_Hook(void* view, void* canvas) {
    if (view && canvas && g_toolbar.load(std::memory_order_relaxed) &&
        g_toolbarAvailable.load(std::memory_order_acquire) &&
        g_active.load(std::memory_order_acquire) && IsToolbarRow(view)) {
        // The canvas is clipped to this view; clear it to reveal the backdrop.
        g_drawColor(canvas, 0, kSkBlendModeClear);
        return;
    }
    g_origOnPaintBackground(view, canvas);
}

void TabOnThemeChanged_Hook(void* tab) {
    ++t_tabThemeScope;
    g_origTabOnThemeChanged(tab);
    --t_tabThemeScope;
}

int GetDisplayProperty_Hook(const void* provider, int id) {
    if (t_tabThemeScope && id == kShouldFillBackgroundTabColor &&
        g_inactiveTabs.load(std::memory_order_relaxed) &&
        g_active.load(std::memory_order_acquire)) {
        return 0;
    }
    return g_origGetDisplayProperty(provider, id);
}

// ---------------------------------------------------------------------------
// Backdrop type

HRESULT WINAPI DwmSetWindowAttribute_Hook(HWND window,
                                          DWORD attribute,
                                          LPCVOID value,
                                          DWORD size) {
    if (attribute == kBackdropTypeAttribute && value && size == sizeof(UINT) &&
        *static_cast<const UINT*>(value) == kBackdropMica &&
        g_active.load(std::memory_order_acquire) && IsBrowserFrameWindow(window)) {
        const UINT backdrop = g_backdrop.load();
        return g_origDwmSetWindowAttribute(window, attribute, &backdrop,
                                           sizeof(backdrop));
    }
    return g_origDwmSetWindowAttribute(window, attribute, value, size);
}

// ---------------------------------------------------------------------------
// Native caption buttons
//
// Windows still draws its caption buttons underneath Chromium's transparent
// custom frame. Hide only WS_SYSMENU and keep reporting it to Chromium, so its
// own logic (including saving the style for fullscreen) sees the original.
// Adapted from Brave Acrylic (MIT, bardelyne).

constexpr wchar_t kCaptionProperty[] = L"Windhawk.ChromeAcrylicTitlebar.NativeCaption";
std::atomic<bool> g_captionFixActive{false};
std::atomic<bool> g_captionStopping{false};
std::atomic<bool> g_captionPropertyUsed{false};
bool g_captionHooksInstalled = false;
decltype(&CreateWindowExW) g_origCreateWindowExW = nullptr;
decltype(&GetWindowLongPtrW) g_origGetWindowLongPtrW = nullptr;
decltype(&SetWindowLongPtrW) g_origSetWindowLongPtrW = nullptr;
decltype(&GetWindowLongW) g_origGetWindowLongW = nullptr;
decltype(&SetWindowLongW) g_origSetWindowLongW = nullptr;
thread_local HWND t_captionStyleWrite = nullptr;

LONG_PTR RealStyle(HWND window) {
    return g_origGetWindowLongPtrW ? g_origGetWindowLongPtrW(window, GWL_STYLE)
                                   : GetWindowLongPtrW(window, GWL_STYLE);
}

void RefreshFrame(HWND window) {
    SetWindowPos(window, nullptr, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE |
                     SWP_FRAMECHANGED);
}

void RestoreNativeCaption(HWND window) {
    if (!GetPropW(window, kCaptionProperty)) {
        return;
    }
    RemovePropW(window, kCaptionProperty);
    // The property is gone, so the hooks pass this write through unchanged.
    const LONG_PTR style = GetWindowLongPtrW(window, GWL_STYLE);
    SetWindowLongPtrW(window, GWL_STYLE, style | WS_SYSMENU);
    RefreshFrame(window);
}

template <typename Value>
Value ReadCaptionStyle(HWND window, int index, Value(WINAPI* original)(HWND, int)) {
    if (index != GWL_STYLE ||
        (!g_captionFixActive.load(std::memory_order_acquire) &&
         !g_captionPropertyUsed.load(std::memory_order_relaxed))) {
        return original(window, index);
    }
    Value value = original(window, index);
    const DWORD error = GetLastError();
    if (GetPropW(window, kCaptionProperty)) {
        value |= WS_SYSMENU;
    }
    SetLastError(error);
    return value;
}

LONG_PTR WINAPI GetWindowLongPtrW_Hook(HWND window, int index) {
    return ReadCaptionStyle(window, index, g_origGetWindowLongPtrW);
}

LONG WINAPI GetWindowLongW_Hook(HWND window, int index) {
    return ReadCaptionStyle(window, index, g_origGetWindowLongW);
}

template <typename Value>
Value WriteCaptionStyle(HWND window,
                        int index,
                        Value value,
                        Value(WINAPI* original)(HWND, int, Value)) {
    // Windows may forward one API to the other; translate the style only once.
    if (index != GWL_STYLE || t_captionStyleWrite == window ||
        (!g_captionFixActive.load(std::memory_order_acquire) &&
         !g_captionPropertyUsed.load(std::memory_order_relaxed))) {
        return original(window, index, value);
    }
    const bool owned = GetPropW(window, kCaptionProperty) != nullptr;
    const bool hide = g_captionFixActive.load(std::memory_order_acquire) &&
                      (value & WS_SYSMENU) && (owned || IsBrowserFrameWindow(window));
    bool marked = owned;
    if (hide && !owned) {
        g_captionPropertyUsed.store(true, std::memory_order_relaxed);
        marked = SetPropW(window, kCaptionProperty, reinterpret_cast<HANDLE>(1)) != FALSE;
    } else if (!hide && owned) {
        RemovePropW(window, kCaptionProperty);
        marked = false;
    }
    const HWND previousWrite = t_captionStyleWrite;
    t_captionStyleWrite = window;
    // Zero is a valid previous style; clear stale errors first.
    SetLastError(ERROR_SUCCESS);
    const Value previous = original(
        window, index, hide && marked ? value & ~static_cast<Value>(WS_SYSMENU) : value);
    const DWORD error = GetLastError();
    t_captionStyleWrite = previousWrite;
    if (!previous && error) {
        if (owned) {
            SetPropW(window, kCaptionProperty, reinterpret_cast<HANDLE>(1));
        } else {
            RemovePropW(window, kCaptionProperty);
        }
    } else if (hide && marked && !g_captionFixActive.load(std::memory_order_acquire)) {
        // The setting changed while SetWindowLong dispatched style messages.
        RestoreNativeCaption(window);
    }
    SetLastError(error);
    return previous && owned ? previous | WS_SYSMENU : previous;
}

LONG_PTR WINAPI SetWindowLongPtrW_Hook(HWND window, int index, LONG_PTR value) {
    return WriteCaptionStyle(window, index, value, g_origSetWindowLongPtrW);
}

LONG WINAPI SetWindowLongW_Hook(HWND window, int index, LONG value) {
    return WriteCaptionStyle(window, index, value, g_origSetWindowLongW);
}

void ApplyNativeCaption(HWND window) {
    if (!g_captionFixActive.load(std::memory_order_acquire) ||
        GetPropW(window, kCaptionProperty)) {
        return;
    }
    const LONG_PTR style = RealStyle(window);
    if (!(style & WS_SYSMENU) || !IsBrowserFrameWindow(window)) {
        return;
    }
    SetWindowLongPtrW_Hook(window, GWL_STYLE, style);
    if (GetPropW(window, kCaptionProperty)) {
        RefreshFrame(window);
    }
}

HWND WINAPI CreateWindowExW_Hook(DWORD exStyle,
                                 LPCWSTR className,
                                 LPCWSTR windowName,
                                 DWORD style,
                                 int x,
                                 int y,
                                 int width,
                                 int height,
                                 HWND parent,
                                 HMENU menu,
                                 HINSTANCE instance,
                                 LPVOID param) {
    const HWND window = g_origCreateWindowExW(exStyle, className, windowName, style, x,
                                              y, width, height, parent, menu, instance,
                                              param);
    const DWORD error = GetLastError();
    if (window && g_active.load(std::memory_order_acquire) &&
        IsFrameCandidate(window, style)) {
        t_candidate = window;
        t_candidateTick = GetTickCount64();
    }
    SetLastError(error);
    return window;
}

BOOL CALLBACK SyncCaptionWindow(HWND window, LPARAM) {
    DWORD process = 0;
    GetWindowThreadProcessId(window, &process);
    if (process == GetCurrentProcessId()) {
        if (g_captionFixActive.load(std::memory_order_acquire)) {
            ApplyNativeCaption(window);
        } else {
            RestoreNativeCaption(window);
        }
    }
    return TRUE;
}

BOOL CALLBACK ForgetBrowserFrame(HWND window, LPARAM) {
    DWORD process = 0;
    GetWindowThreadProcessId(window, &process);
    if (process == GetCurrentProcessId()) {
        RemovePropW(window, kBrowserFrameProperty);
    }
    return TRUE;
}

void SyncCaptions() {
    const bool active = g_captionHooksInstalled &&
                        !g_captionStopping.load(std::memory_order_acquire) &&
                        g_active.load(std::memory_order_acquire) &&
                        g_hideCaptions.load(std::memory_order_relaxed);
    g_captionFixActive.store(active, std::memory_order_release);
    if (g_captionHooksInstalled) {
        EnumWindows(SyncCaptionWindow, 0);
    }
}

BOOL CALLBACK UpdateBackdropWindow(HWND window, LPARAM) {
    if (IsBrowserFrameWindow(window)) {
        const UINT backdrop = g_backdrop.load();
        g_origDwmSetWindowAttribute(window, kBackdropTypeAttribute, &backdrop,
                                    sizeof(backdrop));
    }
    return TRUE;
}

// ---------------------------------------------------------------------------
// Symbol lookup

SRWLOCK g_lock = SRWLOCK_INIT;
CONDITION_VARIABLE g_lookupChanged = CONDITION_VARIABLE_INIT;
HANDLE g_worker = nullptr;
bool g_stopping = false;
struct {
    HMODULE module;
    ULONGLONG deadline;
    bool finished;
    bool succeeded;
    void* address[kSlotCount];
} g_lookup{};

bool ResolveAddresses(HMODULE module, void** result) {
    void* address[kSlotCount]{};
    WindhawkUtils::SYMBOL_HOOK hooks[kSlotCount];
    for (unsigned i = 0; i < kSlotCount; ++i) {
        hooks[i] = {{kSymbols[i]}, &address[i], nullptr, true};
    }
    WH_HOOK_SYMBOLS_OPTIONS options{};
    options.optionsSize = sizeof(options);
    options.symbolServer = g_downloadSymbols.load() ? kChromeSymbolServer : L"";
    // The full Chrome PDB is huge; match decorated names only.
    options.noUndecoratedSymbols = TRUE;
    options.onlineCacheUrl = L"";
    if (!WindhawkUtils::HookSymbols(module, hooks, kSlotCount, &options)) {
        return false;
    }
    std::memcpy(result, address, sizeof(address));
    return address[kTransparent] && address[kFrameOnPaint];
}

DWORD WINAPI LookupWorker(void*) {
    void* address[kSlotCount]{};
    HMODULE pinned = nullptr;
    const HMODULE module = g_lookup.module;
    const bool succeeded =
        GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
                           reinterpret_cast<LPCWSTR>(module), &pinned) &&
        ResolveAddresses(module, address);
    if (pinned) {
        FreeLibrary(pinned);
    }

    AcquireSRWLockExclusive(&g_lock);
    std::memcpy(g_lookup.address, address, sizeof(address));
    g_lookup.succeeded = succeeded;
    g_lookup.finished = true;
    const bool late = GetTickCount64() >= g_lookup.deadline;
    WakeAllConditionVariable(&g_lookupChanged);
    ReleaseSRWLockExclusive(&g_lock);

    if (!succeeded) {
        Wh_Log(L"Symbol lookup failed; Chrome stays unmodified");
    } else if (late) {
        Wh_Log(L"Symbols are ready; fully exit and restart Chrome to apply");
    }
    return 0;
}

bool InstallChromeHooks() {
    void* const* address = g_lookup.address;
    if (!WindhawkUtils::SetFunctionHook(
            reinterpret_cast<Transparent_t>(address[kTransparent]), Transparent_Hook,
            &g_origTransparent) ||
        !WindhawkUtils::SetFunctionHook(
            reinterpret_cast<Paint_t>(address[kFrameOnPaint]), FrameOnPaint_Hook,
            &g_origFrameOnPaint)) {
        return false;
    }
    if (address[kOnPaintBackground] && address[kGetClassName] && address[kDrawColor]) {
        g_getClassName = reinterpret_cast<GetClassName_t>(address[kGetClassName]);
        g_drawColor = reinterpret_cast<DrawColor_t>(address[kDrawColor]);
        g_toolbarAvailable.store(WindhawkUtils::SetFunctionHook(
            reinterpret_cast<Paint_t>(address[kOnPaintBackground]),
            OnPaintBackground_Hook, &g_origOnPaintBackground));
    }
    if (address[kTabOnThemeChanged] && address[kGetDisplayProperty]) {
        if (!WindhawkUtils::SetFunctionHook(
                reinterpret_cast<Event_t>(address[kTabOnThemeChanged]),
                TabOnThemeChanged_Hook, &g_origTabOnThemeChanged) ||
            !WindhawkUtils::SetFunctionHook(
                reinterpret_cast<DisplayProperty_t>(address[kGetDisplayProperty]),
                GetDisplayProperty_Hook, &g_origGetDisplayProperty)) {
            Wh_Log(L"Inactive tab background hooks unavailable");
        }
    }
    return true;
}

// Resolve symbols and, if they arrive within the budget, hook chrome.dll.
// `install` is false when Chrome already has windows: those decided to be
// opaque at creation, so the lookup only fills Windhawk's cache.
void AttemptChrome(HMODULE module, bool install) {
    if (!module || g_chromeAttempted.exchange(true)) {
        return;
    }
    AcquireSRWLockExclusive(&g_lock);
    if (g_stopping) {
        ReleaseSRWLockExclusive(&g_lock);
        return;
    }
    g_lookup.module = module;
    g_lookup.deadline = GetTickCount64() + (install ? kSymbolWaitMs : 0);
    g_worker = CreateThread(nullptr, 0, LookupWorker, nullptr, 0, nullptr);
    if (!g_worker) {
        ReleaseSRWLockExclusive(&g_lock);
        Wh_Log(L"Could not start the symbol lookup");
        return;
    }
    if (!install) {
        ReleaseSRWLockExclusive(&g_lock);
        Wh_Log(L"Chrome was already running; restart Chrome to apply");
        return;
    }
    while (!g_lookup.finished && !g_stopping) {
        const ULONGLONG now = GetTickCount64();
        if (now >= g_lookup.deadline ||
            !SleepConditionVariableSRW(&g_lookupChanged, &g_lock,
                                       static_cast<DWORD>(g_lookup.deadline - now), 0)) {
            break;
        }
    }
    bool active = false;
    if (!g_stopping && g_lookup.finished && g_lookup.succeeded) {
        active = InstallChromeHooks() && Wh_ApplyHookOperations();
        if (!active) {
            Wh_Log(L"Could not hook chrome.dll; Chrome stays unmodified");
        }
    } else if (!g_lookup.finished) {
        Wh_Log(L"Symbols are still loading; Chrome starts without the effect");
    }
    g_active.store(active, std::memory_order_release);
    ReleaseSRWLockExclusive(&g_lock);
    if (active) {
        Wh_Log(L"Transparent titlebar is active");
        SyncCaptions();
    }
}

bool IsExecutableLoad(DWORD flags) {
    return !(flags & (DONT_RESOLVE_DLL_REFERENCES | LOAD_LIBRARY_AS_DATAFILE |
                      LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE |
                      LOAD_LIBRARY_AS_IMAGE_RESOURCE));
}

HMODULE WINAPI LoadLibraryExW_Hook(LPCWSTR name, HANDLE file, DWORD flags) {
    const HMODULE module = g_origLoadLibraryExW(name, file, flags);
    const DWORD error = GetLastError();
    if (module && !g_chromeAttempted.load(std::memory_order_acquire) &&
        IsExecutableLoad(flags) && module == GetModuleHandleW(L"chrome.dll")) {
        // Chrome cannot use chrome.dll until this returns, so no window exists yet.
        AttemptChrome(module, true);
    }
    SetLastError(error);
    return module;
}

BOOL CALLBACK FindChromeWindow(HWND window, LPARAM found) {
    DWORD process = 0;
    GetWindowThreadProcessId(window, &process);
    wchar_t name[32]{};
    if (process == GetCurrentProcessId() &&
        GetClassNameW(window, name, ARRAYSIZE(name)) &&
        !wcscmp(name, L"Chrome_WidgetWin_1")) {
        *reinterpret_cast<bool*>(found) = true;
        return FALSE;
    }
    return TRUE;
}

bool IsBrowserProcess() {
    int count = 0;
    LPWSTR* args = CommandLineToArgvW(GetCommandLineW(), &count);
    if (!args) {
        return false;
    }
    bool browser = true;
    for (int i = 1; i < count; ++i) {
        if (!wcscmp(args[i], L"--") || !wcscmp(args[i], L"--single-argument")) {
            break;
        }
        if (!wcsncmp(args[i], L"--type=", 7)) {
            browser = false;
            break;
        }
    }
    LocalFree(args);
    return browser;
}

}  // namespace

BOOL Wh_ModInit() {
    if (!IsBrowserProcess()) {
        return FALSE;
    }
    g_isBrowserProcess = true;
    LoadSettings();

    if (!WindhawkUtils::SetFunctionHook(DwmSetWindowAttribute,
                                        DwmSetWindowAttribute_Hook,
                                        &g_origDwmSetWindowAttribute)) {
        Wh_Log(L"Could not hook DwmSetWindowAttribute");
        return FALSE;
    }
    g_captionHooksInstalled =
        WindhawkUtils::SetFunctionHook(CreateWindowExW, CreateWindowExW_Hook,
                                       &g_origCreateWindowExW) &&
        WindhawkUtils::SetFunctionHook(GetWindowLongPtrW, GetWindowLongPtrW_Hook,
                                       &g_origGetWindowLongPtrW) &&
        WindhawkUtils::SetFunctionHook(SetWindowLongPtrW, SetWindowLongPtrW_Hook,
                                       &g_origSetWindowLongPtrW) &&
        WindhawkUtils::SetFunctionHook(GetWindowLongW, GetWindowLongW_Hook,
                                       &g_origGetWindowLongW) &&
        WindhawkUtils::SetFunctionHook(SetWindowLongW, SetWindowLongW_Hook,
                                       &g_origSetWindowLongW);
    if (!g_captionHooksInstalled) {
        Wh_Log(L"Caption button fix unavailable");
    }

    if (GetModuleHandleW(L"chrome.dll")) {
        // Handled in Wh_ModAfterInit, once the hooks above are active.
        return TRUE;
    }
    const HMODULE kernelBase = GetModuleHandleW(L"kernelbase.dll");
    const auto loader =
        kernelBase ? reinterpret_cast<decltype(&LoadLibraryExW)>(
                         GetProcAddress(kernelBase, "LoadLibraryExW"))
                   : nullptr;
    if (!loader ||
        !WindhawkUtils::SetFunctionHook(loader, LoadLibraryExW_Hook,
                                        &g_origLoadLibraryExW)) {
        Wh_Log(L"Could not hook LoadLibraryExW");
        return FALSE;
    }
    return TRUE;
}

void Wh_ModAfterInit() {
    if (!g_isBrowserProcess) {
        return;
    }
    // chrome.dll was loaded before the loader hook became active.
    if (const HMODULE chrome = GetModuleHandleW(L"chrome.dll")) {
        bool hasWindows = false;
        EnumWindows(FindChromeWindow, reinterpret_cast<LPARAM>(&hasWindows));
        AttemptChrome(chrome, !hasWindows);
    }
}

void Wh_ModSettingsChanged() {
    if (!g_isBrowserProcess) {
        return;
    }
    LoadSettings();
    SyncCaptions();
    if (g_active.load(std::memory_order_acquire)) {
        EnumWindows(UpdateBackdropWindow, 0);
    }
}

void Wh_ModBeforeUninit() {
    g_captionStopping.store(true, std::memory_order_release);
    g_captionFixActive.store(false, std::memory_order_release);
    AcquireSRWLockExclusive(&g_lock);
    g_stopping = true;
    g_active.store(false, std::memory_order_release);
    const HANDLE worker = g_worker;
    g_worker = nullptr;
    WakeAllConditionVariable(&g_lookupChanged);
    ReleaseSRWLockExclusive(&g_lock);
    // Give WS_SYSMENU back while the style hooks are still installed.
    if (g_isBrowserProcess && g_captionHooksInstalled) {
        EnumWindows(SyncCaptionWindow, 0);
    }
    if (g_isBrowserProcess) {
        EnumWindows(ForgetBrowserFrame, 0);
    }
    // Windhawk's symbol lookup cannot be cancelled; it must finish before the
    // mod DLL unloads.
    if (worker) {
        WaitForSingleObject(worker, INFINITE);
        CloseHandle(worker);
    }
}
