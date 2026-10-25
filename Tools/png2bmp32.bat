@echo off
rem PNG 파일/폴더를 이 bat 위로 드래그&드롭하면 같은 위치에 <이름>_a.bmp 로 변환한다.
python "%~dp0png2bmp32.py" %*
pause
