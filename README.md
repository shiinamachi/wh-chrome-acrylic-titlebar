# Chrome Acrylic Titlebar

Makes Google Chrome's tab strip and toolbar transparent, so the Windows 11
Acrylic backdrop shows through. Web page content is not changed.

## How it works

Chrome asks Windows for a Mica backdrop but paints its frame opaque on top of
it. The mod hooks a few functions in `chrome.dll`:

- `BrowserDesktopWindowTreeHostWin::ShouldWindowContentsBeTransparent` returns
  `true`, so Chrome composites its window with a transparent background.
- `BrowserFrameViewWin::OnPaint` skips painting the titlebar. In incognito
  windows it paints a dark translucent tint instead (optional), so they stay
  recognizable and the wallpaper is calmer behind them.
- `BrowserTabStripController::InitFromModel` marks each tab strip as a glass
  frame with `TabStrip::SetIsGlassFrame(true)`, so inactive tabs don't fill
  their background. On macOS Chrome does this itself; on Windows nothing does.
- `views::View::OnPaintBackground` clears the toolbar and bookmarks bar
  backgrounds (optional), and paints the incognito tint there again. With **Translucent controls** it also skips the
  backgrounds of the address field's text and site info icon, so they sit on
  the field's own background, and hides the toolbar divider.
- `ui::ColorProvider::GetColor` makes the address field translucent, clears
  the tab search and new tab button backgrounds (hover feedback stays), and in
  a dark theme turns the active tab and the incognito chip into a light
  translucent highlight. The
  color ids are looked up by name in Chrome's own id table
  (`ChromeColorIdName`), so they follow Chrome updates.
- `HorizontalTabStyleViews::GetPath` gives the active tab the rounded hover
  shape instead of the file-folder shape that joins the toolbar.
- While `HorizontalTabStyleViews::PaintTab` runs, `gfx::Canvas::DrawRoundRect`
  is skipped: in tab painting it only draws the separators between tabs.

The backdrop Chrome requests (`DWMSBT_MAINWINDOW`) is replaced with the one
chosen in the settings, Acrylic by default.

## First run and Chrome updates

The mod needs the symbols (PDB) of the installed `chrome.dll`. Windhawk
downloads them from Google's symbol server. For Chrome 158 the download is
about 1.1 GB compressed and about 5.8 GB on disk.

1. Enable the mod and start Chrome. While the symbols download, Chrome starts
   normally without the effect.
2. Wait a few minutes until the download finishes. With logging enabled, the
   mod logs `Symbols are ready`.
3. Fully exit Chrome, including any background instance, and start it again.

Windhawk caches the function addresses, so later starts apply the effect
immediately. A Chrome update replaces `chrome.dll`, so the steps repeat after
each update. Without a cache hit, Chrome waits up to 4 seconds at startup.
Downloaded PDBs stay in `C:\ProgramData\Windhawk\Engine\Symbols`; old versions
can be deleted.

## Notes

- Tested with Chrome 158.0.8127.37 (x64). If a hooked function is missing in
  another version, the mod leaves Chrome unchanged.
- Windows draws the Acrylic backdrop, so its tint follows the system light or
  dark theme, and it turns solid while the window is inactive.
- With **Translucent controls**, the focused address field stays opaque so it
  matches the suggestion list below it.
- **Hide duplicate Windows caption buttons** removes `WS_SYSMENU` from browser
  windows. This can change the Alt+Space menu and tools that inspect window
  styles.
- To undo everything, disable the mod and restart Chrome.

Hiding the duplicate caption buttons and the asynchronous symbol lookup are
adapted from [Brave Acrylic](https://windhawk.net/mods/brave-acrylic) by
bardelyne (MIT).
