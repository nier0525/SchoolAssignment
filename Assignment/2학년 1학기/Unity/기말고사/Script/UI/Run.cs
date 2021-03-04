using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Run : MonoBehaviour
{
    public GameObject RUN;

    public void OnClick()
    {
        GameObject.Find("Vol").GetComponent<AudioSource>().Play();

        RUN.SetActive(true);
        this.gameObject.SetActive(false);

        if (UI_StateManager.Rbutton)
        {
            UI_StateManager.Rbutton = false;
        }
        else
        {
            UI_StateManager.Rbutton = true;
        }
    }

}
