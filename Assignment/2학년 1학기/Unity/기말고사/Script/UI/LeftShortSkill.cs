using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class LeftShortSkill : MonoBehaviour
{
    public GameObject Skill_List;
    public GameObject player;
    private PlayerState state;
    private PlayerControl con;
    private PlayerMotion mo;
    public GameObject anotherSK;
    public GameObject SK_MANAGER;

    private void Start()
    {
        state = player.GetComponent<PlayerState>();
        con = player.GetComponent<PlayerControl>();
        mo = player.GetComponent<PlayerMotion>();
    }

    public void OnMouseDown()
    {
        GameObject.Find("Vol").GetComponent<AudioSource>().Play();

        if (Skill_List.activeSelf == false)
        {
            Skill_List.SetActive(true);
            Debug.Log("1");
        }
        else
        {
            Skill_List.SetActive(false);
        }
        anotherSK.SetActive(false);
    }


    private void Update()
    {
    }
}

