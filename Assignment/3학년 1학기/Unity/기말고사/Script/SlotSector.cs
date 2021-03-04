using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public class SlotSector : MonoBehaviour, IPointerEnterHandler, IPointerExitHandler
{
    // 슬롯 정보
    public GameObject m_SlotParent;
    private Slot m_Slot;

    // 1~4 까지 나눈 선택 영역
    public int m_QuadNum;
    public static Vector2Int m_PosOffSet;
    public static SlotSector m_SectorScritp;

    private SlotManager m_Manager;

    // Start is called before the first frame update
    void Start()
    {
        // 슬롯 크기
        float slotSize = transform.parent.parent.GetComponent<CreateSlot>().SlotSize;
        RectTransform rect = GetComponent<RectTransform>();

        // 슬롯 크기의 비율로 크기 설정
        rect.SetSizeWithCurrentAnchors(RectTransform.Axis.Horizontal, slotSize / 2);
        rect.SetSizeWithCurrentAnchors(RectTransform.Axis.Vertical, slotSize / 2);
        rect.localScale = Vector3.one;

        m_Manager = gameObject.transform.parent.parent.GetComponent<SlotManager>();
        m_Slot = m_SlotParent.GetComponent<Slot>();
    }

    // 아이템 중심점을 맞추기 위한 작업
    public void PosOffSet()
    {
        if (ItemInfo.m_SelectedItemSize.x != 0 && ItemInfo.m_SelectedItemSize.x % 2 == 0)
        {
            switch (m_QuadNum)
            {
                case 1:
                    m_PosOffSet.x = 0;
                    break;

                case 2:
                    m_PosOffSet.x = -1;
                    break;

                case 3:
                    m_PosOffSet.x = 0;
                    break;

                case 4:
                    m_PosOffSet.x = -1;
                    break;

                default:
                    break;
            }
        }

        if (ItemInfo.m_SelectedItemSize.y != 0 && ItemInfo.m_SelectedItemSize.y % 2 == 0)
        {
            switch (m_QuadNum)
            {
                case 1:
                    m_PosOffSet.y = -1;
                    break;

                case 2:
                    m_PosOffSet.y = -1;
                    break;

                case 3:
                    m_PosOffSet.y = 0;
                    break;

                case 4:
                    m_PosOffSet.y = 0;
                    break;

                default:
                    break;
            }
        }
    }

    // 마우스 Enter 이벤트
    public void OnPointerEnter(PointerEventData data)
    {
        // 현재 마우스 위치의 슬롯 정보 전달
        m_SectorScritp = this;
        m_Manager.m_highlightedSlot = m_SlotParent;
        PosOffSet();

        // 아이템 장착 가능 여부를 판단함
        if (ItemInfo.m_SelectedItem != null)
            m_Manager.RefreshColor(true);
        if (m_Slot.m_StoredItemObject != null && ItemInfo.m_SelectedItem == null)
            m_Manager.ColorChangeLoop(SlotColorHighlights.Blue, m_Slot.m_StoredItemSize, m_Slot.m_StoredItemStartPos);

    }

    // 마우스 Exit 이벤트
    public void OnPointerExit(PointerEventData data)
    {
        m_SectorScritp = this;
        m_Manager.m_highlightedSlot = null;

        // 초기화 작업
        if (ItemInfo.m_SelectedItem != null)
            m_Manager.RefreshColor(false);

        m_PosOffSet = Vector2Int.zero;

        if (m_Slot.m_StoredItemObject != null && ItemInfo.m_SelectedItem == null)
            m_Manager.ColorChangeLoop(SlotColorHighlights.Blue2, m_Slot.m_StoredItemSize, m_Slot.m_StoredItemStartPos);
    }
}
