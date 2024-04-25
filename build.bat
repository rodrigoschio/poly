@if exist game.exe del game.exe

poly\tcc\tcc.exe game.c poly\poly.c

@echo:
@if exist game.exe @echo: O executavel esta disponivel !
@if not exist game.exe @echo: O codigo fonte nao esta de acordo com as regras da linguagem C.
@echo:

@pause
