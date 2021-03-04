using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class hadrial : MonoBehaviour
{
    public GameObject name, balloon;

    SpriteRenderer hadrial_spr;
    Color c, c_temp;

    public static bool Enter = false;

    private void Start()
    {
        hadrial_spr = gameObject.GetComponent<SpriteRenderer>();
        c_temp = hadrial_spr.color;
        name.SetActive(false);
        balloon.SetActive(false);
    }

    private void OnMouseOver()
    {
        name.SetActive(true);
        GameObject.Find("NPC").GetComponent<NPC>().MouseIn();
        c = new Color(1, 1, 1, 1);
        hadrial_spr.color = c;
    }

    private void OnMouseExit()
    {
        name.SetActive(false);
        GameObject.Find("NPC").GetComponent<NPC>().MouseOut();
        c = c_temp;
        hadrial_spr.color = c;
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Player")
            Enter = true;
    }

    private void OnTriggerExit2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Player")
            Enter = false;
    }

    private void Update()
    {
        if (Quest.Q == 3 || Quest.Q == 5)
        {
            balloon.SetActive(true);
        }
        else
        {
            balloon.SetActive(false);
        }
    }
}
