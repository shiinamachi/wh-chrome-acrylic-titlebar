# AGENTS.md

이 문서는 이 저장소에서 작업하는 AI 에이전트(및 사람 기여자)가 따라야 할 규칙을 정의합니다.

## 프로젝트

- 이름: `wh-chrome-acrylic-titlebar`
- 내용: Google Chrome의 탭 줄과 주소창 줄 뒤에 Windows 11 Acrylic 배경이 보이게 하는 Windhawk 모드 (`chrome-acrylic-titlebar`)
- 라이선스: MIT (`LICENSE` 참고)

## 저장소 구성

- `chrome-acrylic-titlebar.wh.cpp`: 모드 원본. Windhawk 메타데이터, 안내문(`WindhawkModReadme`), 설정(`WindhawkModSettings`), 코드가 한 파일에 들어 있습니다.
- `README.md`: 모드 안내문과 같은 내용입니다.
- 이 저장소에 개발 이력을 모두 남기고, 배포는 `shiinamachi/windhawk-mods`에서 합니다(아래 "배포").

## 커밋 규칙

### 커밋 단위

- **작고 복구 가능한 범위의 작업이 하나 완료될 때마다 즉시 커밋합니다.**
  - 여러 관심사를 하나의 커밋에 섞지 않습니다.
  - 각 커밋은 그 자체로 의미가 있고, 단독으로 되돌릴(revert) 수 있어야 합니다.
  - 빌드/동작이 깨진 상태로 커밋하지 않습니다.
- 작업을 모두 끝낸 뒤 한꺼번에 커밋하지 않습니다.

### 커밋 메시지

