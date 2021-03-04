using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Slot : MonoBehaviour
{
    // 슬롯 상태 여부
    [HideInInspector]
    public Image m_Cheak;

    // 그리드 위치, 아이템 정보, 아이템 사이즈, 아이템 시작 위치 정보
    public Vector2Int m_GridPos;
    public GameObject m_StoredItemObject;
    public Vector2Int m_StoredItemSize;
    public Vector2Int m_StoredItemStartPos;

    // 아이템 상세 정보
    public Item m_Item;
    // 현재 슬롯 장착 여부
    public bool m_IsOccupied;

    // Start is called before the first frame update
    void Awake()
    {
        m_Cheak = transform.GetChild(0).GetComponent<Image>();
        m_Cheak.gameObject.SetActive(false);
    }
}
