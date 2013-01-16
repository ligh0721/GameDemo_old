#ifndef FILEEDITOR_H
#define FILEEDITOR_H

class CFileEditor
{
public:
    CFileEditor();
    virtual ~CFileEditor();

    virtual bool init(const QString& roName, const QString& roFilter, QWidget* pWin = NULL);

    void setChanged(bool bChanged = true);
    bool isChanged() const;

    bool saveCheck();

    virtual bool newFile();
    virtual bool openFile();
    virtual bool saveFile();
    virtual bool saveFileAs();

protected:
    QWidget* m_pWin;
    bool m_bChanged;
    QString m_oFileName;
    QString m_oEditorName;
    QString m_oFilter;
};

#endif // FILEEDITOR_H
