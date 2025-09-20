The http coordinate extractor
Сервер http - парсер координат разных форматов из текста 
###

<div align="left">
  <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/cplusplus/cplusplus-original.svg" height="40" alt="cplusplus logo"  />
  <img width="12" />
  <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/qt/qt-original.svg" height="40" alt="qt logo"  />
</div>

###

Пример использования 

```bash
mint@mint:~$ curl -X POST http://localhost:8080/extract \
-H "Content-Type: application/json" \
-d '{"text":"N55.7558 E37.6173"}'
{
    "координаты": [
        {
            "долгота": "37.61730000",
            "контекст_предложения": "N55.7558 E37.6173",
            "корректна": true,
            "название_координаты": "",
            "оригинальный_текст": "N55.7558 E37.6173",
            "позиция_в_тексте": 0,
            "статус": "корректна",
            "формат": "десятичные_градусы",
            "широта": "55.75580000"
        }
    ],
    "ошибка": "",
    "тип_геометрии": "одиночные_точки"
}
mint@mint:~$ curl -X POST http://localhost:8080/extract \
-H "Content-Type: application/json" \
-d '{"text":"55.7558 37.6173"}'
{
    "координаты": [
        {
            "долгота": "37.61730000",
            "контекст_предложения": "55.7558 37.6173",
            "корректна": true,
            "название_координаты": "",
            "оригинальный_текст": "55.7558 37.6173",
            "позиция_в_тексте": 0,
            "статус": "корректна",
            "формат": "десятичные_градусы",
            "широта": "55.75580000"
        }
    ],
    "ошибка": "",
    "тип_геометрии": "одиночные_точки"
}
```
