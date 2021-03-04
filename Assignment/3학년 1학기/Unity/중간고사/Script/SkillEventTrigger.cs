using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public class SkillEventTrigger : MonoBehaviour
{
    SkillDataBase m_data;
    SkillDataBase m_SkillObject;

    bool m_dragend;

    void EventTrigger_Init()
    {
        EventTrigger m_event = gameObject.AddComponent<EventTrigger>();

        EventTrigger.Entry entry_PointEnter = new EventTrigger.Entry();
        entry_PointEnter.eventID = EventTriggerType.PointerEnter;
        entry_PointEnter.callback.AddListener((data) => { OnPointerEnter((PointerEventData)data); });
        m_event.triggers.Add(entry_PointEnter);

        EventTrigger.Entry entry_PointExit = new EventTrigger.Entry();
        entry_PointExit.eventID = EventTriggerType.PointerExit;
        entry_PointExit.callback.AddListener((data) => { OnPointerExit((PointerEventData)data); });
        m_event.triggers.Add(entry_PointExit);

        EventTrigger.Entry entry_PointClick = new EventTrigger.Entry();
        entry_PointClick.eventID = EventTriggerType.PointerClick;
        entry_PointClick.callback.AddListener((data) => { OnPointerClick((PointerEventData)data); });
        m_event.triggers.Add(entry_PointClick);

        EventTrigger.Entry entry_PointUp = new EventTrigger.Entry();
        entry_PointUp.eventID = EventTriggerType.PointerUp;
        entry_PointUp.callback.AddListener((data) => { OnPointerUp((PointerEventData)data); });
        m_event.triggers.Add(entry_PointUp);

        EventTrigger.Entry entry_PointDragBegin = new EventTrigger.Entry();
        entry_PointDragBegin.eventID = EventTriggerType.BeginDrag;
        entry_PointDragBegin.callback.AddListener((data) => { OnPointerDragBegin((PointerEventData)data); });
        m_event.triggers.Add(entry_PointDragBegin);

        EventTrigger.Entry entry_PointDrag = new EventTrigger.Entry();
        entry_PointDrag.eventID = EventTriggerType.Drag;
        entry_PointDrag.callback.AddListener((data) => { OnPointerDrag((PointerEventData)data); });
        m_event.triggers.Add(entry_PointDrag);

        EventTrigger.Entry entry_PointDragEnd = new EventTrigger.Entry();
        entry_PointDragEnd.eventID = EventTriggerType.EndDrag;
        entry_PointDragEnd.callback.AddListener((data) => { OnPointerDragEnd((PointerEventData)data); });
        m_event.triggers.Add(entry_PointDragEnd);
    }

    // Start is called before the first frame update
    void Start()
    {
        EventTrigger_Init();
        m_data = GetComponent<SkillDataBase>();
        m_dragend = false;
    }

    void OnPointerEnter(PointerEventData data)
    {
        if (data.pointerCurrentRaycast.gameObject.tag != "Skill_Temp")
            SkillManager.Instance.InfoView(m_data.GetData());
    }

    void OnPointerExit(PointerEventData data)
    {
        SkillManager.Instance.CloseView();
    }

    void OnPointerClick(PointerEventData data)
    {
        if (data.pointerId != -1)
        {
            m_data.SkillLevel_Up();
            SkillManager.Instance.InfoView(m_data.GetData());
        }
    }

    void OnPointerDragBegin(PointerEventData data)
    {
        if (m_data.GetData().m_Active)
        {
            m_SkillObject = GameObject.Instantiate(m_data) as SkillDataBase;
            m_SkillObject.transform.SetParent(GameObject.Find("SkillUI").transform, false);

            m_SkillObject.gameObject.tag = "Skill_Temp";
            m_SkillObject.GetComponent<Image>().raycastTarget = false;
            m_SkillObject.GetComponent<Image>().color = new Color(0.5f, 0.5f, 0.5f);
            if (m_SkillObject.transform.childCount >= 2)
                 m_SkillObject.transform.GetChild(1).gameObject.SetActive(false);
        }
    }

    void OnPointerDrag(PointerEventData data)
    {
        if (m_data.GetData().m_Active)
        {
            Vector2 mousepos = Input.mousePosition;
            m_SkillObject.transform.position = mousepos;
        }
    }

    void OnPointerDragEnd(PointerEventData data)
    {
        if (m_data.GetData().m_Active)
        {
            Destroy(GameObject.FindGameObjectWithTag("Skill_Temp"));
        }
    }

    void OnPointerUp(PointerEventData data)
    {
        if (data.pointerCurrentRaycast.gameObject.tag == "SkillKey")
        {
            data.pointerCurrentRaycast.gameObject.GetComponent<SkillDataBase>().SetData(m_data.GetData());
            data.pointerCurrentRaycast.gameObject.GetComponent<SkillDataBase>().UpdateData();
        }
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
