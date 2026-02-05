# Computer Club Schedule (C++)

Тестовое задание для стажировки на позицию стажёра в Yadro

Пример запуска:
```bash
./build/Computer_Club_Schedule test/test.txt
```

---

## Требования
- Стандарт: C++20
- Компилятор: clang++-18 (используется по умолчанию в CMake)
- CMake 3.15+ (рекомендуемая система сборки)
- Сборка в Debug с санитайзерами (Address/Undefined/Leak)

Дополнительно (необязательно):
- Google Test (для unit-тестов)
- clang-tidy (статический анализ)
- clang-format (форматирование)

### Установка Google Test

Ubuntu/Debian:
```bash
sudo apt update
sudo apt install -y libgtest-dev
```

На Ubuntu/Debian библиотека ставится в виде исходников. Нужно собрать её один раз:
```bash
cd /usr/src/gtest
sudo cmake -S . -B build
sudo cmake --build build
```

Arch:
```bash
sudo pacman -S gtest
```

Fedora:
```bash
sudo dnf install -y gtest gtest-devel
```

### Установка clang-tidy и clang-format

Ubuntu/Debian:
```bash
sudo apt update
sudo apt install -y clang-tidy clang-format
```

Arch:
```bash
sudo pacman -S clang
```

Fedora:
```bash
sudo dnf install -y clang-tools-extra clang
```

---

## Сборка (CMake)

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
```

Если нужно явно указать компилятор:
```bash
CC=/usr/bin/clang-18 CXX=/usr/bin/clang++-18 cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
```

Используемые флаги сборки (из CMake):
- -O0 -g
- -Werror -Wall -Wextra -Wpedantic -Wconversion -Wcast-align -Wcast-qual -pedantic-errors
- -fsanitize=address -fsanitize=undefined -fsanitize=leak -fno-omit-frame-pointer -fno-sanitize-recover=all

После сборки исполняемый файл находится в папке build.

---

## Запуск

Входной файл передаётся первым аргументом:
```bash
./build/Computer_Club_Schedule test/test.txt
```

---

## Тесты (опционально)

Если установлен Google Test:
```bash
./build/runTests
```

---

## Анализ и форматирование (опционально)

Если установлен clang-tidy, можно запустить цель lint:
```bash
cmake --build build --target lint
```

clang-format используется для ручного форматирования исходников.

---

## Примеры входных данных

Файлы с примерами лежат в папке test и имеют расширение .txt.
Можно запускать любой из них:
```bash
./build/Computer_Club_Schedule test/test_outside_hours.txt
```

## Сноска по ошибкам

Программа генерирует ошибки (ID 13) в следующих случаях:

| Ошибка | Событие | Условие |
|--------|--------|---------|
| **NotOpenYet** | Type 1 (Клиент пришел) | Время события вне рабочего времени (до open_time или после close_time) |
| **YouShallNotPass** | Type 1 (Клиент пришел) | Клиент уже находится в компьютерном клубе |
| **PlaceIsBusy** | Type 2 (Клиент сел за стол) | Запрашиваемый стол уже занят (включая случай, когда клиент пытается пересесть за свой же стол) |
| **ClientUnknown** | Type 2 (Клиент сел за стол) | Клиент не находится в компьютерном клубе |
| **UnknownTable** | Type 2 (Клиент сел за стол) | Номер стола выходит за пределы диапазона [1, N] |
| **ClientUnknown** | Type 3 (Клиент сел за стол) | Клиент не находится в компьютерном клубе |
| **ICanWaitNoLonger!** | Type 3 (Клиент ожидает) | В компьютерном клубе есть свободные столы (клиент не может встать в очередь) |
| **ClientAlreadyHasTable!** | Type 3 (Клиент ожидает) | Клиент не может ожидать стол, так как уже сидит за столом |
| **ClientUnknown** | Type 4 (Клиент ушел) | Клиент не находится в компьютерном клубе |
