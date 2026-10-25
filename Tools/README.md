# Tools

## png2bmp32

PNG를 **알파 채널이 있는 32bit BMP**로 변환한다.

- GDI(`LoadImage`)는 PNG를 읽지 못하므로, 알파가 필요한 이미지는 BMP로 변환해서 쓴다.
- 알파를 유지하고, `AlphaBlend`(`AC_SRC_ALPHA`)가 요구하는 대로 R/G/B에 알파를 미리 곱해서(premultiplied) 저장한다.
- 파이썬 표준 라이브러리만 사용한다. (Python 3 필요, 별도 설치 없음)

### 사용법

```
python Tools\png2bmp32.py 파일.png                        # 같은 폴더에 파일_a.bmp 생성
python Tools\png2bmp32.py a.png b.png                     # 여러 개 한 번에
python Tools\png2bmp32.py C:\git\ck_dot_art\rpg\ui        # 폴더 안의 *.png 전부
python Tools\png2bmp32.py a.png -o Resources\Sprite\UI    # 출력 폴더 지정
python Tools\png2bmp32.py a.png -s ""                     # 접미사 제거 (이름.bmp)
```

| 옵션 | 설명 |
|------|------|
| `inputs` | PNG 파일 또는 폴더 (여러 개 가능). 폴더는 하위 폴더를 제외하고 그 안의 `*.png`만 변환 |
| `-o`, `--out` | 출력 폴더. 생략하면 원본 PNG와 같은 폴더 |
| `-s`, `--suffix` | 출력 파일명 접미사. 기본값 `_a` (`ui_minimap.png` → `ui_minimap_a.bmp`) |

변환에 실패한 파일이 있으면 `FAIL <파일>: <이유>`를 출력하고 나머지는 계속 변환한다. 하나라도 실패하면 종료 코드는 1이다.

### 드래그&드롭

`png2bmp32.bat` 위로 PNG 파일(또는 폴더)을 끌어다 놓으면 같은 위치에 `<이름>_a.bmp`가 생성된다.

### 지원 범위

- 8/16bit RGB, RGBA, 그레이, 그레이+알파
- 1~8bit 팔레트 (`tRNS` 투명 포함)
- 인터레이스 PNG는 지원하지 않는다. (`FAIL`로 표시됨)

### 그리는 쪽 주의사항

- 변환한 BMP는 **`AlphaBlend`로 그려야 한다.** `BLENDFUNCTION`에 `AlphaFormat = AC_SRC_ALPHA`를 지정한다.
- 컬러키(마젠타)가 필요 없으므로 `LoadTexture`에 `RGB(255, 0, 255)`를 넘기지 않는다.
- 같은 텍스처를 `TransparentBlt`로 그리면 투명 처리가 되지 않는다.
- 로드 후 투명해야 할 곳이 까맣게 나오면 `Texture::LoadBmp`가 알파를 버리고 있는 것이니 로딩 코드를 확인한다.

### 예시 (미니맵)

```
python Tools\png2bmp32.py C:\git\ck_dot_art\rpg\ui\ui_minimap.png -o Resources\Sprite\UI
```

→ `Resources\Sprite\UI\ui_minimap_a.bmp` 생성. `GameScene`에서 `LoadTexture(L"MiniMap", L"Sprite\\UI\\ui_minimap_a.bmp")`로 로드하고, `MiniMap::Render`에서 `AlphaBlend`로 그린다. 전체 투명도는 `MiniMap::SetAlpha(0~255)`로 조절한다.
