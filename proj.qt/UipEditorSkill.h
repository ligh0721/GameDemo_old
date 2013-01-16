#ifndef UIPEDITORSKILL_H
#define UIPEDITORSKILL_H

#include <QDialog>
#include "CommInc.h"
#include "Unit.h"

class CUnitInfoPatch;

namespace Ui {
class CUipEditorSkill;
}

class CUipEditorSkill : public QDialog
{
    Q_OBJECT
    
public:
    explicit CUipEditorSkill(CUnitInfoPatch::VEC_SKILL_INDEX& roSkills, QWidget *parent = 0);
    ~CUipEditorSkill();

    CUnitInfoPatch::VEC_SKILL_INDEX getUipSkills();
    
private slots:
    void on_btnAdd_clicked();
    void on_btnDel_clicked();
    void on_btnReplace_clicked();
    void on_lstOrgSkill_currentRowChanged(int currentRow);
    void on_lstUnitSkill_currentRowChanged(int currentRow);
    void on_lstOrgSkill_itemDoubleClicked(QListWidgetItem *item);
    void on_lstUnitSkill_itemDoubleClicked(QListWidgetItem *item);

    void on_btnClear_clicked();

private:
    Ui::CUipEditorSkill *ui;
    CUnitInfoPatch::VEC_SKILL_INDEX m_vecSkills;
};

#endif // UIPEDITORSKILL_H
