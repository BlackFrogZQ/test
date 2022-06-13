#ifndef HALCONSHARE_H
#define HALCONSHARE_H

#include "HalconCpp.h"
#include "HDevThread.h"
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QAction>
#include <QMainWindow>
#include <QtGui>
#include <QDebug>
using namespace HalconCpp;

void dev_update_off ();
// Chapter: Graphics / Text
// Short Description: This procedure writes a text message.
void disp_message (HTuple hv_WindowHandle, HTuple hv_String, HTuple hv_CoordSystem,
    HTuple hv_Row, HTuple hv_Column, HTuple hv_Color, HTuple hv_Box);
// Chapter: Graphics / Text
// Short Description: Set font independent of OS
void set_display_font (HTuple hv_WindowHandle, HTuple hv_Size, HTuple hv_Font, HTuple hv_Bold,
    HTuple hv_Slant);
void disp_continue_message (HTuple hv_WindowHandle, HTuple hv_Color, HTuple hv_Box);

#endif // HALCONSHARE_H