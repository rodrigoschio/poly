@chcp 1252 >nul
@if exist executavel.exe del executavel.exe
@poly\tcc\tcc.exe game.c poly\poly.c -w
@if exist game.exe @move game.exe executavel.exe

@if not exist executavel.exe @echo:
@if not exist executavel.exe @echo:O c�digo fonte n�o est� de acordo com as regras da linguagem C
@if exist executavel.exe @cls
@if exist executavel.exe @echo:O arquivo executavel est� dispon�vel caso voc� queira compartilhar o jogo !
@echo:

@pause
@if exist executavel.exe executavel.exe 
