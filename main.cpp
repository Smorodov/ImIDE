
// Tiny Json
// https://github.com/button-chen/tinyjson
// Для конфигурации

#include "MainWindow.h"
#include "main.h"

void main()
{
    { // скобки, чтобы лог освобождался
      // после освобождения всех классов.        
        //setlocale(LC_ALL, "ru_RU.utf8");

        // Русский язык в косноли
        setRussianConsole();        
            
        std::shared_ptr <Events> events;
        std::shared_ptr <MainGUIWindow> gui;

        // Настройка логгера
        spdlog::set_level(spdlog::level::warn);
        // Начало работы
        spdlog::info(u8"Цикл обработки событий запущен.");
        
        // создаём обработчик событий
        events = std::shared_ptr <Events>(new Events());
        // создаем GUI
        gui = std::shared_ptr<MainGUIWindow>(new MainGUIWindow(*events));

        // запуск цикла обработки событий
        events->Run();
        // запуск GUI
        gui->Run();

        spdlog::info(u8"Программа запущена.");
        // Все работает, пока не закроют окно
        while (gui->isGUILoopRunning)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        spdlog::info(u8"Программа отстановлена.");
        // Остановка всего что запустили
        gui->Stop();        
        events->Stop();        
    } // все классы освободились здесь
    
    // Освобождаем логгер
    spdlog::shutdown();
}
