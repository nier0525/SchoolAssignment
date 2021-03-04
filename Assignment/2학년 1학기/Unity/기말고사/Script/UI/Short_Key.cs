using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Short_Key : MonoBehaviour
{
    public GameObject Invt;
    public GameObject Stat;
    public GameObject Skill;
    public GameObject Quest;

    public void Mini_Panel_Inv()
    {
        GameObject.Find("Vol").GetComponent<AudioSource>().Play();

        Stat.SetActive(false);
        Skill.SetActive(false);
        if (Invt.activeSelf == false)
        {
            PlayerControl.DontMove = true;
            Invt.SetActive(true);
        }
        else
        {
            PlayerControl.DontMove = false;
            Invt.SetActive(false);
        }
    }
    public void Mini_Panel_Stat()
    {
        GameObject.Find("Vol").GetComponent<AudioSource>().Play();

        Quest.SetActive(false);
        if (Stat.activeSelf == false)
            Stat.SetActive(true);
        else
            Stat.SetActive(false);
    }
    public void Mini_Panel_Skill()
    {
        GameObject.Find("Vol").GetComponent<AudioSource>().Play();

        Stat.SetActive(false);
        Invt.SetActive(false);
        if (Skill.activeSelf == false)
            Skill.SetActive(true);
        else
            Skill.SetActive(false);
    }
    public void Mini_Panel_Quest()
    {
        GameObject.Find("Vol").GetComponent<AudioSource>().Play();

        Stat.SetActive(false);        
        if (Quest.activeSelf == false)
            Quest.SetActive(true);
        else
            Quest.SetActive(false);
    }


}
