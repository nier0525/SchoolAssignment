using System.Collections;
using System.Collections.Generic;
using System.Xml.Schema;
using UnityEngine;
using UnityEngine.Video;

// 아이템 장착 가능, 장착 상태, 장착 불가, 빈 상태를 표현 할 색
public struct SlotColorHighlights
{
    public static Color32 Green2
    { get { return new Color32(0, 255, 0, 25); } }

    public static Color32 Red
    { get { return new Color32(255, 0, 0, 25); } }

    public static Color32 Blue
    { get { return new Color32(0, 0, 255, 25); } }

    public static Color32 Blue2
    { get { return new Color32(0, 0, 255, 50); } }

    public static Color32 Empty
    { get { return new Color32(255, 255, 255, 0); } }

    public static Color32 Gray
    { get { return new Color32(255, 255, 255, 25); } }

}

public class SlotManager : MonoBehaviour
{
    // 전체 슬롯의 배열
    public Slot[,] m_Slot;
    // 현재 선택된 슬롯
    public GameObject m_highlightedSlot;

    // 그리드 사이즈
    [HideInInspector] public Vector2Int m_GridSize;

    Vector2Int TotalOffSet, CheakSize, CheakStartPos;
    Vector2Int OtherItemPos, OtherItemSize;

    int CheakState;
    bool isOverEdge = false;

