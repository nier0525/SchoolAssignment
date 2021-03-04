using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Panel_Button : MonoBehaviour
{
    public GameObject another;
    public GameObject Panels;

    public void OnClickAnother()
    {
        GameObject.Find("Vol").GetComponent<AudioSource>().Play();

        gameObject.SetActive(false);
        another.SetActive(true);

        if (Panels.activeSelf == true)
            Panels.SetActive(false);
        else
            Panels.SetActive(true);

    }
}
