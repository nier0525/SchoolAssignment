using System.Collections;
using System.Collections.Generic;
using System.Transactions;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class ItemInfo : MonoBehaviour
{
    private GameObject m_Inven;
    public static GameObject m_SelectedItem;
    public static Vector2Int m_SelectedItemSize;
    public static bool m_IsDragging = false;

    public Item m_Item;

    [HideInInspector]
    public Vector2Int m_Size;

    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        if (m_IsDragging)
            m_SelectedItem.transform.position = Input.mousePosition;
    }


    // 아이템 선택 시 드래그 활성화
    public static void SetSelectedItem(GameObject obj)
    {
        m_SelectedItem = obj;
        m_SelectedItemSize = obj.GetComponent<ItemInfo>().m_Size;
        obj.transform.SetParent(GameObject.Find("Drag").transform);
        m_IsDragging = true;        
    }

    // 초기화
    public static void ResetSelectedItem(string type)
    {
        m_SelectedItem = null;
        m_SelectedItemSize = Vector2Int.zero;

        m_IsDragging = false;
    }

    // 아이템 정보 갱신
    public void UpdateItem(Item item)
    {
        m_Item = item;
        GetComponent<Image>().sprite = m_Item.Icon;
        m_Size = m_Item.Size;

        float slotSize = transform.parent.parent.GetChild(0).GetComponent<CreateSlot>().SlotSize;
        RectTransform rect = GetComponent<RectTransform>();

        CanvasGroup canvas = GetComponent<CanvasGroup>();
        canvas.blocksRaycasts = false;

        rect.SetSizeWithCurrentAnchors(RectTransform.Axis.Horizontal, slotSize * m_Size.x);
        rect.SetSizeWithCurrentAnchors(RectTransform.Axis.Vertical, slotSize * m_Size.y);
        rect.localScale = Vector3.one;
    }
}
