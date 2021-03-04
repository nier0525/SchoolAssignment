using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class StBtCLK : MonoBehaviour
{
    public GameObject STC, QUEST;

    public void OnClick()
    {
        GameObject.Find("Vol").GetComponent<AudioSource>().Play();

        if (!STC.active)
        {
            if (QUEST.active)
            {
                QUEST.SetActive(false);
            }
            STC.SetActive(true);
        }

    }
}

// 스텟창 스크립트랑 상호작용
