using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// 아이템 정보 객체
public class Item
{
    public int GlobalID;
    public string Name;
    [HideInInspector] public Sprite Icon;

    [Range(1, 100)] public int level;

    [HideInInspector] public int CategoryID;
    [HideInInspector] public string CategoryName;

    [HideInInspector] public string Type;

    [HideInInspector] public Vector2Int Size;
    [HideInInspector] public int Option;

    public Item() {}

    public Item(Item item)
    {
        GlobalID = item.GlobalID;
        Name = item.Name;
        Icon = item.Icon;

        level = item.level;

        Type = item.Type;

        CategoryID = item.CategoryID;
        CategoryName = item.CategoryName;
        Size = item.Size;

        Option = item.Option;
    }
}
