using System.Collections;
using System.Collections.Generic;
using System.Data;
using UnityEngine;
using UnityEngine.UI;

public enum SKillPosition { ONE, TWO, THREE, FOUR }

[System.Serializable]
public struct SKillData
{ 
    public string m_Name;
    public Sprite m_Image;
    public int m_AttackCount;
    public string[] m_DamegeFormula;
    public Animator m_Anim;
    public AudioSource m_Sound;
    public GameObject m_FrontSkill;
    public int m_FrontSkillCount;
    public SKillPosition m_Position;
    public bool m_Active;
    public int m_SkillLevel;
    public int m_MaxSkillLevel;
    public string m_Info;

    [HideInInspector]
    public float[] m_Damege;

    public void InitData(int index)
    {
        m_Name = "None_" + index.ToString();
        m_AttackCount = 0;
        m_DamegeFormula = new string[10];
        m_FrontSkill = null;
        m_FrontSkillCount = 0;
        m_Info = "";
        m_Position = SKillPosition.ONE;
        m_SkillLevel = 0;
        m_MaxSkillLevel = 0;

        m_Image = null;
        m_Anim = null;
        m_Sound = null;

        m_Active = false;
        m_Damege = new float[10];
    }

    public void DamageOperation()
    {
        DataTable table = new DataTable();
        string temp;

        for (int i = 0; i < m_AttackCount; i++)
        {
            temp = m_DamegeFormula[i];
            temp = temp.Replace("SkillLevel", m_SkillLevel.ToString());
            temp = temp.Replace("Skill Level", m_SkillLevel.ToString());
            temp = temp.Replace("AttackCount", m_AttackCount.ToString());
            temp = temp.Replace("Attack Count", m_AttackCount.ToString());
            temp = temp.Replace("Combo", m_AttackCount.ToString());

            m_Damege[i] = float.Parse(table.Compute(temp, "").ToString());
        }
    }
}

[ExecuteInEditMode]
public class SkillDataBase : MonoBehaviour
{
    [SerializeField]
    [HideInInspector]
    SKillData Data;
    public void SetData(SKillData _data) { Data = _data; }
    public SKillData GetData() { return Data; }

    [HideInInspector]
    public Image m_image;
    [HideInInspector]
    public Animator m_anim;
    [HideInInspector]
    public AudioSource m_sound;

    private void Awake()
    {
        m_image = GetComponent<Image>();
        m_anim = GetComponent<Animator>();
        m_sound = GetComponent<AudioSource>();
    }

    private void Update()
    {
        if (!Data.m_Active)
        {
            GetComponent<Image>().color = new Color(0.5f, 0.5f, 0.5f);
        }
        else
        {
            GetComponent<Image>().color = new Color(1, 1, 1);
        }

        //UpdateData();
    }

    public void UpdateData()
    {
        gameObject.name = Data.m_Name;

        m_image.sprite = Data.m_Image;
        m_anim = Data.m_Anim;
        m_sound = Data.m_Sound;

        Data.DamageOperation();
    }

    public void SkillLevel_Up()
    {
        if (!Data.m_Active)
        {
            if (Data.m_FrontSkill == null) Data.m_Active = true;
            else
            {
                if (Data.m_FrontSkill.GetComponent<SkillDataBase>().GetData().m_SkillLevel >= Data.m_FrontSkillCount)                
                    Data.m_Active = true;                
            }
        }

        if (Data.m_MaxSkillLevel > Data.m_SkillLevel && Data.m_Active) Data.m_SkillLevel++;

        Data.DamageOperation();
    }
}