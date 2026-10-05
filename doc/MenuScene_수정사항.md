# MenuScene / UI 작업 수정사항

UI 작업(MenuScene, Utils::DrawText, 초기화 위치 이동) 코드 리뷰 결과. 코드를 읽고 확인한 내용이며 빌드/실행 검증은 하지 않았다.

우선순위: 1, 2, 4, 5번 먼저.

## 버그

### 1. W/S 방향이 반대
- 위치: `Client/MenuScene.cpp` `Update()`
- W는 `_selMenuNumber++`라서 아래 메뉴("맵 제작")로 내려가고, S는 `--`라서 위로 올라간다.
- 조치: W는 감소(위), S는 증가(아래)로 바꾼다. 범위 순환 처리도 같이 맞춘다.
- [ ] 수정

### 2. 박스와 글자 위치 어긋남
- 위치: `MenuScene.h`, `MenuScene.cpp` `Render()`
- 박스 간격은 `+45`, 글자 간격은 `+40`이라 두 번째 메뉴에서 박스가 글자보다 5px 아래로 간다.
- "게임 시작"과 "맵 제작"의 글자 폭이 달라서 둘 다 `textPosX - 40`에 그리면 가운데 정렬이 안 맞는다.
- 간격 숫자(140, 40, 45)가 여러 곳에 흩어져 있다.
- 조치: 간격/기준 좌표를 한 곳의 상수로 관리하고, 글자와 박스가 같은 값을 쓰게 한다.
- [ ] 수정

### 3. Monster.cpp 이름 글자 모양 변경
- 위치: `Client/Monster.cpp:51` `Utils::DrawTextW(...)`
- `DrawText` 새 기본값(맑은 고딕, 20px, **흰색**)이 몬스터 이름에도 적용된다. 이전에는 기본 폰트, 검정색이었다.
- 흰색 글자가 맵 배경에서 안 보일 수 있다.
- 매 프레임, 몬스터마다 폰트를 만들고 지운다. 몬스터가 많아지면 부담이 된다.
- 조치: 실제 화면에서 가독성 확인. 필요하면 몬스터 쪽 호출에서 색을 지정하거나 기본 색을 검정으로 되돌린다. 자주 쓰는 크기는 폰트 캐싱을 검토한다.
- [ ] 확인/수정

### 4. Enter로 씬 전환 시 자기 자신을 delete
- 위치: `MenuScene::Update` → `SceneMgr::ChangeScene`
- `MenuScene::Update` 실행 중에 `ChangeScene`이 `SAFE_DELETE(_scene)`으로 실행 중인 MenuScene 자신을 삭제하고, 이어서 `newScene->Init()`까지 실행한다. 복귀 시점에 `this`는 해제된 메모리를 가리킨다.
- 호출 흐름:
  ```
  Core::Update
   └ SceneMgr::Update
      └ _scene->Update()              ← MenuScene::Update 실행 중
         └ SceneMgr::ChangeScene(GameScene)
            ├ new GameScene()
            ├ SAFE_DELETE(_scene)     ← 실행 중인 MenuScene delete
            ├ _scene = newScene
            └ newScene->Init()
         ← 해제된 this로 복귀
  ```
- 지금은 `ChangeScene` 호출 뒤에 멤버를 건드리는 코드가 없어서 우연히 안 터진다. 뒤에 `_keyPress = true;` 같은 코드를 추가하거나 다른 씬에서 같은 방식으로 호출하면 크래시/메모리 손상이 날 수 있다.
- 같이 있는 위험: `ChangeScene`은 sceneType이 case에 없으면 `newScene == nullptr`인 채로 `_scene`을 지우고 `newScene->Init()`을 호출한다.
- 조치: 전환을 "요청"과 "실행"으로 나눈다. `ChangeScene`은 다음 씬 타입만 기록하고 반환하고, `SceneMgr::Update`에서 `_scene->Update()`가 끝난 뒤 실제 삭제/생성/`Init()`을 한다. `Update`/`Render`가 `_scene` 검사를 이미 하므로 첫 프레임 처리는 안전하다.
- [ ] 수정

## 구멍

### 5. 서버 연결/사운드 초기화가 GameScene으로 이동
- 위치: `Client/GameScene.cpp` `PreLoad()`, `Client/Core.cpp`
- `GameScene::Init`마다 `SoundMgr::Init`, `NetMgr::Init`이 호출된다. GameScene에 두 번 들어가면 소켓 서비스가 또 만들어져 이전 연결이 남는다. 지금은 메뉴로 돌아가는 경로가 없어 드러나지 않는다.
- `TimeMgr`, `InputMgr`, `SceneMgr`, `ResMgr`의 `Init`은 `Core::Init`에서 이미 했으므로 `PreLoad`에서 다시 부를 필요가 없다.
- `NetMgr::Init`의 `assert(startResult)`는 서버가 꺼져 있으면 "게임 시작"을 누를 때 멈춘다. (이전에는 프로그램 시작 시 멈췄다.)
- `PreLoad`에 주석 처리된 `ChangeScene` 줄이 남아 있고, `virtual`이 붙어 있지만 `Scene`에는 이 함수가 없다.
- 조치: `PreLoad`는 `SoundMgr`/`NetMgr` 초기화만 남기고 한 번만 실행되게 한다. 중복 `Init`과 주석 줄, `virtual`을 정리한다.
- [ ] 수정

### 6. "맵 제작"이 EditScene으로 연결
- 위치: `MenuScene.cpp` `Update()` case 1
- `SceneType`에는 `MapEditScene`도 있다. 의도한 씬이 `EditScene`인지 확인한다.
- [ ] 확인

### 7. 입력 처리
- Enter가 `GetButton`(누르고 있는 동안)이라서, 다른 씬에서 Enter를 누른 채로 메뉴에 들어오면 바로 전환된다. `GetButtonDown`이 안전하다.
- `GetButtonDown`을 쓰면 키 반복 방지용 `_keyPress`도 필요 없어진다.
- [ ] 수정

### 8. 메뉴 개수가 코드에 고정
- 위치: `MenuScene.cpp` `if (_selMenuNumber > 1)`, `MenuScene.h` `_menuNumbers[2]`
- 메뉴를 추가하면 여러 곳을 같이 고쳐야 한다.
- 조치: 메뉴 개수를 상수 하나로 두고 범위 검사와 배열 크기가 그것을 쓰게 한다.
- [ ] 수정

## 인코딩

- 이번에 수정한 파일들은 BOM이 있는 UTF-8로 바뀌었다. `InputMgr.h`의 깨졌던 주석도 정상 한글로 복구됐다.
- `SoundMgr.cpp` 등은 여전히 CP949라서 저장소 안에 인코딩이 섞여 있다. BOM이 있는 파일은 컴파일에 문제가 없지만, CP949 파일을 VS에서 UTF-8로 저장할 때 한글이 깨지는지 가끔 확인한다.
- 통일하기로 하면 `.editorconfig`의 `charset = utf-8-bom`과 CLAUDE.md의 "CP949" 문구를 함께 고친다.
- [ ] 정책 결정

## 참고

- `Utils::DrawText` 시그니처: `(HDC hdc, Pos pos, const wstring& str, bool bold = false, int32 fontSize = 20, COLORREF color = RGB(255, 255, 255))`
- `Server/GameRoom.cpp` 변경은 UI 작업이 아니라서 리뷰 대상에서 제외했다.
