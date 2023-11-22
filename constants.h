#pragma once
#include <string>

namespace constants
{
 //
    const int WINDOW_WIDTH = 1024;
    const int WINDOW_HEIGHT = 768;
    const int EVENT_TYPE_GUI = 1;

    const int  GUI_COMMAND_NONE = 0;
    const int  GUI_COMMAND_NEW = 1;
    const int  GUI_COMMAND_OPEN = 2;
    const int  GUI_COMMAND_SAVE = 3;

    const int  GUI_COMMAND_SET_ORIGIN = 4;
    const int  GUI_COMMAND_RUN = 5;
    const int  GUI_COMMAND_STOP = 6;
    const int  GUI_COMMAND_PAUSE = 7;
    const int  GUI_COMMAND_GOTO = 8;
    const int  GUI_COMMAND_CONNECT = 9;
    const int  GUI_COMMAND_DISCONNECT = 10;
    const int  GUI_COMMAND_EXIT = 11;
    const int  GUI_COMMAND_JUMP_TO_CURSOR = 12;
    const int  GUI_COMMAND_MANUAL = 13;
    const int  MAX_CELL_TEXT_LENGTH = 1024;

    const int  DATA_CHANNELS = 8;
    const int  DATA_DELAY_MS = 100;

    const float TABLE_COLOR_CURRENT_ROW_R = 0.5;
    const float TABLE_COLOR_CURRENT_ROW_G = 0.5;
    const float TABLE_COLOR_CURRENT_ROW_B = 1.0;

    // цвет событий поворотных осей
    const float SEQ_COLOR_ROT_R = 1.0;
    const float SEQ_COLOR_ROT_G = 0.0;
    const float SEQ_COLOR_ROT_B = 1.0;
    
    // цвет событий линейных осей
    // ImVec4(constants::SEQ_COLOR_LIN_R, constants::SEQ_COLOR_LIN_G, constants::SEQ_COLOR_LIN_B, 1);
    const float SEQ_COLOR_LIN_R = 0.3;
    const float SEQ_COLOR_LIN_G = 0.3;
    const float SEQ_COLOR_LIN_B = 1.0;
    
    // цвет событий при перекрытии
    // ImVec4(constants::SEQ_COLOR_INTERSECT_R, constants::SEQ_COLOR_INTERSECT_G, constants::SEQ_COLOR_INTERSECT_B, 1);
    const float SEQ_COLOR_INTERSECT_R = 1.0;
    const float SEQ_COLOR_INTERSECT_G = 1.0;
    const float SEQ_COLOR_INTERSECT_B = 1.0;

   // цвет событий при недостатке временм
    // ImVec4(constants::SEQ_COLOR_TIME_R, constants::SEQ_COLOR_TIME_G, constants::SEQ_COLOR_TIME_B, 1);
    const float SEQ_COLOR_TIME_R = 1.0;
    const float SEQ_COLOR_TIME_G = 0.0;
    const float SEQ_COLOR_TIME_B = 0.0;


    const float TABLE_COLOR_SELECTED_ROW_R = 1.0;
    const float TABLE_COLOR_SELECTED_ROW_G = 1.0;
    const float TABLE_COLOR_SELECTED_ROW_B = 0.5;


    const unsigned int COLOR_PALETTE[16] =
    {
     0xffff7070,
     0xff70ff70,
     0xff7070ff,     

     0xffffff70,
     0xff70ffff,
     0xffff70ff,

     0xffffff40,
     0xff40ffff,
     0xffff40ff,

     0xffff7040,
     0xff70ff40,
     0xffff4070,

     0xffff4040,
     0xff40ff40,
     0xffff4040,
     0xffffffff
    };

} // namespace constants
