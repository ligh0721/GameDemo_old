#ifndef COMMON_H
#define COMMON_H

#include <QWidget>
#include <QApplication>
#include <QMainWindow>
#include <QtEvents>
#include <QStringList>
#include <QStringListModel>
#include <QMessageBox>
#include <QModelIndexList>
#include <QFileDialog>
#include <QListWidgetItem>

#ifndef A2U
#define A2U(str) QString::fromLocal8Bit((str))
#endif

#include <cocos2d.h>
#include "../Classes/CommInc.h"
#include <CCEGLView.h>
USING_NS_CC;


#endif // COMMON_H
