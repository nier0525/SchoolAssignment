using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

public class EquipmentManager : MonoBehaviour
{    
    // 현재 선택 중인 슬롯
    public EquipmentSlot m_CurrnetSlot;

    // 무기 슬롯 2 개를 가진 배열
    [HideInInspector] public EquipmentSlot[] m_Wenpon;

    int CheakState;

    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetMouseButtonUp(0))
        {
            // 현재 선택된 슬롯이 있고, 드래그 하고 있는 아이템이 있는 경우와 없는 경우
            if (m_CurrnetSlot != null && ItemInfo.m_SelectedItem != null)
            {
                switch (CheakState)
                {
                    // 빈 공간에 아이템 장착
                    case 0:
                        // 장착하려고 하는 무기가 두손 무기일 경우
                        if (ItemInfo.m_SelectedItem.GetComponent<ItemInfo>().m_Item.Type == "두손무기")
                        {
                            TwoHandWenpon_Enter();
                        }
                        else
                        {
                            StoreItem(ItemInfo.m_SelectedItem, m_CurrnetSlot);
                            ItemInfo.ResetSelectedItem("Equipment");
                        }
                        break;

                    // 이미 장착된 아이템과 교체
                    case 1:
                        // 두손 무기로 인해 잠겨진 슬롯인 경우
                        if (m_CurrnetSlot.m_Look)
                        {
                            TwoHandWenpon_Swap();
                        }

                        // 무기 슬롯이 모두 한손무기로 채워진 상태에서 두손무기를 장착하려는 경우
                        else if (m_Wenpon[0].m_IsOccupied && m_Wenpon[1].m_IsOccupied &&
                                ItemInfo.m_SelectedItem.GetComponent<ItemInfo>().m_Item.Type == "두손무기")
                        {
                            TwoHandWenpon_Swap2();
                        }
                        else
                        {
                            // 현재 슬롯에 장착된 무기가 두손무기가 아니거나, 드래그하고 있는 무기가 두손무기가 아닌 경우
                            if (m_CurrnetSlot.m_item.Type != "두손무기" ||
                                ItemInfo.m_SelectedItem.GetComponent<ItemInfo>().m_Item.Type != "두손무기")
                                TwoHandWenpon_Exit();

                            ItemInfo.SetSelectedItem(SwapItem(ItemInfo.m_SelectedItem, m_CurrnetSlot));
                            ColorChangeLoop2();
                            RefreshColor(true);
                        }
                        break;
                }
            }
            // 장착된 아이템 해제
            else if (m_CurrnetSlot != null && ItemInfo.m_SelectedItem == null)
            {
                GameObject item = GetItem(m_CurrnetSlot);

                if (item != null)
                {
                    // 장착을 해제한 장비가 두손무기인 경우
                    if (item.GetComponent<ItemInfo>().m_Item.Type == "두손무기")
                        TwoHandWenpon_Exit();

                    ColorChangeLoop2();
                    ItemInfo.SetSelectedItem(item);
                    RefreshColor(true);
                }
            }
        }
    }

    // 두손 무기 장착 
    private void TwoHandWenpon_Enter()
    {
        for (var i = 0; i < m_Wenpon.Length; i++)
        {
            // 선택된 무기 슬롯과 다른 무기 슬롯 찾기
            if (m_Wenpon[i] != m_CurrnetSlot)
            {
                // 무기 슬롯이 모두 비어있는 상태라면
                if (!m_Wenpon[i].m_IsOccupied)
                {
                    // 장비 장착
                    StoreItem(ItemInfo.m_SelectedItem, m_CurrnetSlot);

                    // 빈 슬롯 비활성화 작업
                    m_Wenpon[i].m_Cheak.gameObject.SetActive(true);
                    m_Wenpon[i].m_Cheak.color = SlotColorHighlights.Red;
                    m_Wenpon[i].m_Look = true;

                    // 초기화
                    ItemInfo.ResetSelectedItem("Equipment");
                }
                else
                {                    
                    TwoHandWenpon_Swap();

                    // 빈 슬롯 비활성화 작업
                    m_Wenpon[i].m_Cheak.gameObject.SetActive(true);
                    m_Wenpon[i].m_Cheak.color = SlotColorHighlights.Red;
                    m_Wenpon[i].m_Look = true;
                }
                break;
            }
        }
    }

    // 두손무기가 아닌 무기가 비활성화 상태의 슬롯에 장착하려는 경우
    private void TwoHandWenpon_Swap()
    {
        for (var i = 0; i < m_Wenpon.Length; i++)
        {
            // 비활성화 된 슬롯 활성화 시키기
            if (m_Wenpon[i].m_Look)
            {
                m_Wenpon[i].m_Cheak.color = SlotColorHighlights.Empty;
                m_Wenpon[i].m_Look = false;
            }

            // 장착된 두손 장비 찾기
            if (m_Wenpon[i] != m_CurrnetSlot)
            {
                // 교체
                ItemInfo.SetSelectedItem(SwapItem(ItemInfo.m_SelectedItem, m_Wenpon[i]));

                // 초기화
                m_Wenpon[i].m_StoredItemObject = null;
                m_Wenpon[i].m_item = null;
                m_Wenpon[i].m_IsOccupied = false;

                // 비활성화 해제
                m_CurrnetSlot.m_Look = false;

                ColorChangeLoop2();
                RefreshColor(true);

                return;
            }
        }
    }

    // 무기 슬롯이 모두 장착된 상태에서 두손무기를 장착하려는 경우
    private void TwoHandWenpon_Swap2()
    {
        for (var i = 0; i < m_Wenpon.Length; i++)
        {            
            if (m_Wenpon[i] != m_CurrnetSlot)
            {
                // 교체하려는 아이템 정보 임시 저장
                GameObject item = new GameObject();
                item = ItemInfo.m_SelectedItem;

                // 반대 슬롯에 있는 무기를 인벤토리 슬롯으로 보내주는 작업
                // 부모 오브젝트를 인벤토리 내에 아이템 오브젝트로 변경 후
                // 인벤토리 내에 정렬 함수를 통해 빈 자리를 찾고, 그 슬롯에 저장
                SlotManager sm = GameObject.Find("Inventory").GetComponent<SlotManager>();
                m_Wenpon[i].m_StoredItemObject.transform.SetParent(sm.transform.parent.GetChild(1));
                sm.SortItem(m_Wenpon[i].m_StoredItemObject);

                // 현재 슬롯에 장착된 아이템과 두손무기 교체
                ItemInfo.SetSelectedItem(SwapItem(item, m_CurrnetSlot));
                ColorChangeLoop2();
                RefreshColor(true);

                // 반대 슬롯 정보 초기화 후, 비활성화
                m_Wenpon[i].m_StoredItemObject = null;
                m_Wenpon[i].m_item = null;
                m_Wenpon[i].m_IsOccupied = false;

                m_Wenpon[i].m_Cheak.gameObject.SetActive(true);
                m_Wenpon[i].m_Cheak.color = SlotColorHighlights.Red;
                m_Wenpon[i].m_Look = true;
                break;
            }
        }
    }

    // 두손무기를 장착 해제하였을 경우
    private void TwoHandWenpon_Exit()
    {
        for (var i = 0; i < m_Wenpon.Length; i++)
        {
            if (m_Wenpon[i] != m_CurrnetSlot)
            {
                // 비활성화 해제
                if (m_Wenpon[i].m_Look)
                {
                    m_Wenpon[i].m_Cheak.color = SlotColorHighlights.Empty;
                    m_Wenpon[i].m_Look = false;
                }
                else
                {
                    // 한손무기가 장착된 슬롯에 두손무기를 장착 시킬 경우 비활성화 작업
                    if (ItemInfo.m_SelectedItem.GetComponent<ItemInfo>().m_Item.Type == "두손무기")
                    {
                        m_Wenpon[i].m_Cheak.gameObject.SetActive(true);
                        m_Wenpon[i].m_Cheak.color = SlotColorHighlights.Red;
                        m_Wenpon[i].m_Look = true;
                    }
                }
                break;
            }
        }
    }

    // 슬롯 상태 체크
    private int SlotCheak()
    {
        GameObject obj = null;
        EquipmentSlot instance = m_CurrnetSlot;

        // 현재 선택된 슬롯이 있는 경우
        if (instance.m_Enter)
        {
            // 드래그 중인 아이템 정보 저장 후 예외처리
            obj = instance.m_StoredItemObject;
            if (obj != instance.m_StoredItemObject) return 2;

            // 해당 슬롯에 장착된 무기가 없는 경우
            if (obj == null)
            {
                // 해당 슬롯이 비활성화 상태인 경우
                if (instance.m_Look)
                {
                    // 드래그 중인 무기가 두손무기일 때
                    if (ItemInfo.m_SelectedItem.GetComponent<ItemInfo>().m_Item.Type == "두손무기")
                        return 2;
                    // 각 슬롯과 아이템 종류가 일치하는 경우
                    if (instance.m_CategoryID == ItemInfo.m_SelectedItem.GetComponent<ItemInfo>().m_Item.CategoryID)
                        return 1;
                    // 장착이 불가능한 경우
                    else
                        return 2;
                }

                // 각 슬롯과 아이템 종류가 일치하는 경우
                if (instance.m_CategoryID == ItemInfo.m_SelectedItem.GetComponent<ItemInfo>().m_Item.CategoryID)                
                    return 0;                
                // 장착이 불가능한 경우
                else
                    return 2;
            }
            // 해당 슬롯에 이미 장착된 무기가 있는 경우
            else
            {
                // 각 슬롯과 아이템 종류가 일치하는 경우
                if (instance.m_CategoryID == ItemInfo.m_SelectedItem.GetComponent<ItemInfo>().m_Item.CategoryID)                
                    return 1;
                // 장착이 불가능한 경우
                else
                    return 2;
            }
        }

        return 2;
    }

    // 색 변경
    public void ColorChangeLoop(Color32 color)
    {
        m_CurrnetSlot.m_Cheak.gameObject.SetActive(true);
        m_CurrnetSlot.m_Cheak.color = color;
    }

    // 빈 상태 색 변경
    public void ColorChangeLoop2()
    {
        m_CurrnetSlot.m_Cheak.color = SlotColorHighlights.Empty;
    }

    // 상태에 따라 색 변경
    public void RefreshColor(bool enter)
    {
        if (enter)
        {
            CheakState = SlotCheak();

            switch(CheakState)
            {
                case 0:
                    ColorChangeLoop(SlotColorHighlights.Green2);
                    break;

                case 1:
                    ColorChangeLoop(SlotColorHighlights.Green2);
                    break;

                case 2:
                    ColorChangeLoop(SlotColorHighlights.Red);
                    break;
            }
        }
        else
        {
            ColorChangeLoop2();
        }
    }

    // 아이템 장착
    void StoreItem(GameObject item, EquipmentSlot slot)
    {
        // 장비 슬롯 정보
        EquipmentSlot instance = slot;

        // 정보 갱신
        instance.m_StoredItemObject = item;
        instance.m_item = item.GetComponent<ItemInfo>().m_Item;
        instance.m_IsOccupied = true;

        // 색 변경
        instance.m_Cheak.color = SlotColorHighlights.Empty;

        // 장비 아이템으로 부모 객체 변경
        // 위치 설정
        item.transform.SetParent(transform.parent.GetChild(1));
        item.GetComponent<RectTransform>().pivot = new Vector2(0.5f, 0.5f);
        item.transform.position = m_CurrnetSlot.transform.position;
        item.GetComponent<CanvasGroup>().alpha = 1f;
    }

    // 아이템 해제
    GameObject GetItem(EquipmentSlot _slot)
    {
        // 장비 슬롯 정보
        EquipmentSlot slot = _slot;
        // 장비 슬롯에 저장된 장비 정보 저장
        GameObject refitem = slot.m_StoredItemObject;
        // 장비가 없다면 null 반환
        if (refitem == null) return null;

        // 현재 슬롯의 정보
        EquipmentSlot instance = m_CurrnetSlot;

        // 초기화
        instance.m_StoredItemObject = null;
        instance.m_item = null;
        instance.m_IsOccupied = false;

        instance.m_Cheak.gameObject.SetActive(false);

        // 위치 지정
        refitem.GetComponent<RectTransform>().pivot = new Vector2(0.5f, 0.5f);
        refitem.GetComponent<CanvasGroup>().alpha = 0.5f;
        refitem.transform.position = Input.mousePosition;

        // 처음에 저장한 장비 정보 반환
        return refitem;
    }

    // 아이템 교체
    GameObject SwapItem(GameObject item, EquipmentSlot slot)
    {
        // 해제한 아이템 정보
        GameObject temp = GetItem(slot);
        // 아이템 장착
        StoreItem(item, m_CurrnetSlot);
        return temp;
    }
}
