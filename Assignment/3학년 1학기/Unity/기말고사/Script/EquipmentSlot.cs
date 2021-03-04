using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using System;

public class EquipmentSlot : MonoBehaviour, IPointerEnterHandler, IPointerExitHandler
{
    public Image m_Cheak;

    public GameObject m_StoredItemObject;
    public Item m_item;
    [Range(1, 9)] public int m_CategoryID;

    // 카테고리 별 ID
    private enum CATEGORYID { WENPON = 1, ARMOR, HELM, BELT, HANDS, BOOTS, RING, AMULET, ITEM }    
    public string GetCategoryID()
    { return Enum.GetName(typeof(CATEGORYID), m_CategoryID); }

    public bool m_IsOccupied = false;
    public bool m_Enter = false;
    public bool m_Look = false;

    public EquipmentSlot m_Equipment;
    private EquipmentManager m_Manager;

    // Start is called before the first frame update
    void Start()
    {
        m_Cheak = transform.GetChild(0).GetComponent<Image>();
        m_Cheak.gameObject.SetActive(false);

        // 슬롯 사이즈 가져오기
        float slotsize = GameObject.Find("Inventory").GetComponent<CreateSlot>().SlotSize;
        float w = slotsize;
        float h = slotsize;

        // 아이템 종류에 따라 크기 지정
        switch(m_CategoryID)
        {
            case (int)CATEGORYID.WENPON:
                w = slotsize * 2;
                h = slotsize * 4;
                break;

            case (int)CATEGORYID.HELM:
                w = slotsize * 2;
                h = slotsize * 2;
                break;

            case (int)CATEGORYID.ARMOR:
                w = slotsize * 2;
                h = slotsize * 3;
                break;

            case (int)CATEGORYID.BELT:
                w = slotsize * 2;
                h = slotsize;
                break;

            case (int)CATEGORYID.HANDS:
                w = slotsize * 2;
                h = slotsize * 2;
                break;

            case (int)CATEGORYID.BOOTS:
                w = slotsize * 2;
                h = slotsize * 2;
                break;

            case (int)CATEGORYID.RING:
                w = slotsize;
                h = slotsize ;
                break;

            case (int)CATEGORYID.AMULET:
                w = slotsize;
                h = slotsize;
                break;

            case (int)CATEGORYID.ITEM:
                w = slotsize;
                h = slotsize;
                break;
        }

        // 아이템 크기에 맞게 이미지 크기 변환
        RectTransform rect = GetComponent<RectTransform>();
        rect.SetSizeWithCurrentAnchors(RectTransform.Axis.Horizontal, w);
        rect.SetSizeWithCurrentAnchors(RectTransform.Axis.Vertical, h);
        rect.localScale = Vector3.one;

        m_Manager = GameObject.Find("Equipment").GetComponent<EquipmentManager>();        
    }
    
    // 마우스 Enter 이벤트
    public void OnPointerEnter(PointerEventData data)
    {
        m_Enter = true;

        // 현재 마우스 위치의 슬롯 정보 전달
        m_Equipment = this;
        m_Manager.m_CurrnetSlot = m_Equipment;

        // 아이템 장착 가능 여부를 판단함
        if (ItemInfo.m_SelectedItem != null)
            m_Manager.RefreshColor(true);
        if (m_Equipment.m_StoredItemObject != null && ItemInfo.m_SelectedItem == null)
            m_Manager.ColorChangeLoop(SlotColorHighlights.Green2);

    }

    // 마우스 Exit 이벤트
    public void OnPointerExit(PointerEventData data)
    {
        m_Enter = false;
        m_Equipment = this;

        // 초기화 작업
        if (ItemInfo.m_SelectedItem != null) m_Manager.RefreshColor(false);
        if (m_Equipment.m_StoredItemObject != null && ItemInfo.m_SelectedItem == null)
            m_Manager.ColorChangeLoop(SlotColorHighlights.Empty);

        // 두손 무기 장착 시, 다른 무기 슬롯을 비활성화
        if (m_Look)
            m_Manager.ColorChangeLoop(SlotColorHighlights.Red);

        m_Manager.m_CurrnetSlot = null;
    }
}
