using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class Short_Skill_Input : MonoBehaviour
{
    public GameObject player;
    private PlayerState state;
    //public GameObject UI_MAIN;
    public int i;
    public GameObject A_Text;
    public GameObject S_Text;
    public GameObject D_Text;
    public GameObject F_Text;
    public GameObject G_Text;
    public GameObject H_Text;
    public GameObject Short_Skill;

    public GameObject A_Image;
    public GameObject S_Image;
    public GameObject D_Image;
    public GameObject F_Image;
    public GameObject G_Image;
    public GameObject H_Image;

    public bool sw = false;
    void Start()
    {
        state = player.GetComponent<PlayerState>();
    }
    public void Quick()
    {
        //Debug.Log("!!!");
        sw = true;       
    }
    public void UQ()
    {
        sw = false;
    }
    public void Skill_Input()
    {
        Short_Skill.GetComponent<Image>().sprite = gameObject.GetComponent<Image>().sprite;
        Short_Skill.gameObject.name = gameObject.gameObject.name;

    }
    public void Skill_Left_Input()
    {
        state.L_Skill = i;
    }
    public void Skill_Right_Input()
    {
        state.R_Skill = i;
    }

    void Update()
    {
        if (sw)
        {
            if (Input.GetKeyDown(KeyCode.A))
            {
                Debug.Log("Input A");
                A_Text.transform.position = this.transform.position;
                A_Image.GetComponent<Image>().sprite = gameObject.GetComponent<Image>().sprite;
                state.A = i;
            }
            if (Input.GetKeyDown(KeyCode.S))
            {
                Debug.Log("Input S");
                S_Text.transform.position = this.transform.position;
                S_Image.GetComponent<Image>().sprite = gameObject.GetComponent<Image>().sprite;
                state.S = i;
            }
            if (Input.GetKeyDown(KeyCode.D))
            {
                Debug.Log("Input D");
                D_Text.transform.position = this.transform.position;
                D_Image.GetComponent<Image>().sprite = gameObject.GetComponent<Image>().sprite;
                state.D = i;
            }
            if (Input.GetKeyDown(KeyCode.F))
            {
                Debug.Log("Input F");
                F_Text.transform.position = this.transform.position;
                F_Image.GetComponent<Image>().sprite = gameObject.GetComponent<Image>().sprite;
                state.F = i;
            }
            if (Input.GetKeyDown(KeyCode.G))
            {
                Debug.Log("Input G");
                G_Text.transform.position = this.transform.position;
                G_Image.GetComponent<Image>().sprite = gameObject.GetComponent<Image>().sprite;
                state.G = i;
            }
            if (Input.GetKeyDown(KeyCode.H))
            {
                Debug.Log("Input H");
                H_Text.transform.position = this.transform.position;
                H_Image.GetComponent<Image>().sprite = gameObject.GetComponent<Image>().sprite;
                state.H = i;
            }
        }
    }
}
