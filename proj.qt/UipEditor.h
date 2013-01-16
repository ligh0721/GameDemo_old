#ifndef UIPEDITOR_H
#define UIPEDITOR_H

#include <QMainWindow>
#include "CommInc.h"
#include "Unit.h"
#include "FileEditor.h"


namespace Ui {
class CUipEditor;
}

class CUipEditor : public QMainWindow, public CFileEditor
{
    Q_OBJECT
    
public:
    explicit CUipEditor(QWidget *parent = 0);
    ~CUipEditor();

    void setUnitPatchInfo(CUnitInfoPatch* pUip);
    CUnitInfoPatch* getUnitPatchInfo();

    int getAttackValueCount(QList<CAttackValue::ATTACK_TYPE>* pAttack = NULL) const;
    void setAttackValueCount(const QList<CAttackValue::ATTACK_TYPE>& roList);

    void setSkills();

    virtual bool newFile();
    virtual bool openFile();
    virtual bool saveFile();
    virtual bool saveFileAs();
    
private slots:
    void on_comboBoxOrgUnit_currentIndexChanged(int index);
    void on_comboBoxAttackType_currentIndexChanged(int index);
    void on_txtBaseAttack_textChanged(const QString &arg1);
    void on_btnAdd_clicked();
    void on_btnUpdate_clicked();
    void on_btnDel_clicked();
    void on_btnClear_clicked();
    void on_lstUnits_currentRowChanged(int currentRow);
    void on_action_N_triggered();
    void on_action_O_triggered();
    void on_action_S_triggered();
    void on_action_A_triggered();
    void on_action_X_triggered();
    void on_btnSkill_clicked();
    void on_btnSkillClear_clicked();

    void on_lstUnits_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::CUipEditor *ui;
    CUnitInfoPatchManager m_oUipm;
    CAttackValue m_oEditAv;
    CUnitInfoPatch::VEC_SKILL_INDEX m_vecSkills;
};

#endif // UIPEDITOR_H
