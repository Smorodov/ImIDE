
// Tiny Json
// https://github.com/button-chen/tinyjson
// ��� ������������

#include "MainWindow.h"
#include "main.h"

void main()
{
    { // ������, ����� ��� ������������
      // ����� ������������ ���� �������.        
        //setlocale(LC_ALL, "ru_RU.utf8");

        // ������� ���� � �������
        setRussianConsole();        
            
        std::shared_ptr <Events> events;
        std::shared_ptr <MainGUIWindow> gui;

        // ��������� �������
        spdlog::set_level(spdlog::level::warn);
        // ������ ������
        spdlog::info(u8"���� ��������� ������� �������.");
        
        // ������ ���������� �������
        events = std::shared_ptr <Events>(new Events());
        // ������� GUI
        gui = std::shared_ptr<MainGUIWindow>(new MainGUIWindow(*events));

        // ������ ����� ��������� �������
        events->Run();
        // ������ GUI
        gui->Run();

        spdlog::info(u8"��������� ��������.");
        // ��� ��������, ���� �� ������� ����
        while (gui->isGUILoopRunning)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        spdlog::info(u8"��������� ������������.");
        // ��������� ����� ��� ���������
        gui->Stop();        
        events->Stop();        
    } // ��� ������ ������������ �����
    
    // ����������� ������
    spdlog::shutdown();
}
