using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Short_Skill : MonoBehaviour
{
    public GameObject Skill_List;
    public GameObject player;
    private PlayerState state;
    private PlayerControl con;
    private PlayerMotion mo;
    public GameObject anotherSK;
    public GameObject A_Image;
    public GameObject S_Image;
    public GameObject D_Image;
    public GameObject F_Image;
    public GameObject G_Image;
    public GameObject H_Image;
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
        if (Input.GetKeyDown(KeyCode.A))
        {
            if (state.A >= 0)
            {
                gameObject.GetComponent<Image>().sprite = A_Image.GetComponent<Image>().sprite;
                state.R_Skill = state.A;
            }
        }
        else if (Input.GetKeyDown(KeyCode.S))
        {
            if (state.S >= 0)
            {
                gameObject.GetComponent<Image>().sprite = S_Image.GetComponent<Image>().sprite;
                state.R_Skill = state.S;
            }
        }
        else if (Input.GetKeyDown(KeyCode.D))
        {
            if (state.D >= 0)
            {
                gameObject.GetComponent<Image>().sprite = D_Image.GetComponent<Image>().sprite;
                state.R_Skill = state.D;
            }
        }
        else if (Input.GetKeyDown(KeyCode.F))
        {
            if (state.F >= 0)
            {
                gameObject.GetComponent<Image>().sprite = F_Image.GetComponent<Image>().sprite;
                state.R_Skill = state.F;
            }
        }
        else if (Input.GetKeyDown(KeyCode.G))
        {
            if (state.G >= 0)
            {
                gameObject.GetComponent<Image>().sprite = G_Image.GetComponent<Image>().sprite;
                state.R_Skill = state.G;
            }
        }
        else if (Input.GetKeyDown(KeyCode.H))
        {
            if (state.H >= 0)
            {
                gameObject.GetComponent<Image>().sprite = H_Image.GetComponent<Image>().sprite;
                state.R_Skill = state.H;
            }
        }

        if (mo.isCanUseSkill)
        {
            gameObject.GetComponent<Image>().color = new Color32(255, 255, 255, 255);
        }
        else
        {
            gameObject.GetComponent<Image>().color = new Color32(150, 0, 0, 255);
        }

        for (int i = 1; i <= 8; i++)
        {
            if (state.R_Skill == i)
                if (con.isDelay[i] == false)
                    mo.isCanUseSkill = true;
                else
                    mo.isCanUseSkill = false;
        }
    }
}

