python Tools/png2bmp32.py C:\\git\\TheLegendOfBeak\\Resources\\Sprite\\Building

* Client
일반 -> 출력 디렉토리
$(SolutionDir)Libraries\Client\$(Configuration)\
일반 -> 중간 디렉토리
$(SolutionDir)Intermediate\Client\$(Configuration)\
C/C++ -> 일반 -> 추가 포함디렉터리
$(SolutionDir)Libraries\Include\;$(SolutionDir)ServerCore\;%(AdditionalIncludeDirectories)
링커 -> 일반 -> 추가 라이브러리 디렉터리
$(SolutionDir)Libraries\Libs\;%(AdditionalLibraryDirectories)

* DummyClient
일반 -> 출력 디렉토리
$(SolutionDir)Binaries\DummyClient\$(Platform)\
일반 -> 중간 디렉토리
$(SolutionDir)Intermediate\DummyClient\$(Platform)\
C/C++ -> 일반 -> 추가 포함디렉터리
$(SolutionDir)Libraries\Include\;$(SolutionDir)ServerCore\;%(AdditionalIncludeDirectories)
링커 -> 일반 -> 추가 라이브러리 디렉터리
$(SolutionDir)Libraries\Libs\;%(AdditionalLibraryDirectories)

* Server
일반 -> 출력 디렉토리
$(SolutionDir)Binaries\$(Platform)\
일반 -> 중간 디렉토리
$(SolutionDir)Intermediate\$(Platform)\
C/C++ -> 일반 -> 추가 포함디렉터리
$(SolutionDir)Libraries\Include\;$(SolutionDir)ServerCore\;%(AdditionalIncludeDirectories)
링커 -> 일반 -> 추가 라이브러리 디렉터리
$(SolutionDir)Libraries\Libs\;%(AdditionalLibraryDirectories)

구성 속성 -> 빌드 이벤트 -> 빌드 전 이벤트 -> 명령줄
call "$(SolutionDir)\Common\protoc\bin\GenPackets.bat"

* ServerCore
일반 -> 출력 디렉토리
$(SolutionDir)Libraries\ServerCore\$(Configuration)\
일반 -> 중간 디렉토리
$(SolutionDir)Intermediate\ServerCore\$(Configuration)\