using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

[ExecuteInEditMode]
public class SkillUI : MonoBehaviour
{
    [SerializeField]
    RectTransform m_ProtoType;
    [SerializeField]
    RectTransform m_SkillLine;

    SkillDataBase m_Skill;

    static RectTransform[] m_SkillList = new RectTransform[48];

    [SerializeField]
    ScrollRect m_Scroll;

    float content_x, content_y;

    // Start is called before the first frame update
    void Awake()
    {
        m_Skill = GetComponentInChildren<SkillDataBase>();
        content_x = 0f;
        content_y = 618.4f;
    }

    public void CreateSkill()
    {
        var m_skill = GameObject.Instantiate(m_ProtoType) as RectTransform;
        m_skill.SetParent(transform, false);
        m_skill.anchoredPosition3D = new Vector3(0, 0, 0);

        m_skill.gameObject.SetActive(true);

        for (int i = 0; i < transform.childCount; i++)
        {
            m_SkillList[i] = transform.GetChild(i).GetComponent<RectTransform>();
        }
    }

    public void UpdateSkill(SKillData data, int index)
    {
        if (data.m_FrontSkill == null) return;

        for (int i = 0; i < transform.childCount; i++)
        {
            m_SkillList[i] = transform.GetChild(i).GetComponent<RectTransform>();            
        }

        RectTransform rect = data.m_FrontSkill.GetComponent<RectTransform>();
        RectTransform m_line;

        if (m_SkillList[index].childCount == 1)
        {
            m_line = GameObject.Instantiate(m_SkillLine) as RectTransform;
            m_line.SetParent(m_SkillList[index].transform, false);
        }
        else
        {
            m_line = m_SkillList[index].GetChild(1).GetComponent<RectTransform>();
        }

        switch(data.m_Position)
        {
            case SKillPosition.ONE:
                m_SkillList[index].anchoredPosition = new Vector3(rect.anchoredPosition.x + 100,rect.anchoredPosition.y + 100, 0);
                m_line.anchoredPosition = new Vector3(-70, 0, 0);
                m_line.localRotation = Quaternion.Euler(0, 0, 0);
                break;

            case SKillPosition.TWO:
                m_SkillList[index].anchoredPosition = new Vector3(rect.anchoredPosition.x + 100,rect.anchoredPosition.y - 100, 0);
                m_line.anchoredPosition = new Vector3(0, 70, 0);
                m_line.localRotation = Quaternion.Euler(0, 0, 90);
                break;

            case SKillPosition.THREE:
                m_SkillList[index].anchoredPosition = new Vector3(rect.anchoredPosition.x - 100,rect.anchoredPosition.y -100, 0);
                m_line.anchoredPosition = new Vector3(70, 0, 0);
                m_line.localRotation = Quaternion.Euler(0, 0, 0);
                break;

            case SKillPosition.FOUR:
                m_SkillList[index].anchoredPosition = new Vector3(rect.anchoredPosition.x - 100,rect.anchoredPosition.y + 100, 0);
                m_line.anchoredPosition = new Vector3(0, -70, 0);
                m_line.localRotation = Quaternion.Euler(0, 0, 90);
                break;
        }

        float max_x = 0, max_y = 0;
        float min_x = 0, min_y = 0;

        for (int i = 0; i < transform.childCount; i++)
        {
            if(m_SkillList[i].anchoredPosition.y > max_y)
            {
                max_y = m_SkillList[i].anchoredPosition.y;
            }
            if (m_SkillList[i].anchoredPosition.y < min_y)
            {
                min_y = m_SkillList[i].anchoredPosition.y;
            }
            if (m_SkillList[i].anchoredPosition.x > max_x)
            {
                max_x = m_SkillList[i].anchoredPosition.x;
            }
            if (m_SkillList[i].anchoredPosition.x < min_x)
            {
                min_x = m_SkillList[i].anchoredPosition.x;
            }
        }

        if (max_x >= 600)
        {
            m_Scroll.content.sizeDelta = new Vector2(content_x - ((150 * (max_x - 500) / 100) * -1), m_Scroll.content.sizeDelta.y);
        }
        else if (min_x <= -600)
        {
            m_Scroll.content.sizeDelta = new Vector2(content_x - ((150 * (min_x - -500) / 100)), m_Scroll.content.sizeDelta.y);
        }
        else
        {
            m_Scroll.content.sizeDelta = new Vector2(content_x, m_Scroll.content.sizeDelta.y);
        }

        if (max_y >= 300)
        {
            m_Scroll.content.sizeDelta = new Vector2(m_Scroll.content.sizeDelta.x, (content_y + (175 * ((max_y - 200) / 100))));
        }
        else if (min_y <= -300)
        {
            m_Scroll.content.sizeDelta = new Vector2(m_Scroll.content.sizeDelta.x, (content_y + (( 175 * ((min_y - -200) / 100)) * -1)));
        }
        else
        {
            m_Scroll.content.sizeDelta = new Vector2(m_Scroll.content.sizeDelta.x, content_y);
        }
    }

    public void RemoveSkill(int index)
    {
        DestroyImmediate(m_SkillList[index].gameObject);

        for (int i = 0; i < transform.childCount; i++)
        {
            m_SkillList[i] = transform.GetChild(i).GetComponent<RectTransform>();
        }
    }
}
