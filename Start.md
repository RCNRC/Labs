# Начало работы

login: iakovenko.in
password: Hm1F6MTewg

Быстрое подключение из терминала по команде: `ssh iakovenko.in@samos.dozen.mephi.ru`, дальше вводим пароль `Hm1F6MTewg`

## Работа с vi -> компиляция -> исполнение

1. `vi n.c` = открыть файл `n.c` на редактирование.
2. `ctrl+shift+v` = вставить текст из юуфера обмена.
3. `:` -> `i` = начать редактировать файл нормальны способом.
4. `ESC` -> `:` -> `wq` = выйти из редактирования, сохранить файл и выйти из вима.
5. `gcc n.c -o n` - компиляция программы n.c в исполняемый файл n
6. `./n param1 param2 ...` - запуск программы `n` с параметрами `param1`, `param2`, `...`.
