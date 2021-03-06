﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Jamella : MonoBehaviour
{
    public GameObject name;

    SpriteRenderer jamella_spr;
    Color c, c_temp;

    static public bool Enter = false;
    static public bool Click = false;

    private void Start()
    {
        jamella_spr = gameObject.GetComponent<SpriteRenderer>();
        c_temp = jamella_spr.color;
        name.SetActive(false);
    }

    private void OnMouseOver()
    {
        name.SetActive(true);
        GameObject.Find("NPC").GetComponent<NPC>().MouseIn();
        c = new Color(1, 1, 1, 1);
        jamella_spr.color = c;
    }

    private void OnMouseExit()
    {
        name.SetActive(false);
        GameObject.Find("NPC").GetComponent<NPC>().MouseOut();
        c = c_temp;
        jamella_spr.color = c;
    }

    private void OnMouseDown()
    {
        Click = true;
    }

    private void OnTriggerStay2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Player")
        {
            if (Click)
            {
                GameObject.Find("Sorceress").GetComponent<PlayerControl>().Stop();
                Click = false;
            }
        }
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {

    }
}
