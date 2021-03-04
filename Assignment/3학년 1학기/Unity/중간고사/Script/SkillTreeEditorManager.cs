using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SkillTreeEditorManager : MonoBehaviour
{
    [SerializeField]
    [HideInInspector]
    SKillData m_SKillData;
    public void SetData(SKillData _data) { m_SKillData = _data; }
    public SKillData GetData() { return m_SKillData; }

    static SKillData[] m_data_list = new SKillData[48];
    public void SetData(SKillData _data, int i) { m_data_list[i] = _data; }
    public SKillData GetData(int i) { return m_data_list[i]; }

    GameObject[] m_skill;
    public int Length() { return m_skill.Length; }

    [HideInInspector]
    public SkillUI m_skillmanager;

    // Start is called before the first frame update
    void Start()
    {
        InitSkillInfo();
    }

    public void CreateSkill()
    {
        m_skillmanager.CreateSkill();
        m_skill = GameObject.FindGameObjectsWithTag("Skill");

        m_SKillData.InitData(Length());
    }

    public void InitSkillInfo()
    {
        m_skill = GameObject.FindGameObjectsWithTag("Skill");
        if (Length() == 0)
        {            
            Debug.Log("Skill Empty");           
            return;
        }

        for (int i = 0; i < Length(); i++)
        {
            m_data_list[i] = m_skill[i].GetComponent<SkillDataBase>().GetData();
            m_skill[i].GetComponent<SkillDataBase>().UpdateData();
        }
    }

    public void SetSkillInfo(int i)
    {
        if (Length() == 0)
        {
            Debug.Log("No Setting");
            return;
        }

        SetData(m_SKillData, i);
        m_skill[i].GetComponent<SkillDataBase>().SetData(m_SKillData);
        m_skill[i].GetComponent<SkillDataBase>().UpdateData();
        m_skillmanager.UpdateSkill(m_SKillData, i);
    }

    public void SelectSkill(int i)
    {
        if (Length() == 0) return;
        m_SKillData = m_data_list[i];
    }

    public void RemoveSkill(int i)
    {
        if (Length() == 0) return;
        m_skillmanager.RemoveSkill(i);
    }

    private void Update()
    {

    }
}
