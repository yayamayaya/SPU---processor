<!-- Improved compatibility of Вернуться наверх link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->
<a name="readme-top"></a>
<!--
*** Thanks for checking out the Best-README-Template. If you have a suggestion
*** that would make this better, please fork the repo and create a pull request
*** or simply open an issue with the tag "enhancement".
*** Don't forget to give the project a star!
*** Thanks again! Now go create something AMAZING! :D
-->



<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
<!-- PROJECT LOGO -->
<br />
<div align="center">

<h3 align="center">Мой стек</h3>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Таблица содержимого</summary>
  <ol>
    <li>
      <a href="#about-the-project">О проекте</a>
    </li>
    <li>
      <a href="#getting-started">Начало работы</a>
    </li>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->
## О проекте

Стек является одним из наиболее распространённых способов хранения данных, работающих по принципу "последний положил, первый достал". Моя версия стека реализована с использованием защит, таких, как: канарейки, хеш, яд. Стек имеет неограниченный (программно) размер, реализован с использованием шаблонов, классов и методов.

Стек имеет следующий набор функций (методов):

- `Stack(const size_t capacity = 16)` - стандартный констурктор, создать стек вместимостью capacity по умолчанию равном 16, заметьте, что при `capacity` равном 0 конструктор выкинет исключение;
- `~Stack()` - деструктор, уничтожить стек;
- `void enable_stack_log(const char *log_filename)` - открыть лог для стека с названием `log_filename`. Лог будет помещён в папку `logs`;
- `stack_err_t push(const T value)` - поместить value в стек. Возвращает код ошибки, если целостность стека была нарушена;
- `T pop()` - достать данные из стека. Выкинет исключение, если целостность стека была нарушена либо если стек пуст;
- `void print(FILE *out_buff = stdout)` - печать стека. По умолчанию печатается в `stdout`, однако можно передать желаемый файл в качестве аргумента. При передаче `stderr` будет напечатано расширенное содержимое стека, вместе со всеми защитами;
-  `void log_print()` - вывести расширенное содержимое стека в лог;
- `stack_err_t resize(const size_t new_capacity)` - изменить размер стека до `new_capacity`;
- `const T *at(const size_t ind)` - получить данные на позиции `ind` в стеке;
- `size_t stack_size()` - получить размер стека

В случае успеха методы с типом `stack_err_t` возвращают 0, в ином случае произошла ошибка и она будет напечатана вместе со стеком в `stderr`.

При компиляции вашего проекта можно создать макрос `SECURED` для включения защитных функций (канарейки, хеши, яд)

<p align="right">(<a href="#readme-top">Вернуться наверх</a>)</p>


<!-- GETTING STARTED -->
## Начало работы

Чтобы воспользоваться программой, выполните следующие шаги:

1. Склонируйте репозиторий в свою среду vscode:
   ```sh
   git clone git@github.com:yayamayaya/stack.git
   ```
2. Подгрузите библиотеку `stack.h` в ваш проект
3. В `CMake` не забудьте добавить субдерикторию `src` стека и сделать `target_link_libraries(... stack)`
