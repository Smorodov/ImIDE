#include "MainWindow.h"
#include "FileBrowser/ImGuiFileBrowser.h"
#include "ImGuiPropertyInspector.h"
static imgui_addons::ImGuiFileBrowser file_dialog;
static bool show_open_dialog = false;
static bool show_save_dialog = false;
static property::inspector props_briwser("browser");
// -----------------------------
// Main window 
// -----------------------------
MainGUIWindow::MainGUIWindow(Events& events):events_(events)
{
	spdlog::info((const char*)u8"MainGUIWindow constructor.");
	window = nullptr;
	initialized = false;
	// Size of window
	win_width = constants::WINDOW_WIDTH;
	win_height = constants::WINDOW_HEIGHT;
	prev_time = std::chrono::steady_clock::now();
	statusMessage = "Message";

	toolbarSize = 50;
	statusbarSize = 50;
	menuBarHeight = 0;
	resized = false;
	
			props_briwser.clear();
            props_briwser.AppendEntity();
            props_briwser.AppendEntityComponent( property::getTable( props_briwser ), &props_briwser );	

//	props_briwser.AppendEntity();
//	props_briwser.getPropertyVTable()["gee"]=19;

}
MainGUIWindow::~MainGUIWindow()
{
	spdlog::info((const char*)u8"MainGUIWindow destructor.");

}
// -----------------------------
// 
// -----------------------------
void MainGUIWindow::DockSpaceUI()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	// Save off menu bar height for later.
	//menuBarHeight = ImGui::GetCurrentWindow()->MenuBarHeight();

	ImGui::SetNextWindowPos(viewport->Pos + ImVec2(0, toolbarSize + menuBarHeight));
	ImGui::SetNextWindowSize(viewport->Size - ImVec2(0, toolbarSize + menuBarHeight + statusbarSize));

	ImGui::SetNextWindowViewport(viewport->ID);
	ImGuiWindowFlags window_flags = 0
		| ImGuiWindowFlags_NoDocking
		| ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	ImGui::Begin("Master DockSpace", NULL, window_flags);
	ImGuiID dockMain = ImGui::GetID("MyDockspace");
	ImGui::DockSpace(dockMain, ImVec2(0.0f, 0.0f));
	ImGui::End();
	ImGui::PopStyleVar(3);
}
// -----------------------------
// 
// -----------------------------
int MainGUIWindow::ToolbarUI()
{
	int command = constants::GUI_COMMAND_NONE;
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y));
	ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, toolbarSize + menuBarHeight));
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGuiWindowFlags window_flags = 0
		| ImGuiWindowFlags_MenuBar
		| ImGuiWindowFlags_NoDocking
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoSavedSettings
		;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.5f, 1.0f));
	ImGui::Begin("TOOLBAR", NULL, window_flags);
	ImGui::PopStyleVar();
	menuBarHeight = ImGui::GetCurrentWindow()->MenuBarHeight();
	if (ImGui::Button(ICON_FA_FILE, ImVec2(0, 37)))
	{
		ImGui::OpenPopup((const char*)u8"Очистить всё");
	}

	// ---------------------------------------------
	// 	  Длалог очистки
	// ---------------------------------------------
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGuiStyle& style = ImGui::GetStyle();
	static ImVec4 prevCol = style.Colors[ImGuiCol_TitleBgActive];
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(1, 0.3, 0.3, 1);

	if (ImGui::BeginPopupModal((const char*)u8"Очистить всё", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text((const char*)u8"Очистить всйё?\n\n");
		ImGui::Separator();
		if (ImGui::Button((const char*)u8"Да", ImVec2(120, 0))) { command = constants::GUI_COMMAND_NEW;; ImGui::CloseCurrentPopup(); }
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button((const char*)u8"Отмена", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}
	style.Colors[ImGuiCol_TitleBgActive] = prevCol;

	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip((const char*)u8"Новый документ");
	}

	ImGui::SameLine();
	if (ImGui::Button(ICON_FA_FOLDER_OPEN_O, ImVec2(0, 37)))
	{
		command = constants::GUI_COMMAND_OPEN;
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip((const char*)u8"Загрузить документ");
	}
	ImGui::SameLine();
	if (ImGui::Button(ICON_FA_FLOPPY_O, ImVec2(0, 37)))
	{
		command = constants::GUI_COMMAND_SAVE;
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip((const char*)u8"Сохранить документ");
	}
	ImGui::SameLine();
	ImGui::Text(" | ", ImVec2(25, 0));
	ImGui::SameLine();
	if (ImGui::Button(ICON_FA_PLUG, ImVec2(0, 37)))
	{
		command = constants::GUI_COMMAND_CONNECT;
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip((const char*)u8"Подключить COM-порт.");
	}
	ImGui::SameLine();
	if (ImGui::Button(ICON_FA_BULLSEYE, ImVec2(0, 37)))
	{
		command = constants::GUI_COMMAND_SET_ORIGIN;
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip((const char*)u8"Установить ноль.");
	}
	ImGui::SameLine();
	ImGui::Text(" | ", ImVec2(25, 0));
	// -----------------------
	ImGui::BeginDisabled(false);
	ImGui::SameLine();
	if (ImGui::Button(ICON_FA_PLAY, ImVec2(0, 37)))
	{
		command = constants::GUI_COMMAND_RUN;
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip((const char*)u8"Пуск");
	}
	ImGui::EndDisabled();
	// -----------------------
	ImGui::BeginDisabled(false);
	ImGui::SameLine();
	if (ImGui::Button(ICON_FA_PAUSE, ImVec2(0, 37)))
	{
		command = constants::GUI_COMMAND_PAUSE;
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip((const char*)u8"Пауза");
	}
	ImGui::SameLine();
	ImGui::EndDisabled();
	// -----------------------
	ImGui::BeginDisabled(false);
	if (ImGui::Button(ICON_FA_STOP, ImVec2(0, 37)))
	{
		command = constants::GUI_COMMAND_STOP;
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip((const char*)u8"Стоп");
	}
	ImGui::EndDisabled();
	// -----------------------
	ImGui::BeginDisabled(false);
	ImGui::SameLine();
	if (ImGui::Button(ICON_FA_FAST_FORWARD, ImVec2(0, 37)))
	{
		command = constants::GUI_COMMAND_JUMP_TO_CURSOR;
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip((const char*)u8"Быстро к курсору.");
	}

	ImGui::EndDisabled();
	// -----------------------
	// Ручной режим   
	// -----------------------
	ImGui::SameLine();
	ImGui::Text(" | ", ImVec2(25, 0));
	ImGui::SameLine();
	if (ImGui::Button(ICON_FA_HAND_POINTER_O, ImVec2(0, 37)))
	{
		command = constants::GUI_COMMAND_MANUAL;
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip((const char*)u8"Ручной режим.");
	}
	// -----------------------


	// ----------------------------------------------------
	// Этрисовка меню
	// ----------------------------------------------------
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu((const char*)u8"Файл"))
		{
			if (ImGui::MenuItem((const char*)u8"Загрузить", "", false)) { command = constants::GUI_COMMAND_OPEN; }
			if (ImGui::MenuItem((const char*)u8"Сохранить", "", false)) { command = constants::GUI_COMMAND_SAVE; }
			ImGui::Separator();
			if (ImGui::MenuItem((const char*)u8"Выход", "", false)) { command = constants::GUI_COMMAND_EXIT; }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}


	ImGui::End();
	ImGui::PopStyleColor();

	return command;
}

