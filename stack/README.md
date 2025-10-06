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
    <li><a href="#usage">Примеры использования</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## О проекте

Стек является одним из наиболее распространённых способов хранения данных, работающих по принципу "последний положил, первый достал". Моя версия стека реализована с использованием защит, таких, как: канарейки, хеш, яд. Стек может имеет неограниченный (программно) размер, реализован с использованием шаблонов, классов и методов. 

Стек имеет следующий набор функций (методов):


- `int Ctor(const int capacity, const char *file_name)` - констурктор, создать стек вместимостью capacity и логфайлом file_name;
- `int Dtor()` - деструктор, уничтожить стек;
- `int push(const T data)` - поместить data в стек;
- `int pop(T *data)` - достать данные из стека в data
- `int print(int option)` - печать стека:
    + IN_CONSOLE - в консоль, 
    + IN_FILE - внутрь логфайла
- `int stk_realloc(const int num)` - изменить размер стека:
    + `UP` - Увеличить в два раза
    + `DOWN` - Уменьшить в два раза
- `T get_data_on_pos(const int pos)` - получить данные на позиции pos в стеке
- `T* get_data_ptr()` - получить указатель на данные
- `int get_stack_size()` - получить размер стека
  
В случае успеха методы с типом `int` возвращают 0, в ином случае произошла ошибка.

При компиляции вашего проекта нужно указать макрос:
- `DEBUG` - для дебаггинга и отладки в логфайлы.
- `SECURE` - для включения защитных функций (канарейки, хеши)

<p align="right">(<a href="#readme-top">Вернуться наверх</a>)</p>


<!-- GETTING STARTED -->
## Начало работы

Чтобы воспользоваться программой, выполните следующие шаги:

1. Склонируйте репозиторий в свою среду vscode:
   ```sh
   git clone git@github.com:yayamayaya/stack.git
   ```
2. Подгрузите библиотеку `stack.h` в ваш проект



<!-- USAGE EXAMPLES -->
## Примеры использования

Пример использования предоставлен в ветке stack_test, перейдите в эту ветку, создайте проект с помощью стандартного build task и тестируйте стек в удовольствие

