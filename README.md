# Computer Club Schedule (C++)

Тестовое задание для стажировки на позицию стажёра в Yadro

Пример запуска:
```bash
./build/Computer_Club_Schedule test/test.txt
```

---

## Требования
- Стандарт: C++20
- Компилятор: gcc или clang (Linux), mingw/cygwin (Windows)
- CMake 3.15+ (рекомендуемая система сборки)

Дополнительно (необязательно):
- Google Test (для unit-тестов)
- clang-tidy (статический анализ)
- clang-format (форматирование)

---

## Сборка (CMake)

```bash
cmake -S . -B build
cmake --build build --parallel
```

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
