using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CreateSlot : MonoBehaviour
{
    public RectTransform m_ProtoType;

    // 전체 슬롯을 담을 배열
    [HideInInspector]
    public Slot[,] m_SlotList;

    // grid x, y;
    public int width;
    public int height;

    public float SlotSize;
    public float EdgePadding;

    // Start is called before the first frame update
    void Awake()
    {
        m_SlotList = new Slot[width, height];
        ResizePanel();
        CreateSlots();

        Vector2Int grid = new Vector2Int(width, height);
        GetComponent<SlotManager>().m_GridSize = grid;
    }

    // 설정된 정보로 슬롯 생성
    void CreateSlots()
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                RectTransform Slot = GameObject.Instantiate(m_ProtoType) as RectTransform;
                Slot.transform.SetParent(gameObject.transform);

                Slot.localPosition = new Vector3(x * SlotSize + EdgePadding, y * SlotSize + EdgePadding, 0);
                Slot.SetSizeWithCurrentAnchors(RectTransform.Axis.Horizontal, SlotSize);
                Slot.SetSizeWithCurrentAnchors(RectTransform.Axis.Vertical, SlotSize);

                Slot.localScale = Vector3.one;
                Slot.name = x.ToString() + "," + y.ToString();

                m_SlotList[x, y] = Slot.GetComponent<Slot>();
                m_SlotList[x, y].m_GridPos = new Vector2Int(x, y);
            }
        }

        m_ProtoType.gameObject.SetActive(false);
    }

    // 패널 크기 조정
    void ResizePanel()
    {
        float w, h;
        w = (width * SlotSize) + (EdgePadding * 2);
        h = (height * SlotSize) + (EdgePadding * 2);

        RectTransform rect = GetComponent<RectTransform>();
        rect.SetSizeWithCurrentAnchors(RectTransform.Axis.Horizontal, w);
        rect.SetSizeWithCurrentAnchors(RectTransform.Axis.Vertical, h);
        rect.localScale = Vector3.one;
    }
}
