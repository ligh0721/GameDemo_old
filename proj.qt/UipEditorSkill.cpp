#include "CommInc.h"
#include "GameCtrl.h"
#include "Unit.h"
#include "Skill.h"
#include "SkillInfo.h"
#include "UipEditorSkill.h"
#include "ui_UipEditorSkill.h"


CUipEditorSkill::CUipEditorSkill(CUnitInfoPatch::VEC_SKILL_INDEX& roSkills, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CUipEditorSkill)
{
    ui->setupUi(this);

    // init org skill list
    for (COrgSkillInfo::MAP_SKILL_INFO::iterator it = g_oOrgSkillInfo.m_mapSkills.begin(); it != g_oOrgSkillInfo.m_mapSkills.end(); ++it)
    {
        ui->lstOrgSkill->addItem(A2U(it->second.sName.c_str()));
    }
    ui->lstOrgSkill->setCurrentRow(0);

    // init unit skill list
    m_vecSkills = roSkills;
    for (CUnitInfoPatch::VEC_SKILL_INDEX::iterator it = roSkills.begin(); it != roSkills.end(); ++it)
    {
        ui->lstUnitSkill->addItem(A2U(g_oOrgSkillInfo.m_mapSkills[*it].sName.c_str()));
    }
    if (!m_vecSkills.empty())
    {
        ui->lstUnitSkill->setCurrentRow(0);
    }
}

CUipEditorSkill::~CUipEditorSkill()
{
    delete ui;
}

CUnitInfoPatch::VEC_SKILL_INDEX CUipEditorSkill::getUipSkills()
{
    return m_vecSkills;
}

void CUipEditorSkill::on_btnAdd_clicked()
{
    int iIndex2 = ui->lstOrgSkill->currentRow();
    if (iIndex2 < 0)
    {
        QMessageBox::information(this, A2U("单位编辑器"), A2U("没有可选技能被选中"), QMessageBox::Ok);
        return;
    }
    m_vecSkills.push_back(iIndex2);
    int iIndex = m_vecSkills.size() - 1;
    ui->lstUnitSkill->addItem(A2U(g_oOrgSkillInfo.m_mapSkills[iIndex2].sName.c_str()));
    ui->lstUnitSkill->setCurrentRow(iIndex);
}

void CUipEditorSkill::on_btnDel_clicked()
{
    int iIndex = ui->lstUnitSkill->currentRow();
    if (iIndex < 0)
    {
        QMessageBox::information(this, A2U("单位编辑器"), A2U("没有技能被选中"), QMessageBox::Ok);
        return;
    }
    QListWidgetItem* pItem = ui->lstUnitSkill->item(iIndex);
    ui->lstUnitSkill->removeItemWidget(pItem);
    delete pItem;
    m_vecSkills.erase(m_vecSkills.begin() + iIndex);
}

void CUipEditorSkill::on_btnReplace_clicked()
{
    int iIndex = ui->lstUnitSkill->currentRow();
    if (iIndex < 0)
    {
        QMessageBox::information(this, A2U("单位编辑器"), A2U("没有技能被选中"), QMessageBox::Ok);
        return;
    }
    int iIndex2 = ui->lstOrgSkill->currentRow();
    if (iIndex2 < 0)
    {
        QMessageBox::information(this, A2U("单位编辑器"), A2U("没有可选技能被选中"), QMessageBox::Ok);
        return;
    }
    m_vecSkills[iIndex] = iIndex2;
    ui->lstUnitSkill->item(iIndex)->setText(A2U(g_oOrgSkillInfo.m_mapSkills[iIndex2].sName.c_str()));
}

void CUipEditorSkill::on_lstOrgSkill_currentRowChanged(int currentRow)
{
    if (currentRow < 0)
    {
        ui->txtNameDesc->clear();
    }
    QString s;
    s.append("<strong>");
    s.append(A2U(g_oOrgSkillInfo.m_mapSkills[currentRow].sName.c_str()));
    s.append(":</strong><p />\n");
    s.append(A2U(g_oOrgSkillInfo.m_mapSkills[currentRow].sDesc.c_str()));
    ui->txtNameDesc->setText(s);
}

void CUipEditorSkill::on_lstUnitSkill_currentRowChanged(int currentRow)
{
    int iIndex = ui->lstUnitSkill->currentRow();
    if (iIndex < 0)
    {
        return;
    }
    ui->lstOrgSkill->setCurrentRow(m_vecSkills[iIndex]);
}

void CUipEditorSkill::on_lstOrgSkill_itemDoubleClicked(QListWidgetItem *item)
{
    on_btnAdd_clicked();
}

void CUipEditorSkill::on_lstUnitSkill_itemDoubleClicked(QListWidgetItem *item)
{
    on_btnDel_clicked();
}

void CUipEditorSkill::on_btnClear_clicked()
{
    int iIndex = ui->lstUnitSkill->currentRow();
    if (iIndex < 0)
    {
        QMessageBox::information(this, A2U("单位编辑器"), A2U("没有技能被选中"), QMessageBox::Ok);
        return;
    }

    ui->lstUnitSkill->clear();
    m_vecSkills.clear();
}
