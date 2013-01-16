#include "CommInc.h"
#include "FileEditor.h"


CFileEditor::CFileEditor()
    : m_pWin(NULL)
    , m_bChanged(false)
{
}

CFileEditor::~CFileEditor()
{
}

bool CFileEditor::init(const QString& roName, const QString& roFilter, QWidget* pWin)
{
    m_oEditorName = roName;
    m_oFilter = roFilter;
    m_pWin = pWin;
    m_bChanged = false;

    return true;
}

void CFileEditor::setChanged(bool bChanged)
{
    m_bChanged = bChanged;
    if (m_pWin)
    {
        QString s = m_oFileName.isNull() ? A2U("未命名") : m_oFileName;
        s.append(m_bChanged ? A2U("* - ") : A2U(" - "));
        s.append(m_oEditorName);
        m_pWin->setWindowTitle(s);
    }
}

bool CFileEditor::isChanged() const
{
    return m_bChanged;
}

bool CFileEditor::saveCheck()
{
    if (isChanged())
    {
        QString s;
        if (m_oFileName.isNull())
        {
            s.append(A2U("是否保存新文档?"));
        }
        else
        {
            s.append(A2U("是否将更改保存到 "));
            s.append(m_oFileName);
            s.append("?");
        }

        int iRes = QMessageBox::question(m_pWin, m_oEditorName, s, A2U("保存(&S)"), A2U("不保存(&N)"), A2U("取消(&C)"), 0, 2);
        switch (iRes)
        {
        case 0:
            return saveFile();
        case 2:
            return false;
        }
    }

    return true;
}

bool CFileEditor::newFile()
{
    if (!saveCheck())
    {
        return false;
    }

    m_oFileName = QString::null;
    setChanged(false);

    return true;
}

bool CFileEditor::openFile()
{
    if (!saveCheck())
    {
        return false;
    }

    QString s = QFileDialog::getOpenFileName(m_pWin, A2U("打开"), "", m_oFilter);
    if (s.isNull())
    {
        return false;
    }

    m_bChanged = false;
    newFile();
    m_oFileName = s;
    setChanged(false);

    return true;
}

bool CFileEditor::saveFile()
{
    if (m_oFileName.isNull())
    {
        return saveFileAs();
    }
    setChanged(false);

    return true;
}

bool CFileEditor::saveFileAs()
{
    QString s = QFileDialog::getSaveFileName(m_pWin, A2U("保存"), "", A2U("单位信息文件(*.uip);;所有文件(*.*)"));
    if (s.isNull())
    {
        return false;
    }
    m_oFileName = s;

    return saveFile();
}
