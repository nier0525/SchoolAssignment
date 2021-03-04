using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SKBTCLK : MonoBehaviour
{
    public GameObject SKBG, INVBG;

    public void OnClick()
    {
        GameObject.Find("Vol").GetComponent<AudioSource>().Play();

        if (!SKBG.active)
        {
            if (INVBG.active)
            {
                INVBG.SetActive(false);
            }
            SKBG.SetActive(true);
        }
    }
    
}

// 스킬창이랑 상호작용