using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraSize : MonoBehaviour
{
    public GameObject Skill, Status , inven, Quest;
    //public GameObject Panel01, Panel02, Panel03, Panel04;

    float width = 4f;
    float height = 3f;

    Vector3 temp;
    public bool status = false;
    public bool skill = false;

    // Start is called before the first frame update
    void Start()
    {
        // 카메라 비율 16:9 고정
        //Camera.main.aspect = width / height;
    }

    // 스킬, 스탯 활성화 카메라 시점 이동 함수

    public void LeftMove()
    {
        transform.position += new Vector3(2, 0, 0);
    }

    public void RightMove()
    {
        transform.position -= new Vector3(2, 0, 0);
    }

    public void Button01()
    {
        if ((Status.active || Quest.active) && !skill)
        {
            transform.position -= new Vector3(2, 0, 0);
            skill = true;
        }
        if (!Status.active && !Quest.active)
        {
            transform.position += new Vector3(2, 0, 0);
            skill = false;
        }
    }

    public void Button02()
    {
        if ((Skill.active || inven.active) && !status)
        {
            transform.position += new Vector3(2, 0, 0);
            status = true;
        }

        if (!Skill.active && !inven.active)
        {
            transform.position -= new Vector3(2, 0, 0);
            status = false;
        }
    }

    public void SkillCamera()
    {
        if (Skill.active && !status)
        {
            transform.position += new Vector3(2, 0, 0);
            status = true;
        }
        
        if (!Skill.active && !inven.active)
        {
            transform.position -= new Vector3(2, 0, 0);
            status = false;
        }
    }

    public void StatusCamera()
    {
        if (Status.active && !skill)
        {
            transform.position -= new Vector3(2, 0, 0);
            skill = true;
        }
        if (!Status.active && !Quest.active)
        {
            transform.position += new Vector3(2, 0, 0);
            skill = false;
        }
    }

    // Update is called once per frame
    void Update()
    {

    }
}