    // Start is called before the first frame update
    void Start()
    {
        m_Slot = GetComponent<CreateSlot>().m_SlotList;

        // 초반 아이템 인벤토리 배치 작업
        InventorySort();
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetMouseButtonUp(0))
        {
            // 현재 선택된 슬롯이 있고, 드래그 하고 있는 아이템이 있는 경우와 없는 경우
            if (m_highlightedSlot != null && ItemInfo.m_SelectedItem != null && !isOverEdge)
            {
                switch(CheakState)
                {
                    // 빈 공간 아이템 장착
                    case 0:
                        StoreItem(ItemInfo.m_SelectedItem);
                        ColorChangeLoop(SlotColorHighlights.Blue, ItemInfo.m_SelectedItemSize, TotalOffSet);
                        ItemInfo.ResetSelectedItem("Inventory");
                        break;

                    // 이미 장착된 아이템과 교체
                    case 1:
                        ItemInfo.SetSelectedItem(SwapItem(ItemInfo.m_SelectedItem));
                        SlotSector.m_SectorScritp.PosOffSet();
                        ColorChangeLoop(SlotColorHighlights.Empty, OtherItemSize, OtherItemPos);
                        RefreshColor(true);
                        break;
                }
            }

            // 장착된 아이테 해제
            else if (m_highlightedSlot != null && ItemInfo.m_SelectedItem == null && m_highlightedSlot.GetComponent<Slot>().m_IsOccupied)
            {
                ColorChangeLoop(SlotColorHighlights.Empty, m_highlightedSlot.GetComponent<Slot>().m_StoredItemSize, m_highlightedSlot.GetComponent<Slot>().m_StoredItemStartPos);
                
                ItemInfo.SetSelectedItem(GetItem(m_highlightedSlot.GetComponent<Slot>()));
                SlotSector.m_SectorScritp.PosOffSet();
                RefreshColor(true);
            }
        }
    }

    // 아이템 사이즈가 슬롯에 장착이 가능한 지 여부 판단 함수
    public void CheakArea(Vector2Int size)
    {
        Vector2Int HalfOffSet = new Vector2Int();
        Vector2Int OverCheck = new Vector2Int();
        
        HalfOffSet.x = (size.x - (size.x % 2 == 0 ? 0 : 1)) / 2;
        HalfOffSet.y = (size.y - (size.y % 2 == 0 ? 0 : 1)) / 2;

        TotalOffSet = m_highlightedSlot.GetComponent<Slot>().m_GridPos - (HalfOffSet + SlotSector.m_PosOffSet);
        CheakStartPos = TotalOffSet;
        CheakSize = size;
        OverCheck = TotalOffSet + size;

        isOverEdge = false;

        if (OverCheck.x > m_GridSize.x)
        {
            CheakSize.x = m_GridSize.x - TotalOffSet.x;
            isOverEdge = true;
        }
        if (TotalOffSet.x < 0)
        {
            CheakSize.x = size.x + TotalOffSet.x;
            CheakStartPos.x = 0;
            isOverEdge = true;
        }
        if (OverCheck.y > m_GridSize.y)
        {
            CheakSize.y = m_GridSize.y - TotalOffSet.y;
            isOverEdge = true;
        }
        if (TotalOffSet.y < 0)
        {
            CheakSize.y = size.y + TotalOffSet.y;
            CheakStartPos.y = 0;
            isOverEdge = true;
        }
    }

    public void CheakArea2(Vector2Int size, int x, int y)
    {
        Vector2Int HalfOffSet = new Vector2Int();
        Vector2Int OverCheck = new Vector2Int();

        HalfOffSet.x = (size.x - (size.x % 2 == 0 ? 0 : 1)) / 2;
        HalfOffSet.y = (size.y - (size.y % 2 == 0 ? 0 : 1)) / 2;

        Vector2Int grid = new Vector2Int(x, y);
        TotalOffSet = grid - (HalfOffSet + SlotSector.m_PosOffSet);
        CheakStartPos = TotalOffSet;
        CheakSize = size;
        OverCheck = TotalOffSet + size;

        isOverEdge = false;

        if (OverCheck.x > m_GridSize.x)
        {
            CheakSize.x = m_GridSize.x - TotalOffSet.x;
            isOverEdge = true;
        }
        if (TotalOffSet.x < 0)
        {
            CheakSize.x = size.x + TotalOffSet.x;
            CheakStartPos.x = 0;
            isOverEdge = true;
        }
        if (OverCheck.y > m_GridSize.y)
        {
            CheakSize.y = m_GridSize.y - TotalOffSet.y;
            isOverEdge = true;
        }
        if (TotalOffSet.y < 0)
        {
            CheakSize.y = size.y + TotalOffSet.y;
            CheakStartPos.y = 0;
            isOverEdge = true;
        }
    }

    // 슬롯 상태 체크
    private int SlotCheck(Vector2 size)
    {
        GameObject obj = null;
        Slot instance;

        // 아이템이 해당 슬롯에 장착 가능한 지 체크 ( 그리드 범위 초과 )
        if (!isOverEdge)
        {
            for (int y = 0; y < size.y; y++)
            {
                for (int x = 0; x < size.x; x++)
                {
                    // 아이템 사이즈 만큼의 슬롯 정보
                    instance = m_Slot[CheakStartPos.x + x, CheakStartPos.y + y];

                    // 슬롯에 장착된 무기가 있는 경우
                    if (instance.m_IsOccupied)
                    {
                        // 오브젝트 정보 갱신
                        if (obj == null)
                        {
                            obj = instance.m_StoredItemObject;
                            OtherItemPos = instance.m_StoredItemStartPos;
                            OtherItemSize = obj.GetComponent<ItemInfo>().m_Size;
                        }
                        // 이후 다음 슬롯에서도 같은 경우
                        // 종류가 다른 무기가 인식 되는 경우 장착 불가 판정
                        else if (obj != instance.m_StoredItemObject)
                            return 2;
                    }
                }
            }

            // 슬롯에 아무것도 없는 경우 바로 장착 가능
            if (obj == null)
                return 0;
            // 슬롯에 한 종류의 무기가 있는 경우 교체 작업
            else
                return 1;
        }
        return 2;
    }

    // 색 변경
    public void ColorChangeLoop(Color32 color, Vector2Int size, Vector2Int start)
    {
        for (int y = 0; y < size.y; y++)
        {
            for (int x = 0; x < size.x; x++)
            {
                m_Slot[start.x + x, start.y + y].m_Cheak.gameObject.SetActive(true);
                m_Slot[start.x + x, start.y + y].m_Cheak.color = color;
            }
        }
    }

    // 빈 상태 색 변경
    public void ColorChangeLoop2(Vector2Int size, Vector2Int start)
    {
        Slot slot;

        for (int y = 0; y < size.y; y++)
        {
            for (int x = 0; x < size.x; x++)
            {
                slot = m_Slot[start.x + x, start.y + y];
                if (slot.m_IsOccupied != false)
                {
                    slot.m_Cheak.color = SlotColorHighlights.Blue2;
                }
                else
                {
                    slot.m_Cheak.color = SlotColorHighlights.Empty;
                }
            }
        }
    }

    // 상태에 따라 색 변경
    public void RefreshColor(bool enter)
    {
        if (enter)
        {
            CheakArea(ItemInfo.m_SelectedItemSize);
            CheakState = SlotCheck(CheakSize);

            switch(CheakState)
            {
                case 0: 
                    ColorChangeLoop(SlotColorHighlights.Green2, CheakSize, CheakStartPos);
                    break;
                case 1:
                    ColorChangeLoop(SlotColorHighlights.Gray, OtherItemSize, OtherItemPos);
                    //ColorChangeLoop(SlotColorHighlights.Empty, CheakSize, CheakStartPos);
                    break;
                case 2:
                    ColorChangeLoop(SlotColorHighlights.Red, CheakSize, CheakStartPos);
                    break;
            }
        }
        else
        {
            isOverEdge = false;
            ColorChangeLoop2(CheakSize, CheakStartPos);
            if (CheakState == 1)
                ColorChangeLoop(SlotColorHighlights.Blue2, OtherItemSize, OtherItemPos);
        }
    }

    // 아이템 정렬
    public void SortItem(GameObject item)
    {
        Vector2Int itemSize = item.GetComponent<ItemInfo>().m_Size;

        for (var y = 0; y < m_GridSize.y; y++)
        {
            for (var x = 0; x < m_GridSize.x; x++)
            {
                // 슬롯이 비어있을 경우
                if (!m_Slot[x, y].m_IsOccupied)
                {
                    // 아이템 사이즈와 현재 슬롯 위치를 비교하여 장착 가능 여부 판단
                    CheakArea2(itemSize, x, y);
                    CheakState = SlotCheck(CheakSize);

                    // 장착이 가능하다면 해당 슬롯에 장착 후 정보 갱신
                    if (CheakState == 0)
                    {
                        ItemInfo.SetSelectedItem(item);
                        StoreItem(ItemInfo.m_SelectedItem);                     
                        ColorChangeLoop(SlotColorHighlights.Blue2, ItemInfo.m_SelectedItemSize, TotalOffSet);
                        ItemInfo.ResetSelectedItem("Inventory");
                        return;
                    }
                }
            }
        }

        print("Inventory is Full");
    }

    // 인벤토리 전체 정렬
    public void InventorySort()
    {
        // 인벤토리 초기화
        for (var y = 0; y < m_GridSize.y; y++)
        {
            for (var x = 0; x < m_GridSize.x; x++)
            {
                m_Slot[x, y].m_StoredItemObject = null;
                m_Slot[x, y].m_Item = null;
                m_Slot[x, y].m_StoredItemSize = Vector2Int.zero;
                m_Slot[x, y].m_StoredItemStartPos = Vector2Int.zero;
                m_Slot[x, y].m_IsOccupied = false;

                m_Slot[x, y].m_Cheak.gameObject.SetActive(false);
            }
        }

        // 인벤토리 내에 아이템 정보들 가져오기
        ItemInfo[] item = GameObject.Find("InventoryItem").GetComponentsInChildren<ItemInfo>();

        // 아이템 정렬
        for (var i = 0; i < item.Length; i++)
            SortItem(item[i].gameObject);
    }

    // 아이템 장착
    void StoreItem(GameObject item)
    {
        Slot instance;
        Vector2Int itemSize = item.GetComponent<ItemInfo>().m_Size;

        for (int y = 0; y < itemSize.y; y++)
        {
            for (int x = 0; x < itemSize.x; x++)
            {
                instance = m_Slot[TotalOffSet.x + x, TotalOffSet.y + y];
                instance.m_StoredItemObject = item;
                instance.m_Item = item.GetComponent<ItemInfo>().m_Item;
                instance.m_StoredItemSize = itemSize;
                instance.m_StoredItemStartPos = TotalOffSet;
                instance.m_IsOccupied = true;

                instance.m_Cheak.color = SlotColorHighlights.Empty;
            }
        }

        item.transform.SetParent(transform.parent.GetChild(1));
        item.GetComponent<RectTransform>().pivot = Vector2.zero;
        item.transform.position = m_Slot[TotalOffSet.x, TotalOffSet.y].transform.position;
        item.GetComponent<CanvasGroup>().alpha = 1f;
    }

    // 아이템 해제
    GameObject GetItem(Slot slot)
    {
        Slot SlotObj = slot;
        GameObject refitem = SlotObj.m_StoredItemObject;
        Vector2Int tempitempos = SlotObj.m_StoredItemStartPos;
        Vector2Int size = SlotObj.m_StoredItemSize;

        Slot instance;
        for (int y = 0; y < size.y; y++)
        {
            for (int x = 0; x < size.x; x++)
            {
                instance = m_Slot[tempitempos.x + x, tempitempos.y + y];
                instance.m_StoredItemObject = null;
                instance.m_Item = null;
                instance.m_StoredItemSize = Vector2Int.zero;
                instance.m_StoredItemStartPos = Vector2Int.zero;
                instance.m_IsOccupied = false;

                instance.m_Cheak.gameObject.SetActive(false);
            }
        }

        refitem.GetComponent<RectTransform>().pivot = new Vector2(0.5f, 0.5f);
        refitem.GetComponent<CanvasGroup>().alpha = 0.5f;
        refitem.transform.position = Input.mousePosition;
        return refitem;
    }

    // 아이템 교체
    GameObject SwapItem(GameObject item)
    {
        GameObject temp;
        temp = GetItem(m_Slot[OtherItemPos.x, OtherItemPos.y]);
        StoreItem(item);
        return temp;
    }
}
