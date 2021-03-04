using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Portal : MonoBehaviour
{
    public GameObject TownPortal;

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if(collision.gameObject.name == "Sorceress")
        {
            GameObject.Find("enterportal").GetComponent<AudioSource>().Play();

            GameObject.Find("DiabloBGM").GetComponent<AudioSource>().Stop();
            GameObject.Find("TownBGM").GetComponent<AudioSource>().Play();

            collision.gameObject.transform.position = TownPortal.transform.position;
            PlayerControl.tel = 0;
        }
    }
}
