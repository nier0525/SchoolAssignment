using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CreateItem : MonoBehaviour
{
    public static Dictionary<int, Item> m_ItemMap;
    public RectTransform m_ProtoType;
    public Transform m_InventoryItem;

    // Start is called before the first frame update
    void Awake()
    {
        Setting();
    }

    // 세팅
    // 준비한 csv 파일을 파싱하여 아이템 정보를 얻은 후, 아이템 생성
    void Setting()
    {
        m_ItemMap = new Dictionary<int, Item>();       

        Sprite[] resourece = Resources.LoadAll<Sprite>("Sprite");
        Dictionary<string, Sprite> SpriteMap = new Dictionary<string, Sprite>();

        for (var i = 0; i < resourece.Length; i++)
            SpriteMap.Add(resourece[i].name, resourece[i]);

        string path = "Item";
        List<Dictionary<string, object>> data = CSVReader.Read(path);
        for (var i = 0; i < data.Count; i++)
        {
            Item item = new Item();

            item.GlobalID = (int)data[i]["GlobalID"];
            item.Name = (string)data[i]["Name"];
            item.Icon = SpriteMap[(string)data[i]["Icon"]];
            item.level = (int)data[i]["Level"];
            item.CategoryID = (int)data[i]["CategoryID"];
            item.CategoryName = (string)data[i]["CategoryName"];
            item.Type = (string)data[i]["Type"];
            item.Size.x = (int)data[i]["SizeX"];
            item.Size.y = (int)data[i]["SizeY"];
            item.Option = (int)data[i]["Option"];

            m_ItemMap.Add(item.GlobalID, item);            
        }

        for (var i = 0; i < m_ItemMap.Count; i++)
        {
            var item = GameObject.Instantiate(m_ProtoType) as RectTransform;
            item.transform.SetParent(m_InventoryItem);

            item.GetComponent<ItemInfo>().UpdateItem(m_ItemMap[i + 1]);
            item.name = item.GetComponent<ItemInfo>().m_Item.Name;

        }

        m_ProtoType.gameObject.SetActive(false);
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
