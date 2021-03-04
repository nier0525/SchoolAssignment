using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class SkillManager : MonoBehaviour
{
    public static SkillManager Instance = null;

    public GameObject m_SkillInfo;
    Vector3 MousePos;

    public TextMeshProUGUI m_name, m_level, m_frontskill, m_info, m_currentinfo;

    [SerializeField]
    SkillDataBase[] m_Skill = new SkillDataBase[4];

    [HideInInspector]
    public bool view;

    // Start is called before the first frame update
    void Start()
    {
        if (Instance == null) Instance = this;
        else if (Instance != this) Destroy(gameObject);
        DontDestroyOnLoad(gameObject);

        view = false;
        m_SkillInfo.SetActive(false);

        m_currentinfo.text = "None";
    }

    public void InfoView(SKillData data)
    {
        m_SkillInfo.SetActive(true);

        m_name.text = data.m_Name;
        m_level.text = data.m_SkillLevel.ToString();
        if (data.m_FrontSkill == null) m_frontskill.text = "선행스킬 : None";
        else m_frontskill.text = "선행스킬 : "  + data.m_FrontSkill.name + " \n Lv " + data.m_FrontSkillCount;
        m_info.text = data.m_Info;

        view = true;
    }

    public void CloseView()
    {
        view = false;
        m_SkillInfo.SetActive(false);
    }

    // Update is called once per frame
    void Update()
    {
        if (view)
        {
            MousePos = Input.mousePosition;

            if (MousePos.y > Screen.height /2)
            {
                MousePos.y = MousePos.y - (Screen.height / 5);
            }
            else
            {
                MousePos.y = MousePos.y + (Screen.height / 5);
            }

            if (MousePos.x > Screen.width / 2)
            {
                MousePos.x = MousePos.x - (Screen.width / 8);
            }
            else
            {
                MousePos.x = MousePos.x + (Screen.width / 8);
            }

            m_SkillInfo.transform.position = new Vector3(MousePos.x, MousePos.y, 0);
        }

        if (Input.GetKeyDown(KeyCode.Q))
        {
            if (m_Skill[0].GetData().m_Active)
            {
                m_currentinfo.text = "방금 사용한 스킬 : " + m_Skill[0].GetData().m_Name + " / 스킬 데미지 (1타) : " + m_Skill[0].GetData().m_Damege[0];
            }
        }

        if (Input.GetKeyDown(KeyCode.W))
        {
            if (m_Skill[1].GetData().m_Active)
            {
                m_currentinfo.text = "방금 사용한 스킬 : " + m_Skill[1].GetData().m_Name + " / 스킬 데미지 (1타) : " + m_Skill[1].GetData().m_Damege[0];
            }
        }

        if (Input.GetKeyDown(KeyCode.E))
        {
            if (m_Skill[2].GetData().m_Active)
            {
                m_currentinfo.text = "방금 사용한 스킬 : " + m_Skill[2].GetData().m_Name + " / 스킬 데미지 (1타) : " + m_Skill[2].GetData().m_Damege[0];
            }
        }

        if (Input.GetKeyDown(KeyCode.R))
        {
            if (m_Skill[3].GetData().m_Active)
            {
                m_currentinfo.text = "방금 사용한 스킬 : " + m_Skill[3].GetData().m_Name + " / 스킬 데미지 (1타) : " + m_Skill[3].GetData().m_Damege[0];
            }
        }
    }
}
