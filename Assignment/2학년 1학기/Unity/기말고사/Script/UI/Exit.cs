using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Exit : MonoBehaviour
{
    public GameObject SX;
    public void OnClick()
    {
        GameObject.Find("Vol").GetComponent<AudioSource>().Play();

        if (SX.activeSelf == true)
        {
            SX.SetActive(false);
            PlayerControl.DontMove = false;
        }
        else
            SX.SetActive(true);     
    }
}
