# AGENTS.md

## Project Overview

C++ 멀티플레이어 2D RPG 게임 클라이언트/서버 프로젝트. WinAPI 기반 클라이언트와 IOCP 기반 서버로 구성된 학습용 프로젝트.

## Architecture

### Directory Structure

```
Client/          - WinAPI 기반 2D 게임 클라이언트
Server/          - 게임 서버 (GameRoom, Monster AI 등)
ServerCore/      - IOCP 네트워크 라이브러리 (Session, Service, Listener)
Common/protoc/   - Protocol Buffers 정의 (.proto) 및 코드 생성 스크립트
DummyClient/     - 네트워크 스트레스 테스트용 더미 클라이언트
Libraries/       - 빌드된 라이브러리 및 헤더 (Include/, Libs/)
Resources/       - 게임 리소스 (스프라이트, 사운드, 타일맵 데이터)
Binaries/        - 빌드 출력물
Intermediate/    - 중간 빌드 파일
protobuf-3.21.12/ - Protocol Buffers 3.21.12 소스
```

### Build System

- **IDE**: Visual Studio (RpgProject.sln)
- **언어**: C++20 (`<format>`, `<filesystem>` 사용)
- **프로젝트 4개**: Client, Server, ServerCore (static lib), DummyClient
- **프로토콜 코드 생성**: 빌드 후 이벤트에서 `Common/protoc/bin/GenPackets.bat` 실행
- **Include 경로**: `Libraries/Include/`, `ServerCore/`
- **Library 경로**: `Libraries/Libs/`

### Networking

- **프로토콜**: Protocol Buffers 3.21.12
- **서버 I/O**: Windows IOCP (IocpCore → Session → PacketSession)
- **패킷 구조**: `PacketHeader { uint16 size; uint16 id; }` + protobuf payload
- **Proto 파일**: `Common/protoc/bin/` 아래 Enum.proto, Struct.proto, Protocol.proto
- **패킷 핸들러**: ClientPacketHandler (클라이언트), ServerPacketHandler (서버)

### Client Architecture

**엔티티 계층:**
```
Actor → FlipbookActor → GameObject → Creature → Player → MyPlayer
                                              → Monster
                                   → Arrow (Projectile)
                                   → HitEffect
```

**매니저 (싱글톤, SING() 매크로):**
- SceneMgr - 씬 관리, 카메라, MyPlayer 추적
- NetMgr - 네트워크 송수신
- ObjMgr - 게임오브젝트 ID/인스턴스 관리
- ResMgr - 리소스 로딩/캐싱 (텍스처, 스프라이트, 플립북, 타일맵, 사운드)
- InputMgr - 키보드/마우스 입력
- TimeMgr - 델타타임, FPS (QueryPerformanceCounter)
- CollisionMgr - 충돌 감지
- SoundMgr - DirectSound 오디오

**씬:** GameScene (메인 게임플레이), EditScene, MapEditScene

**렌더링:** Texture → Sprite → Flipbook → FlipbookActor, 레이어 기반 렌더링

### Server Architecture

- **GameRoom** - 게임 월드 (플레이어/몬스터 관리, 브로드캐스트)
- **GameSession** - 플레이어 네트워크 세션
- **Monster AI** - 상태머신 (Idle/Move/Skill/Dead)
- **A* 길찾기** - 그리드 기반 네비게이션

### Key Patterns

- 싱글톤 패턴 (SING 매크로)
- 상태머신 (TickIdle/TickMove/TickSkill/TickDead)
- 컴포넌트 패턴 (CameraComponent, BoxCollider)
- 레이어 기반 렌더링/충돌

## Protocol Messages

| ID | Message | 방향 | 용도 |
|----|---------|------|------|
| 4  | S_MyPlayer | S→C | 내 플레이어 스폰 |
| 5  | S_AddObject | S→C | 오브젝트 추가 |
| 7  | S_RemoveObject | S→C | 오브젝트 제거 |
| 10 | C_Move | C→S | 이동 요청 |
| 11 | S_Move | S→C | 이동 브로드캐스트 |
| 12 | C_Attack | C→S | 근접 공격 |
| 13 | S_Attack | S→C | 공격 브로드캐스트 |
| 14 | C_Attack_Arrow | C→S | 화살 공격 |
| 15 | S_Attack_Arrow | S→C | 화살 브로드캐스트 |
| 16 | C_ObjectWeaponChange | C→S | 무기 변경 |

## Rules

- 질문에는 답변만 한다. 절대로 코드를 작성하지 않는다. 사용자가 명시적으로 "코딩해줘", "구현해줘", "작성해줘" 등 요청하기 전까지는 코드 수정/생성 금지.
- 프로젝트 파일은 CP949(EUC-KR) 인코딩이므로 한글 주석/문자열 작성 시 주의. 폰트 이름 등은 영문 사용 (예: L"Malgun Gothic")

## Conventions

- 커밋 메시지: 한국어, `날짜: 내용` 형식 (예: `11/26: 화살 발사 서버작업`)
- 매크로: `SING()` (싱글톤), `GET_SINGLE()` (인스턴스 접근), `DT` (델타타임)
- 좌표계: 셀 기반 그리드 (TILE_SIZEX=48, TILE_SIZEY=48)
- 캐릭터 타입: Knight, Mage, Archer
- 무기 타입: Sword, Staff, Bow
