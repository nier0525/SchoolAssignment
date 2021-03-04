using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ItemBox : MonoBehaviour
{
    public GameObject name, inven, box, cam;

    SpriteRenderer item_spr;
    Color c, c_temp;

    static public bool Stay = false;
    static public bool Click = false;
    bool Using = false;

    private void Start()
    {
        item_spr = gameObject.GetComponent<SpriteRenderer>();
        c_temp = item_spr.color;
        name.SetActive(false);
    }

    private void Update()
    {
        if ((!inven.active || !box.active) && Using)
        {
            PlayerControl.DontMove = false;
            inven.SetActive(false);
            box.SetActive(false);
            Using = false;
        }
    }

    private void OnTriggerStay2D(Collider2D collision)
    {
        if (collision.gameObject.name == "Sorceress")
        {
            if (Click)
            {
                GameObject.Find("Sorceress").GetComponent<PlayerControl>().Stop();
                PlayerControl.DontMove = true;
                Debug.Log("dd");
                cam.transform.position += new Vector3(2, 0, 0);
                inven.SetActive(true);
                box.SetActive(true);
                Click = false;
                Using = true;
            }
        }
    }

    private void OnMouseDown()
    {
        Click = true;
    }

    private void OnMouseOver()
    {
        name.SetActive(true);
        GameObject.Find("NPC").GetComponent<NPC>().MouseIn();
        c = new Color(1, 1, 1, 1);
        item_spr.color = c;
    }

    private void OnMouseExit()
    {
        name.SetActive(false);
        GameObject.Find("NPC").GetComponent<NPC>().MouseOut();
        c = c_temp;
        item_spr.color = c;
    }
}
