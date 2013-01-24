#include "CommInc.h"
#include "UipEditor.h"
#include "ui_UipEditor.h"
#include "UipEditorSkill.h"
#include "UnitInfo.h"
#include "SkillInfo.h"


CUipEditor::CUipEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CUipEditor)
{
    ui->setupUi(this);
    // init org unit combo
    M_DEF_UM(pUm);
    M_DEF_OU(pOu);
    QString s;
    int n = pUm->m_mapUnitInfo.size();
    for (int i = 0; i < n; ++i)
    {
        ui->comboBoxOrgUnit->addItem(s.sprintf("[%d] %s", i, pUm->unitInfoByIndex(i)->m_sName.c_str()));
    }

    // init attack combo
    n = CAttackValue::CONST_MAX_ATTACK_TYPE;
    for (int i = 0; i < n; ++i)
    {
        ui->comboBoxAttackType->addItem(A2U(CAttackValue::getName((CAttackValue::ATTACK_TYPE)i, CAttackValue::kCnName)));
    }

    // init armor combo
    n = CArmorValue::CONST_MAX_ARMOR_TYPE;
    for (int i = 0; i < n; ++i)
    {
        ui->comboBoxArmorType->addItem(A2U(CArmorValue::getName((CArmorValue::ARMOR_TYPE)i, CArmorValue::kCnName)));
    }

    m_oUipm.init();

    CFileEditor::init(A2U("单位编辑器"), A2U("单位信息文件(*.uip);;所有文件(*.*)"), this);
    newFile();
}

CUipEditor::~CUipEditor()
{
    delete ui;
}

void CUipEditor::setUnitPatchInfo(CUnitInfoPatch *pUip)
{
    M_DEF_UM(pUm);
    ui->comboBoxOrgUnit->setCurrentIndex(pUip->m_iOrgUnitIndex);
    ui->txtName->setText(pUip->m_szName);
    ui->txtBaseMoveSpeed->setText(QString::number(pUip->m_fBaseMoveSpeed));
    ui->txtBaseAttackInterval->setText(QString::number(pUip->m_fBaseAttackInterval));
    ui->txtMinAttackRange->setText(QString::number(pUip->m_fAttackMinRange));
    ui->txtAttackRange->setText(QString::number(pUip->m_fAttackRange));
    ui->txtHostilityRange->setText(QString::number(pUip->m_fHostilityRange));
    ui->txtProjMoveSpeed->setText(QString::number(pUip->m_fProjectileMoveSpeed));
    ui->txtProjScale->setText(QString::number(pUip->m_fProjectileScale));
    m_oEditAv = pUip->m_oBaseAttackValue;
    QList<CAttackValue::ATTACK_TYPE> l;
    getAttackValueCount(&l);
    setAttackValueCount(l);
    ui->comboBoxAttackType->setCurrentIndex(l.isEmpty() ? 0 : l.first());
    ui->txtExAttackRand->setText(QString::number(pUip->m_fExAttackRandomRange));
    ui->comboBoxArmorType->setCurrentIndex(pUip->m_eArmorType);
    ui->txtBaseArmor->setText(QString::number(pUip->m_fBaseArmorValue));
    ui->txtMaxHp->setText(QString::number(pUip->m_fMaxHp));
    ui->chkFixed->setChecked(pUip->m_bIsFixed);
    ui->txtRewardGold->setText(QString::number(pUip->m_iRewardGold));
    ui->txtRewardExp->setText(QString::number(pUip->m_iRewardExp));
    m_vecSkills = pUip->m_vecSkills;
    setSkills();
}

