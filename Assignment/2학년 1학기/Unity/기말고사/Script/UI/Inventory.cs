using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Inventory : MonoBehaviour
{
    public GameObject SlotOverlay;
    SlotPanel Slot_p;

    private void Start()
    {
        Slot_p = SlotOverlay.GetComponent<SlotPanel>();
    }
    ////아이템 리스트
    //public List<Item> items = new List<Item>();

    ////슬롯 사이즈 10x4
    //static int slotWidthSize = 10;
    //static int slotHeightSize = 4;

    ////슬롯 10x4
    //public Slot[,] slots = new Slot[slotWidthSize, slotHeightSize];

    ////슬롯 위치
    //int slotX;
    //int slotY;

    ////슬롯 크기
    //public int width = 29;
    //public int height = 30;

    //public Texture2D test;

    //public GameObject player;
    //private PlayerState state;
    //public Text GOLD_txt;

    //void Start()
    //{
    //    position.x = transform.gameObject.GetComponent<RectTransform>().rect.x;
    //    position.y = transform.gameObject.GetComponent<RectTransform>().rect.y;
    //    setSlots();
    //    //player = GameObject.Find("player"); // 플레이어 오브젝트 찾기
    //    state = player.GetComponent<PlayerState>(); // 플레이어 스탯 호출
    //}

    //void setSlots()
    //{
    //    Slot.test = test;
    //    for (int x=0; x< slotWidthSize; x++)
    //    {
    //        for (int y = 0; y < slotHeightSize; y++)
    //        {
    //            slots[x, y] = new Slot(new Rect(slotX + width*x,slotY + height*y,width,height));
    //        }
    //    }
    //}

    //void Update()
    //{
    //    GOLD_txt.text = "" + state.gold;
    //}

    //private void OnGUI()
    //{
    //    drawSlots();
    //}

    //void drawSlots()
    //{
    //    for(int x = 0; x < slotWidthSize; x++)
    //    {
    //        for (int y = 0; y < slotHeightSize; y++)
    //        {
    //            slots[x, y].draw(position.x,position.y );
    //        }
    //    }
    //}
}
