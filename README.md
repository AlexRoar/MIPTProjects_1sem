# Проекты проги МФТИ 1 семестр
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/75c9791e4ced4d66a24029409c69f22a)](https://app.codacy.com/manual/dremov.me/MIPTProjects_1sem?utm_source=github.com&utm_medium=referral&utm_content=AlexRoar/MIPTProjects_1sem&utm_campaign=Badge_Grade_Dashboard)
[![CodeFactor](https://www.codefactor.io/repository/github/alexroar/miptprojects_1sem/badge/master)](https://www.codefactor.io/repository/github/alexroar/miptprojects_1sem/overview/master)

## Транслятор assembly-подобного кода и SPU-исполнитель
В данном проекте реализован транслятор кода в неравномерный байткод, дизассемблер для байткода, исполнитель байткода (Soft Processor Unit).

В дополнение, присутствует анализатор кода, проверяющий синтаксические ошибки и выбрасывающий отчет в clang формате. Поэтому, XCode и другие IDE поддерживающие clang подсвечивают неверные выражения, отображают комментарии.

- [Основной код транслятора](https://github.com/AlexRoar/MIPTProjects_1sem/blob/master/SoftProcessorUnit/SoftProcessorUnit/Assembly/SPUAssembly.cpp)
- [Основной код дизассемблера](https://github.com/AlexRoar/MIPTProjects_1sem/blob/master/SoftProcessorUnit/SoftProcessorUnit/Disassembly/SPUDisAssembly.cpp)
- [Основной код SPU](https://github.com/AlexRoar/MIPTProjects_1sem/blob/master/SoftProcessorUnit/SoftProcessorUnit/SPU/SPU.cpp)
- [Примеры](https://github.com/AlexRoar/MIPTProjects_1sem/tree/master/SPUAsm/SPUAsm)

## Нерушимый стек
Стек который *почти* невозможно взломать. Хэш-суммы, проверки, валидации - тут. Отчеты о взломе моего стека и мой взлом товарища так же тут.
- [Стек на стероидах](StackOnSteroids/StackOnSteroids/StackRigid.h)
  - [Стек на стероидах docs](https://alexroar.github.io/MIPTProjects_1sem/StackOnSteroids/docs_doxygen/html/index.html)
  - [PDF взлома моего стека (от напарника)](StackOnSteroids/Vzlom_Alexandra_Dremova.pdf)
  - [PDF взлома стека напарника](StackOnSteroids/Взлом%20Артема.pdf)
  - [Проект напарника](https://github.com/JakMobius/MIPT-Tasks/tree/master/invulnerable-stack)
  

## Сортируем Онегина
**Предыстория:** Александр Сергеевич Пушкин, гений (любой) мысли написал Евгения Онегина – энциклопедию русской жизни. К сожалению, Александр Сергеевич был гуманитарий и написал энциклопедию не в авлфавитном порядке. Данный код помогает русскому писателю стать технарем.

Более того, если отсортировать строки с конца, строки будут выстраиваться в порядке рифмы.

### Сортировка методом поддержки верного порядка в контейнере на моменте считывания
- [Сортировщик Онегин](OneginSort/OneginSort/main.c) [insertion sort]
  - [Сортировщик Онегин docs](https://alexroar.github.io/MIPTProjects_1sem/OneginSort/docs/html/main_8c.html)
  
### Сортировка методом быстрой сортировки, с умным компаратором, учитывающим пробелы, пунктуацию, кириллицу.
- [Сортировщик Онегин v2](OneginSortv2/OneginSortv2/main.c) [quick sort + single line storage]
  - [Сортировщик Онегин v2 docs](https://alexroar.github.io/MIPTProjects_1sem/OneginSortv2/docs_sphinx/_build/html/api/c_doxygen_sphinx.html#program-documentation)
  - [Сортировщик Онегин v2 docs - raw Doxygen](https://alexroar.github.io/MIPTProjects_1sem/OneginSortv2/docs_doxygen/html/main_8c.html)
  
## Решение квадратных уравнений с документацией, проверками и блэк джэком.

- [Square Solver](SquareRoots/SquareRoots/main.c)
  - [Square Solver docs](https://alexroar.github.io/MIPTProjects_1sem/SquareRoots/docs/html/main_8c.html)