CUnitInfoPatch* CUipEditor::getUnitPatchInfo()
{
    static CUnitInfoPatch oUip;
    oUip.m_iOrgUnitIndex = ui->comboBoxOrgUnit->currentIndex();
    QByteArray oBa = ui->txtName->text().toUtf8();
    int n = MIN(oBa.length(), sizeof(oUip.m_szName) - 1);
    memset(oUip.m_szName, 0, sizeof(oUip.m_szName));
    memmove(oUip.m_szName, oBa.data(), n);
    oUip.m_fBaseMoveSpeed = ui->txtBaseMoveSpeed->text().toFloat();
    oUip.m_fBaseAttackInterval = ui->txtBaseAttackInterval->text().toFloat();
    oUip.m_fAttackMinRange = ui->txtMinAttackRange->text().toFloat();
    oUip.m_fAttackRange = ui->txtAttackRange->text().toFloat();
    oUip.m_fHostilityRange = ui->txtHostilityRange->text().toFloat();
    oUip.m_fProjectileMoveSpeed = ui->txtProjMoveSpeed->text().toFloat();
    oUip.m_fProjectileScale = ui->txtProjScale->text().toFloat();
    oUip.m_oBaseAttackValue = m_oEditAv;
    oUip.m_fExAttackRandomRange = ui->txtExAttackRand->text().toFloat();
    oUip.m_eArmorType = (CArmorValue::ARMOR_TYPE)ui->comboBoxArmorType->currentIndex();
    oUip.m_fBaseArmorValue = ui->txtBaseArmor->text().toFloat();
    oUip.m_fMaxHp = ui->txtMaxHp->text().toFloat();
    oUip.m_bIsFixed = ui->chkFixed->isChecked();
    oUip.m_iRewardGold = ui->txtRewardGold->text().toInt();
    oUip.m_iRewardExp = ui->txtRewardExp->text().toInt();
    oUip.m_vecSkills = m_vecSkills;
    return &oUip;
}

int CUipEditor::getAttackValueCount(QList<CAttackValue::ATTACK_TYPE>* pAttack) const
{
    int iAc = 0;
    for (int i = 0; i < CAttackValue::CONST_MAX_ATTACK_TYPE; ++i)
    {
        if (m_oEditAv.getAttack((CAttackValue::ATTACK_TYPE)i) > FLT_EPSILON)
        {
            if (pAttack)
            {
                pAttack->append((CAttackValue::ATTACK_TYPE)i);
            }
            ++iAc;
        }
    }
    return iAc;
}

void CUipEditor::setAttackValueCount(const QList<CAttackValue::ATTACK_TYPE> &roList)
{
    if (roList.isEmpty())
    {
        ui->txtAttackTypeCount->setText(A2U("无"));
        return;
    }
    QString s;
    s.sprintf("%d", roList.count());
    s.append(A2U("种"));
    s.append(" (");
    for (int i = 0; i < roList.count(); ++i)
    {
        s.append(ui->comboBoxAttackType->itemText(roList.at(i)));
        s.append(i < roList.count() - 1 ? "+" : "");
    }
    s.append(")");
    ui->txtAttackTypeCount->setText(s);
}

void CUipEditor::setSkills()
{
    M_DEF_OS(pOs);
    ui->comboBoxSkills->clear();
    if (m_vecSkills.empty())
    {
        return;
    }
    int n = m_vecSkills.size();
    for (CUnitInfoPatch::VEC_SKILL_INDEX::iterator it = m_vecSkills.begin(); it != m_vecSkills.end(); ++it)
    {
        ui->comboBoxSkills->addItem(A2U(pOs->m_mapSkills[*it].sName.c_str()));
    }
}

bool CUipEditor::newFile()
{
    if (!CFileEditor::newFile())
    {
        return false;
    }

    m_oUipm.m_vecPatches.clear();
    ui->lstUnits->clear();
    ui->comboBoxOrgUnit->setCurrentIndex(0);
    ui->comboBoxOrgUnit->currentIndexChanged(0);

    return true;
}

bool CUipEditor::openFile()
{
    if (!CFileEditor::openFile())
    {
        return false;
    }

    int n = m_oUipm.addPatches(m_oFileName.toLocal8Bit().data());
    if (!n)
    {

        return false;
    }
    for (int i = 0; i < n; ++i)
    {
        ui->lstUnits->addItem(m_oUipm.patchByIndex(i)->m_szName);
    }
    if (n)
    {
        ui->lstUnits->setCurrentRow(0);
        ui->txtIndex->setText(QString::number(0));
    }

    return true;
}

bool CUipEditor::saveFile()
{
    if (!CFileEditor::saveFile())
    {
        return false;
    }

    m_oUipm.saveAsFile(m_oFileName.toLocal8Bit());

    return true;
}

bool CUipEditor::saveFileAs()
{
    if (!CFileEditor::saveFileAs())
    {
        return false;
    }

    return true;
}

void CUipEditor::on_comboBoxOrgUnit_currentIndexChanged(int index)
{
    M_DEF_UM(pUm);
    CUnitInfoPatch oUip;
    oUip.initWithUnitInfo(index);
    setUnitPatchInfo(&oUip);
}

void CUipEditor::on_comboBoxAttackType_currentIndexChanged(int index)
{
    ui->txtBaseAttack->setText(QString::number(m_oEditAv.getAttack((CAttackValue::ATTACK_TYPE)index)));
}