void MainGUIWindow::StatusbarUI(std::string statusMessage)
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + viewport->Size.y - statusbarSize));
	ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, statusbarSize));
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGuiWindowFlags window_flags = 0
		| ImGuiWindowFlags_NoDocking
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoSavedSettings
		;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.5f, 1.0f));
	ImGui::Begin("STATUSBAR", NULL, window_flags);
	ImGui::PopStyleVar();
	ImGui::Text(statusMessage.c_str());
	ImGui::SameLine();

	char buf[1000] = { 0 };
	sprintf(buf, (const char*)u8" Статус");
	ImGui::Text(buf);

	ImGui::End();
	ImGui::PopStyleColor();
}
// -----------------------------
// Organize our dockspace
// -----------------------------
int MainGUIWindow::ProgramUI(std::string statusMessage)
{
	DockSpaceUI();
	int command = ToolbarUI();
	StatusbarUI(statusMessage);
	return command;
}
// -----------------------------
// 
// -----------------------------
int MainGUIWindow::render()
{
	ImGui::GetIO().WantCaptureMouse = true;
	glfwPollEvents();
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	// Render our dock (menu, toolbar, status bar).    
	int command = ProgramUI(statusMessage);
	if (command == constants::GUI_COMMAND_NEW)
	{
		ImGui::OpenPopup((const char*)u8"Новый документ");

	}
	if (command == constants::GUI_COMMAND_OPEN)
	{
		ImGui::OpenPopup((const char*)u8"Открыть файл");
		show_open_dialog = false;
	}
	else if (command == constants::GUI_COMMAND_SAVE)
	{
		ImGui::OpenPopup((const char*)u8"Сохранить файл");
		show_save_dialog = false;
	}

	// -------------------
// Окно ручного управления
// -------------------
	auto center = ImGui::GetMainViewport()->GetCenter();
	int trackInd = 0;

	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	if (ImGui::BeginPopupModal((const char*)u8"Ручное управление", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{

		// -----------
		ImGui::Separator();
		//if (ImGui::Button((const char*)u8"Добавить событие.", ImVec2(250, 0))) 
		//{
		
		//}
		
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button((const char*)u8"Закрыть", ImVec2(120, 0))) {  ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}
	else if (command == constants::GUI_COMMAND_MANUAL)
	{
		ImGui::OpenPopup((const char*)u8"Ручное управление");
	}

	//Show an open/save/select file dialog. Last argument provides a list of supported files. Selecting other files will show error. If "*.*" is provided, all files can be opened.
	if (file_dialog.showFileDialog((const char*)u8"Открыть файл", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(900, 600), ".tbl"))
	{
		printf("%s\n", file_dialog.selected_fn.c_str());
		printf("%s\n", file_dialog.selected_path.c_str());
		
	}
	if (file_dialog.showFileDialog((const char*)u8"Сохранить файл", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(900, 600), ".tbl"))
	{
		printf("%s\n", file_dialog.selected_fn.c_str());
		printf("%s\n", file_dialog.ext.c_str());
		printf("%s\n", file_dialog.selected_path.c_str());
		
	}



	// Rendering user content
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	/*
			static bool open_popup = true;
			static std::string m_file_path;

			if (open_popup)
			{
				ImGui::OpenPopup("TEST Selection");
			}

			if (ImGui::BeginPopupModal("TEST Selection"))
			{
				if (ImGui::Button("Cancel"))
				{
					ImGui::CloseCurrentPopup();
					open_popup = false;
					m_file_path = "test";
				}
				static char text[1000];
				ImGui::Text("file");
				ImGui::SameLine();
				ImGui::PushItemWidth(-175);
				if (ImGui::InputText("##file_123", text,
					ImGuiInputTextFlags_EnterReturnsTrue)) {
					ImGui::CloseCurrentPopup();
					open_popup = false;
					m_file_path = text;
				}
				ImGui::SameLine();
				if (ImGui::Button("Other")) {
					ImGui::CloseCurrentPopup();
					open_popup = false;
					m_file_path = "test";
				}
				ImGui::EndPopup();
			}
			*/
	
	props_briwser.Show([]
        {
            if( ImGui::Button( "  Undo  "  ) ) props_briwser.Undo();
            ImGui::SameLine(80);
            if( ImGui::Button( "  Redo  ") ) props_briwser.Redo();
        } );


	std::time_t result = std::time(nullptr);
	std::stringstream statusMessageTime;
	statusMessageTime << std::ctime(&result);
	statusMessage = statusMessageTime.str();

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	// std::cout << "Elapseed = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
	// std::cout << "Elapseed (frame)= " << std::chrono::duration_cast<std::chrono::microseconds>(end - prev_time).count() << "[µs]" << std::endl;
	prev_time = end;


	// ImGui rendering
	glClearColor(0, 0, 0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(window);
	return command;
}

void MainGUIWindow::resize_window_callback(GLFWwindow* glfw_window, int x, int y)
{
	if (x == 0 || y == 0)
	{
		return;
	}
	if (initialized)
	{
		render();
	}
	resized = true;
}
// -----------------------------
// Initializing openGL things
// -----------------------------
void MainGUIWindow::InitGraphics()
{
	// Setup window
	//glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
	{
		return;
	}

	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	// Create window with graphics context
	window = glfwCreateWindow(win_width, win_height, (const char*)u8"Шарманка", NULL, NULL);
	if (window == NULL)
	{
		return;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

	// Load Fonts        
	io.Fonts->AddFontFromFileTTF("fonts/FiraCode/ttf/FiraCode-Regular.ttf", 30, NULL, io.Fonts->GetGlyphRangesCyrillic());
	// merge in icons from Font Awesome
	static  ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
	io.Fonts->AddFontFromFileTTF("fonts/fontawesome-webfont.ttf", 30.0f, &icons_config, icons_ranges);
	// Load Fonts        
	io.Fonts->AddFontFromFileTTF("fonts/a_FuturaOrto.TTF", 20, NULL, io.Fonts->GetGlyphRangesCyrillic());


	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	// Initialize GLEW
	glewExperimental = true; // Needed for core profile

	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		return;
	}

	//glfwSetWindowSizeCallback(window, resize_window_callback);
	initialized = true;


	ImGuiStyle& style = ImGui::GetStyle();

	style.ChildRounding = 3.f;
	style.GrabRounding = 0.f;
	style.WindowRounding = 0.f;
	style.ScrollbarRounding = 3.f;
	style.FrameRounding = 3.f;
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

}


// -----------------------------
// Free graphic resources
// -----------------------------
void MainGUIWindow::TerminateGraphics(void)
{
	initialized = false;
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
}

// -----------------------------
// 
// -----------------------------
void MainGUIWindow::GUILoop(void)
{
	InitGraphics();
	isGUILoopRunning = true;
	spdlog::info((const char*)u8"Вход в GUILoop.");
	while (!needStop && !glfwWindowShouldClose(window))
	{
		int command = render();
		if (command != constants::GUI_COMMAND_NONE)
		{
			spdlog::info((const char*)u8"Worker: enqueue commandEvent({0}).", command);
			events_.queue.enqueue(constants::EVENT_TYPE_GUI, std::shared_ptr < MyEvent>(new MyEvent(command, 1)));
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}
	spdlog::info((const char*)u8"Освобожление ресурсов графики.");
	isGUILoopRunning = false;
	TerminateGraphics();
}
// -----------------------------
// 
// -----------------------------
void MainGUIWindow::Run(void)
{
	spdlog::info((const char*)u8"Environment loop starting.");
	GUILoopThread = new std::thread(&MainGUIWindow::GUILoop, this);
	isGUILoopRunning = true;
	needStop = false;
}
// -----------------------------
// 
// -----------------------------
void MainGUIWindow::Stop(void)
{
	needStop = true;
	if (GUILoopThread != nullptr)
	{
		if (GUILoopThread->joinable())
		{
			GUILoopThread->join();
		}

		delete GUILoopThread;
		GUILoopThread = nullptr;
	}
}
// -----------------------------
// 
// -----------------------------