커밋 메시지는 [Conventional Commits 1.0.0](https://www.conventionalcommits.org/en/v1.0.0/)을 준수합니다.

```
<type>[optional scope]: <description>

[optional body]

[optional footer(s)]
```

- `type`
  - `feat`: 새로운 기능
  - `fix`: 버그 수정
  - `docs`: 문서 변경
  - `style`: 동작에 영향 없는 포맷팅 등
  - `refactor`: 기능 변경 없는 코드 구조 개선
  - `perf`: 성능 개선
  - `test`: 테스트 추가/수정
  - `build`: 빌드 시스템, 의존성 변경
  - `ci`: CI 설정 변경
  - `chore`: 그 외 잡무 (설정 파일 등)
  - `revert`: 이전 커밋 되돌리기
- `description`은 명령형 현재 시제로 간결하게 작성하고, 끝에 마침표를 붙이지 않습니다.
- 하위 호환성을 깨는 변경은 `type!:` 형식을 사용하거나 footer에 `BREAKING CHANGE:`를 명시합니다.

예시:

```
feat(theme): add dark accent color palette
fix: correct window border color on high-DPI displays
docs: update installation guide
```

이 저장소의 초기 이력에는 `shiinamachi/windhawk-mods` 브랜치에서 옮겨 온 `Chrome Acrylic Titlebar: ...` 형식의 커밋이 섞여 있습니다. 새 커밋은 위 규칙을 따릅니다.

## README와 모드 안내문

- `README.md`와 `chrome-acrylic-titlebar.wh.cpp`의 `// ==WindhawkModReadme==` 블록(`/*` 다음 줄부터 `*/` 앞까지)은 **글자 하나까지 같아야 합니다.**
- 안내문을 고칠 때는 `README.md`를 고친 뒤 그 내용을 그대로 cpp 블록에 복사하고, 같은 커밋에 넣습니다.
- 확인:

  ```bash
  python3 -c "s=open('chrome-acrylic-titlebar.wh.cpp',encoding='utf-8').read();a=s.index('// ==WindhawkModReadme==\n/*\n')+28;b=s.index('*/\n// ==/WindhawkModReadme==');print('same' if s[a:b]==open('README.md',encoding='utf-8').read() else 'DIFFERENT')"
  ```

- 안내문, 설정 이름과 설명, 코드 주석은 영어로 씁니다. 상류(`ramensoftware/windhawk-mods`) 카탈로그의 관례입니다.
- README와 설정 설명은 **사용자용 문서**입니다. 이 모드가 무엇인지, 처음 실행할 때 무엇이 일어나는지, 각 설정이 화면에서 무엇을 바꾸는지를 씁니다. 후킹하는 함수, 색 ID, 심볼 이름 같은 구현 설명은 넣지 않고 아래 "구현 개요"에 둡니다.
- 설정을 추가하거나 동작을 바꾸면 README의 "Settings"와 설정 `$description`을 함께 고칩니다.

### 이미지

- README 이미지는 이 저장소의 `images/`에 두고, Markdown 이미지 문법으로 `https://raw.githubusercontent.com/shiinamachi/wh-chrome-acrylic-titlebar/refs/heads/main/images/<파일>` 주소를 씁니다. 상류는 `i.imgur.com`과 `raw.githubusercontent.com`의 공개 주소만 받고, 상대 경로나 `<img>` 태그는 오류입니다. 그래서 이 저장소는 공개로 둡니다.
- 상류는 이미지를 처음 본 때의 내용으로 보관하고 갱신하지 않습니다. 이미지를 바꿀 때는 기존 파일을 덮어쓰지 말고 **새 파일 이름**으로 추가한 뒤 README 주소를 바꿉니다.
- 이미지 하나는 2 MB 미만으로 유지합니다.
- 스크린샷에 개인 정보(프로필 사진, 작업 표시줄 앱, 열린 탭 제목 등)가 보이면 넣기 전에 사용자에게 확인합니다.

## 버전

- `@version`은 배포할 때만 올립니다. 개발 중 커밋마다 올리지 않습니다.
- 한 번 배포한 버전 번호는 다시 쓰지 않습니다. 상류는 업데이트마다 더 높은 버전을 요구합니다.

## 배포 (`shiinamachi/windhawk-mods`)

- `shiinamachi/windhawk-mods`는 `ramensoftware/windhawk-mods`의 fork이며, **배포할 버전마다 커밋 하나만** 올립니다. 개발 이력은 이 저장소에만 남깁니다.
- 배포 커밋은 fork의 브랜치에 `mods/chrome-acrylic-titlebar.wh.cpp` 파일 하나만 바꾸고, 메시지는 상류 관례대로 `Chrome Acrylic Titlebar v<버전>`으로 씁니다.
- **브랜치에 push까지만 합니다.** `ramensoftware/windhawk-mods`에 PR을 여는 것, 상류 저장소에 댓글이나 이슈를 남기는 것은 사용자가 명시적으로 요청할 때만 합니다. "제출"처럼 모호한 표현은 push까지로 해석하고 묻습니다. 공개 PR은 삭제할 수 없고 닫기만 할 수 있습니다.
- 상류 PR 규칙: 파일 하나(`mods/<mod-id>.wh.cpp`)만 담고, `@github`가 PR 작성자 프로필과 같아야 하며, PR 본문에 템플릿의 "Mod authorship" 섹션을 남겨야 합니다.
- 작성자 메타데이터:
  - `@author`는 표시 이름이라 `shiinamachi`를 씁니다.
  - `@github`는 PR을 여는 사용자 계정인 `https://github.com/kentakang`으로 둡니다. `shiinamachi`는 조직 계정이라 PR 작성자가 될 수 없어서, `@github`를 조직으로 바꾸면 상류 검사가 항상 실패합니다.
  - 상류에 한 번 등록되면 두 값 모두 바꿀 수 없으니 임의로 바꾸지 않습니다.
  - `@homepage`는 이 저장소(`https://github.com/shiinamachi/wh-chrome-acrylic-titlebar`)입니다.

### 릴리즈 요청을 받았을 때

사용자가 "릴리즈해 줘"처럼 배포를 지시하면, **그 요청을 받은 시점의 이 저장소 최신 코드**를 `shiinamachi/windhawk-mods`에 배포 커밋 하나로 올립니다. 배포는 이 저장소가 원본이고, fork에서 코드를 직접 고치지 않습니다.

1. **기준 커밋 정하기**
   - 원격 저장소를 fetch해서, 요청 시점의 `origin/main` 최신 커밋을 배포 기준으로 삼습니다.
   - 커밋하지 않은 변경이나 아직 push하지 않은 로컬 커밋이 있으면, 배포에 포함할지 사용자에게 묻습니다. 포함한다면 먼저 커밋·push해서 기준 커밋이 원격에 있게 합니다.
   - 기준 커밋의 SHA를 기록해 두고, 이후 단계는 모두 그 커밋의 파일로 진행합니다.

2. **버전 정하기**
   - fork의 배포 브랜치에 마지막으로 올린 버전(없으면 상류 카탈로그에 있는 버전)을 확인합니다.
   - 기준 커밋의 `@version`이 그보다 높지 않으면 새 버전이 필요합니다. 변경 내용에 맞는 버전(기능 추가는 minor, 수정만 있으면 patch)을 제안하고 사용자 확인을 받은 뒤, 이 저장소에 버전을 올리는 커밋(`chore(release): bump version to <버전>`)을 만들어 push합니다. 이 커밋이 새 기준 커밋이 됩니다.
   - 배포 커밋에 넣을 변경 목록을 직전 배포 이후의 이 저장소 커밋들로 정리합니다.

3. **검증하기**
   - 기준 커밋의 파일로 "검증"의 세 가지를 모두 통과해야 합니다. 상류 검사 스크립트는 요청 시점의 상류 `main`에서 새로 받아 씁니다. 실제 Chrome 확인을 아직 하지 않은 변경이 있으면 사용자에게 알리고 진행 여부를 묻습니다.
   - README와 안내문이 같은지도 확인합니다.

4. **fork에 배포 커밋 올리기**
   - fork의 `main`이 상류보다 뒤처져 있으면 먼저 동기화합니다. 상류에 워크플로 파일 변경이 있으면 `gh` 토큰에 `workflow` 권한이 필요하니, 사용자에게 GitHub의 "Sync fork" 버튼이나 `gh auth refresh -s workflow`를 요청합니다.
   - 배포 브랜치는 `chrome-acrylic-titlebar` 하나를 계속 씁니다. 없으면 최신 fork `main`에서 새로 만듭니다.
   - 그 브랜치에 `mods/chrome-acrylic-titlebar.wh.cpp`를 기준 커밋의 파일과 바이트 단위로 같게 넣고, 이 파일 하나만 바뀐 커밋 하나를 만듭니다.
     - 제목: `Chrome Acrylic Titlebar v<버전>`
     - 본문: 2단계에서 정리한 변경 목록. 상류는 이 내용을 모드 변경 이력으로 보여 줍니다.
     - footer: `Source: shiinamachi/wh-chrome-acrylic-titlebar@<기준 커밋 SHA>`
   - push까지만 합니다. PR은 열지 않습니다.

5. **기록하기**
   - push한 뒤 fork 브랜치의 파일이 기준 커밋의 파일과 같은지 다시 확인합니다.
   - 이 저장소의 기준 커밋에 `v<버전>` 태그를 달아 push합니다. 어떤 소스가 어느 버전으로 배포됐는지 태그로 찾을 수 있게 합니다.
   - 사용자에게 배포한 버전, 기준 커밋, fork 커밋 링크를 알리고, 상류 PR은 요청하면 열 수 있다고 덧붙입니다.

## 검증

배포 전이나 코드를 바꾼 뒤에는 둘 다 통과해야 합니다.

1. **상류 검사 스크립트:** `ramensoftware/windhawk-mods`의 `.github/pr_validation.py`, `preprocessor.py`, `extract_mod_symbols.py`를 받아 같은 폴더(`.github/`)에 두고, 모드 파일을 `mods/chrome-acrylic-titlebar.wh.cpp` 위치에 둔 뒤 실행합니다(`requests`, `pyyaml` 필요).

   ```bash
   python .github/pr_validation.py mods/chrome-acrylic-titlebar.wh.cpp kentakang 11825619
   ```

   오류는 0개여야 합니다. 검토 항목 `noUndecoratedSymbols` 하나는 의도한 것입니다(Chrome PDB가 약 5.8 GB라 전체 심볼을 언데코레이트하면 매우 느림).

2. **컴파일:** Windhawk 앱이 쓰는 것과 같은 옵션으로, Windhawk에 들어 있는 clang으로 빌드합니다. 경고 없이 끝나야 합니다.

   ```
   clang++.exe -std=c++23 -O2 -shared -DUNICODE -D_UNICODE -DWINVER=0x0A00 -D_WIN32_WINNT=0x0A00 -D_WIN32_IE=0x0A00 -DNTDDI_VERSION=0x0A000008 -D__USE_MINGW_ANSI_STDIO=0 -DWH_MOD "-DWH_MOD_ID=L\"chrome-acrylic-titlebar\"" "-DWH_MOD_VERSION=L\"<버전>\"" "<Windhawk>\Engine\<엔진 버전>\64\windhawk.lib" -x c++ chrome-acrylic-titlebar.wh.cpp -include windhawk_api.h -target x86_64-w64-mingw32 -Wl,--export-all-symbols -o mod.dll -ldwmapi -lshell32 -Wall -Wextra
   ```

   작업 폴더는 `<Windhawk>\Compiler`입니다.

3. **실제 Chrome 확인:** 사용자가 Windhawk에서 모드를 갱신하고 Chrome을 완전히 재시작한 뒤 캡처로 확인합니다. 비활성 창은 Acrylic이 단색으로 바뀌므로, 활성 창 캡처로 판단합니다.

## 구현 개요

Chrome은 창에 Mica 배경을 지정하지만 탭 줄과 주소창 줄을 불투명하게 칠해서 가립니다. 이 모드는 `chrome.dll`의 다음 함수들을 후킹해 그 칠하기를 걷어 내거나 바꿉니다. 모든 이름은 Chrome 158.0.8127.37 PDB로 확인했습니다.

| 대상 | 후킹 | 하는 일 |
| --- | --- | --- |
| 창 투명도 | `BrowserDesktopWindowTreeHostWin::ShouldWindowContentsBeTransparent` | `true`를 돌려줘 브라우저 창을 투명 배경으로 출력하게 합니다. 이때 같은 스레드에서 막 만든 HWND를 브라우저 창으로 표시합니다. |
| 배경 효과 | `DwmSetWindowAttribute`(`DWMWA_SYSTEMBACKDROP_TYPE`) | 표시한 창의 Mica 요청을 설정한 효과(기본 Acrylic)로 바꿉니다. |
| 제목 표시줄 | `BrowserFrameViewWin::OnPaint` | 칠하지 않습니다. 시크릿 창이면 어두운 틴트를 칠하고 HWND에 시크릿 표시를 남깁니다(`BrowserFrameView::GetBrowserView` → `BrowserView::GetProfile` → `Profile::IsIncognitoProfile`, `views::HWNDForView`). |
| 비활성 탭 | `BrowserTabStripController::InitFromModel` 직후 `TabStrip::SetIsGlassFrame(true)` | macOS의 유리 프레임처럼 비활성 탭이 배경을 칠하지 않게 합니다. Windows에는 이 상태를 켜는 `GlassFrameService`가 없습니다. 새 `TabView` 경로에 대비해 `TabStyleViewDelegateImpl::ShouldPaintTabBackgroundColor`도 `false`를 돌려줍니다. |
| 주소창 줄 | `views::View::OnPaintBackground` | 클래스 이름이 `ToolbarView`, `BookmarkBarView`면 `kClear`로 지우고 시크릿 창이면 틴트를 다시 칠합니다(아래에 Chrome이 칠한 불투명한 층이 있어서, 칠하지 않기만으로는 투명해지지 않습니다). `OmniboxViewViews`, `LocationIconView`, `ToolbarDivider`는 배경을 건너뜁니다. |
| 색 | `ui::ColorProvider::GetColor` | 주소창(`kColorLocationBarBackground*`)은 알파만 낮추고, 활성 탭(`kColorTabBackgroundActiveFrame*`)과 시크릿 칩(`kColorAvatarButtonHighlightIncognito`)은 어두운 테마에서 흰색 반투명 강조로, 탭 줄 버튼 배경(`kColorNewTabButton*Background*`, `kColorTabSearchButtonBackground`)은 투명하게 합니다. 색 ID는 `ChromeColorIdName`의 `{ID, 이름}` 표에서 이름으로 찾습니다. |
| 활성 탭 모양 | `HorizontalTabStyleViews::GetPath` | `PathType::kActiveTab`(0) 요청에 `kHighlight`(3)를 돌려줘, 주소창 줄과 이어지는 폴더 모양 대신 둥근 모양으로 그립니다. |
| 탭 구분선 | `HorizontalTabStyleViews::PaintTab` 동안 `gfx::Canvas::DrawRoundRect` | 탭 그리기에서 이 함수는 구분선에만 쓰이므로 건너뜁니다. 구분선 색은 Chrome이 알파를 덮어써서 색으로는 숨길 수 없습니다. |
| 창 단추 | `CreateWindowExW`, `Get/SetWindowLong(Ptr)W` | 투명한 탭 줄 아래에 Windows가 그리는 기본 창 단추를 숨기려고 브라우저 창의 `WS_SYSMENU`를 빼고, Chromium에는 원래 스타일을 보여 줍니다(Brave Acrylic 방식). |

심볼은 `chrome.dll`이 로드될 때 작업 스레드에서 찾고 최대 4초 기다립니다. 그보다 오래 걸리면 Chrome은 효과 없이 시작하고, 찾은 주소는 Windhawk가 캐시해 다음 실행부터 적용됩니다.

## Chrome 후킹 원칙

이 모드는 `chrome.dll` 내부 함수를 PDB 심볼로 찾아 후킹합니다. 새 후킹을 넣거나 Chrome 새 버전에 맞출 때 다음을 지킵니다.

- **이름 확인:** 데코레이트된 이름이 설치된 `chrome.dll`의 PDB 공개 심볼에 정확히 있는지 확인합니다. PDB는 Google 심볼 서버(`chromium-browser-symsrv.commondatastorage.googleapis.com`)에서 받을 수 있습니다.
- **ICF 확인:** 같은 주소를 공유하는 다른 심볼이 없는지 확인합니다. 본문이 합쳐진 함수를 후킹하면 무관한 코드까지 바뀝니다.
- **실제로 불리는지 확인:** 작은 함수는 호출 측에 인라인되거나 가상 호출이 없어져서 후킹해도 효과가 없을 수 있습니다. 호출 지점을 디스어셈블해서 실제 `call`(또는 CFG 간접 호출)로 남아 있는지 봅니다.
- **안정적인 이름:** 익명 네임스페이스(`?A0x...`)나 빌드 해시 접미사(`.llvm.…`, `.e7aaf…`)가 붙은 심볼은 Chrome 업데이트마다 바뀌므로 쓰지 않습니다.
- **ABI 확인:** 값으로 객체를 돌려주는 멤버 함수는 숨은 반환 포인터가 `this` 다음 인자로 들어갑니다. 호출 지점의 레지스터 배치를 보고 후킹 함수 형태를 맞춥니다.
- **색 ID:** 번호는 Chrome 버전마다 바뀌므로 상수로 쓰지 않고, `ChromeColorIdName`의 표에서 이름으로 찾습니다. 표 검증에 실패하면 해당 기능만 끕니다.
- **안전한 실패:** 심볼을 못 찾거나 검증에 실패하면 그 기능만 꺼지고 Chrome은 원래대로 동작해야 합니다.
- 확인한 Chrome 버전을 안내문의 "Notes"에 적습니다.
