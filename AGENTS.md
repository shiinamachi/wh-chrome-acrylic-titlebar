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

## 버전

- `@version`은 배포할 때만 올립니다. 개발 중 커밋마다 올리지 않습니다.
- 한 번 배포한 버전 번호는 다시 쓰지 않습니다. 상류는 업데이트마다 더 높은 버전을 요구합니다.

## 배포 (`shiinamachi/windhawk-mods`)

- `shiinamachi/windhawk-mods`는 `ramensoftware/windhawk-mods`의 fork이며, **배포할 버전마다 커밋 하나만** 올립니다. 개발 이력은 이 저장소에만 남깁니다.
- 배포 커밋은 fork의 브랜치에 `mods/chrome-acrylic-titlebar.wh.cpp` 파일 하나만 바꾸고, 메시지는 상류 관례대로 `Chrome Acrylic Titlebar v<버전>`으로 씁니다.
- **브랜치에 push까지만 합니다.** `ramensoftware/windhawk-mods`에 PR을 여는 것, 상류 저장소에 댓글이나 이슈를 남기는 것은 사용자가 명시적으로 요청할 때만 합니다. "제출"처럼 모호한 표현은 push까지로 해석하고 묻습니다. 공개 PR은 삭제할 수 없고 닫기만 할 수 있습니다.
- 상류 PR 규칙: 파일 하나(`mods/<mod-id>.wh.cpp`)만 담고, `@github`가 PR 작성자 프로필과 같아야 하며, PR 본문에 템플릿의 "Mod authorship" 섹션을 남겨야 합니다.

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