void CUipEditor::on_txtBaseAttack_textChanged(const QString &arg1)
{
    int iIndex = ui->comboBoxAttackType->currentIndex();
    float fAv = ui->txtBaseAttack->text().toFloat();
    m_oEditAv.setAttack((CAttackValue::ATTACK_TYPE)iIndex, fAv);
    QList<CAttackValue::ATTACK_TYPE> l;
    getAttackValueCount(&l);
    setAttackValueCount(l);
}

void CUipEditor::on_btnAdd_clicked()
{
    CUnitInfoPatch* pUip = getUnitPatchInfo();
    int iIndex = m_oUipm.addPatch(*pUip);
    ui->lstUnits->addItem(m_oUipm.patchByIndex(iIndex)->m_szName);
    ui->lstUnits->setCurrentRow(iIndex);
    ui->txtIndex->setText(QString::number(iIndex));
    setChanged();
}

void CUipEditor::on_btnUpdate_clicked()
{
    int iIndex = ui->lstUnits->currentRow();
    if (iIndex < 0)
    {
        QMessageBox::information(this, A2U("单位编辑器"), A2U("没有单位被选中"), QMessageBox::Ok);
        return;
    }
    CUnitInfoPatch* pUip = getUnitPatchInfo();
    m_oUipm.setPatch(iIndex, *pUip);
    ui->lstUnits->item(iIndex)->setText(pUip->m_szName);
    CUnitInfoPatch* p = m_oUipm.patchByIndex(iIndex);
    setChanged();
}

void CUipEditor::on_btnDel_clicked()
{
    int iIndex = ui->lstUnits->currentRow();
    if (iIndex < 0)
    {
        QMessageBox::information(this, A2U("单位编辑器"), A2U("没有单位被选中"), QMessageBox::Ok);
        return;
    }
    QListWidgetItem* pItem = ui->lstUnits->item(iIndex);
    ui->lstUnits->removeItemWidget(pItem);
    delete pItem;
    m_oUipm.m_vecPatches.erase(m_oUipm.m_vecPatches.begin() + iIndex);

    // new iIndex
    iIndex = ui->lstUnits->currentRow();
    if (iIndex < 0)
    {
        ui->txtIndex->clear();
    }
    else
    {
        ui->txtIndex->setText(QString::number(iIndex));
    }
    setChanged();
}

void CUipEditor::on_btnClear_clicked()
{
    int iIndex = ui->lstUnits->currentRow();
    if (iIndex < 0)
    {
        QMessageBox::information(this, A2U("单位编辑器"), A2U("没有单位被选中"), QMessageBox::Ok);
        return;
    }

    ui->lstUnits->clear();
    ui->txtIndex->clear();
    m_oUipm.m_vecPatches.clear();
    setChanged();
}

void CUipEditor::on_lstUnits_currentRowChanged(int currentRow)
{
    //QMessageBox::information(this, QString::number(ui->lstUnits->currentRow()), QMessageBox::Ok);
    int iIndex = ui->lstUnits->currentRow();
    if (iIndex < 0)
    {
        ui->txtIndex->clear();
        return;
    }
    CUnitInfoPatch* pUip = m_oUipm.patchByIndex(iIndex);
    setUnitPatchInfo(pUip);
    ui->txtIndex->setText(QString::number(iIndex));
    setUnitPatchInfo(pUip);
}

void CUipEditor::on_action_N_triggered()
{
    newFile();
}

void CUipEditor::on_action_O_triggered()
{
    openFile();
}

void CUipEditor::on_action_S_triggered()
{
    saveFile();
}

void CUipEditor::on_action_A_triggered()
{
    saveFileAs();
}

void CUipEditor::on_action_X_triggered()
{
    close();
}

void CUipEditor::on_btnSkill_clicked()
{
    CUipEditorSkill oDlg(m_vecSkills, this);
    if (oDlg.exec() != QDialog::Accepted)
    {
        return;
    }
    m_vecSkills = oDlg.getUipSkills();
    setSkills();
}

void CUipEditor::on_btnSkillClear_clicked()
{
    m_vecSkills.clear();
    setSkills();
}

void CUipEditor::on_lstUnits_itemDoubleClicked(QListWidgetItem *item)
{
    //on_btnDel_clicked();
}

void CUipEditor::on_lstUnits_clicked(const QModelIndex &index)
{
    if (ui->lstUnits->count() == 1)
    {
        on_lstUnits_currentRowChanged(ui->lstUnits->currentRow());
    }
}
