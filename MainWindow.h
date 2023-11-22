#pragma once
#include "constants.h"
#include <iostream>
#include <chrono>
#include <sstream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui_internal.h"
#include <GL/glew.h>            // Initialize with glewInit()
//Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>
#include "icons_font_awesome_4.h"
#include "events.h"
#include "ImGuiPropertyInspector.h"
class MainGUIWindow
{
public:
    MainGUIWindow(Events& events);
    ~MainGUIWindow();
    GLFWwindow* window;
    int win_width;
    int win_height;
    std::chrono::steady_clock::time_point prev_time;
    std::string statusMessage;
    bool initialized;
    bool resized;

    float toolbarSize;
    float statusbarSize;
    float menuBarHeight;    

    void ShowAppDockSpace(bool* p_open);
    void DockSpaceUI();
    int ToolbarUI();
    void StatusbarUI(std::string statusMessage);
    // -----------------------------
    // Organize our dockspace
    // -----------------------------
    int ProgramUI(std::string statusMessage);
    // -----------------------------
    // 
    // -----------------------------
    int render();
    void resize_window_callback(GLFWwindow* glfw_window, int x, int y);
    // -----------------------------
    // Initializing openGL things
    // -----------------------------
    void InitGraphics();
    // -----------------------------
    // Free graphic resources
    // -----------------------------
    void TerminateGraphics(void);

    void Run(void);
    void Stop(void);
    bool isGUILoopRunning;
    bool needStop;
    void GUILoop(void);
    std::thread* GUILoopThread;

public:
    Events& events_;    
};